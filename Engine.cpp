#include "Engine.h"
#include <iostream>
#include <bitset>
#include "Scale.h"
#include <string>
#include <sstream>

Engine::Engine(int width, int height) : screenWidth(width), screenHeight(height), window(nullptr), renderer(nullptr), texture(nullptr) {

    window = SDL_CreateWindow("Gameboy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight);

    pixelBuffer.resize(screenWidth * screenHeight);
}

Engine::~Engine() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Engine::setBuffer(std::vector<std::vector<uint16_t>> tileMap, std::vector<Sprite> Sprites, uint8_t SCY, uint8_t SCX, bool is8x16Mode)
{
    int visibleWidth = 160;
    int visibleHeight = 144;

    for (int y = 0; y < visibleHeight; y++)
    {
        for (int x = 0; x < visibleWidth; x++)
        {
            uint8_t vx = x + SCX;
            uint8_t vy = y + SCY;

            uint16_t tileIndex = (vy / 8) * 32 + (vx / 8);

            std::vector<uint16_t>& tileData = tileMap[tileIndex];

            uint8_t pixelValue = (tileData[vy % 8] >> ((7 - (vx % 8)) * 2)) & 0x03;

            int pixelBufferIndex = y * visibleWidth + x;

            switch (pixelValue)
            {
                case 0:
                    pixelBuffer[pixelBufferIndex] = 0xFFFFFFFF; // White
                    break;
                case 1:
                    pixelBuffer[pixelBufferIndex] = 0xFFC0C0C0; // Light gray
                    break;
                case 2:
                    pixelBuffer[pixelBufferIndex] = 0xFF808080; // Dark gray
                    break;
                case 3:
                    pixelBuffer[pixelBufferIndex] = 0xFF000000; // Black
                    break;
                default:
                    std::cout << "UNKNOWN VALUE, USING BLACK AS DEFAULT" << std::endl;
                    pixelBuffer[pixelBufferIndex] = 0xFF000000; // Black
                    break;
            }
        }
    }

    for (const auto& sprite : Sprites) {
        uint8_t priority = sprite.attributes & 0x80;
        uint8_t yFlip = sprite.attributes & 0x40;
        uint8_t xFlip = sprite.attributes & 0x20;
        uint8_t dmgPalette = sprite.attributes & 0x10;

        int tileHeight = 8;
        if (is8x16Mode) {
            tileHeight = 16;
        }

        std::vector<uint16_t> tiles;

        if (is8x16Mode)
        {
            std::pair<std::vector<uint16_t>, std::vector<uint16_t>> tilePair = std::get<std::pair<std::vector<uint16_t>, std::vector<uint16_t>>>(sprite.tiles);
            tiles.insert(tiles.end(), tilePair.first.begin(), tilePair.first.end());
            tiles.insert(tiles.end(), tilePair.second.begin(), tilePair.second.end());

            //std::cout << "TILES SIZE: " << std::dec << tiles.size() << std::endl;
        }
        else
        {
            tiles = std::get<std::vector<uint16_t>>(sprite.tiles);
        }

        for (int y = 0; y < tileHeight; y++) {

            uint16_t packedPixels = tiles[yFlip ? (tileHeight - 1 - y) : y];

            for (int x = 0; x < 8; x++) {
                int adjustedX = (sprite.x - 8) + (xFlip ? (7 - x) : x);
                int adjustedY = (sprite.y - 16) + (yFlip ? (tileHeight - 1 - y) : y);

                if (adjustedX >= 0 && adjustedX < visibleWidth && adjustedY >= 0 && adjustedY < visibleHeight) {
                    uint8_t pixelValue = (packedPixels >> ((xFlip ? x : 7 - x) * 2)) & 0x03;

                    int pixelBufferIndex = adjustedY * visibleWidth + adjustedX;

                    if (pixelValue != 0) { // For sprites, 0 is transparent (do not render it)
                        switch (pixelValue) {
                        case 0:
                            pixelBuffer[pixelBufferIndex] = 0xFFFFFFFF; // White
                            break;
                        case 1:
                            pixelBuffer[pixelBufferIndex] = 0xFFC0C0C0; // Light gray
                            break;
                        case 2:
                            pixelBuffer[pixelBufferIndex] = 0xFF808080; // Dark gray
                            break;
                        case 3:
                            pixelBuffer[pixelBufferIndex] = 0xFF000000; // Black
                            break;
                        default:
                            std::cout << "UNKNOWN VALUE, USING BLACK AS DEFAULT" << std::endl;
                            pixelBuffer[pixelBufferIndex] = 0xFF000000; // Black
                            break;
                        }
                    }
                }
            }
        }
    }
}

void Engine::render() {
    SDL_UpdateTexture(texture, nullptr, &pixelBuffer[0], screenWidth * sizeof(uint32_t));

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);

    frameCount++;

    Uint32 currentTime = SDL_GetTicks();
    Uint32 elapsedTime = currentTime - lastFrameTime;

    if (elapsedTime >= 1000)
    {
        auto fps = static_cast<float>(frameCount) / (elapsedTime / 1000);
        frameCount = 0;
        lastFrameTime = currentTime;

        std::string windowTitle = "Gameboy | FPS: " + std::to_string(static_cast<int>(fps));
        SDL_SetWindowTitle(window, windowTitle.c_str());
    }
    SDL_SetWindowSize(window, 480, 432);
    SDL_SetWindowResizable(window, SDL_TRUE);
}
