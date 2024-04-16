#include "Engine.h"
#include <iostream>
#include <bitset>
#include "Scale.h"
#include <string>
#include <sstream>
#include <iomanip>

Engine::Engine(int width, int height) : screenWidth(width), screenHeight(height), window(nullptr), renderer(nullptr), texture(nullptr) {

    window = SDL_CreateWindow("Gameboy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // ADD SDL_RENDERER_PRESENTVSYNC |  for vsync

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight);

    pixelBuffer.resize(screenWidth * screenHeight);

}

Engine::~Engine() {

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Engine::setBuffer(std::vector<uint32_t> frameBuffer)
{
    pixelBuffer = frameBuffer;
}

void Engine::setBuffer(uint8_t vramDataArray[6144])
{
    int bufferIndex = 0;
    std::vector<std::vector<uint16_t>> backgroundTiles;
    std::vector<uint16_t> tile;

    for (int i = 0; i < 6144; i += 2)
    {
        uint16_t tileRow = 0x0000;
        for (int j = 0; j < 8; j++)
        {
            uint8_t lsbBit = (vramDataArray[i] >> (7 - j)) & 0x01;
            uint8_t msbBit = (vramDataArray[i + 1] >> (7 - j)) & 0x01;

            uint8_t paletteIndex = (msbBit << 1) | lsbBit;

            tileRow |= (paletteIndex << ((7 - j) * 2));
        }

        tile.push_back(tileRow);

        if (tile.size() == 8)
        {
            backgroundTiles.push_back(tile);
            tile.clear();
        }
    }

    int tileSize = 8;
    int tilesPerRow = 32;
    int tilesPrinted = 0;

    for (int tileIndex = 0; tileIndex < backgroundTiles.size(); ++tileIndex)
    {
        int tileRow = tileIndex / tilesPerRow;

        if (tileRow < 0)
        {
            tileRow = 0;
        }

        int tileCol = tileIndex % tilesPerRow;

        for (int y = 0; y < tileSize; ++y)
        {
            uint16_t packedPixels = backgroundTiles[tileIndex][y];

            int scaledX = tileCol * tileSize;
            int scaledY = tileRow * tileSize + y;

            for (int sx = 0; sx < tileSize; sx++)
            {
                uint8_t pixelValue = (packedPixels >> ((7 - sx) * 2)) & 0x03;

                int pixelIndex = scaledY * screenWidth + scaledX + sx;

                switch (pixelValue)
                {
                    case 0: pixelBuffer[pixelIndex] = 0xFFFFFFFF; break;
                    case 1: pixelBuffer[pixelIndex] = 0xFFC0C0C0; break;
                    case 2: pixelBuffer[pixelIndex] = 0xFF808080; break;
                    case 3: pixelBuffer[pixelIndex] = 0xFF000000; break;
                    default: std::cout << "UNKNOWN VALUE" << std::endl; break;
                }
            }
        }
    }
}

void Engine::setBuffer(uint8_t vramDataArray[6144], uint8_t SCY, uint8_t SCX, uint16_t tileMapAddress, bool signedMode)
{

}

void Engine::render() {
    SDL_UpdateTexture(texture, nullptr, &pixelBuffer[0], screenWidth * sizeof(uint32_t));

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);

    SDL_RenderPresent(renderer);

    frameCount++;

    Uint32 currentTime = SDL_GetTicks64();
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
