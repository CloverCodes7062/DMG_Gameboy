#include "Engine.h"
#include <iostream>
#include <bitset>

Engine::Engine(int width, int height) : screenWidth(width), screenHeight(height), window(nullptr), renderer(nullptr), texture(nullptr) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) 
    {
        return;
    }

    window = SDL_CreateWindow("SDL Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if (window == nullptr) 
    {
        SDL_Quit();
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) 
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight);
    if (texture == nullptr) 
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    pixelBuffer.resize((screenWidth * 2) * (screenHeight * 2));

}

Engine::~Engine() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Engine::setBuffer(std::vector<std::vector<std::vector<uint8_t>>>& tiles, std::vector<Sprite> Sprites)
{
    int tileSize = 8;
    int tilesPerRow = 32;
    int tilesPrinted = 0;
    int scale = 3;

    for (int tileIndex = 0; tileIndex < tiles.size(); ++tileIndex) {
        int tileRow = tileIndex / tilesPerRow;
        int tileCol = tileIndex % tilesPerRow;

        for (int y = 0; y < tileSize; ++y) {
            for (int x = 0; x < tileSize; ++x) {
                uint8_t pixelValue = tiles[tileIndex][y][x];

                int scaledX = tileCol * tileSize * scale + x * scale;
                int scaledY = tileRow * tileSize * scale + y * scale;

                for (int sy = 0; sy < scale; ++sy) {
                    for (int sx = 0; sx < scale; ++sx) {

                        int pixelIndex = (scaledY + sy) * screenWidth + (scaledX + sx);
                        switch (pixelValue) {
                        case 0:
                            pixelBuffer[pixelIndex] = 0xFFFFFFFF;  // White
                            break;
                        case 1:
                            pixelBuffer[pixelIndex] = 0xFFC0C0C0;  // Light gray
                            break;
                        case 2:
                            pixelBuffer[pixelIndex] = 0xFF808080;  // Dark gray
                            break;
                        case 3:
                            pixelBuffer[pixelIndex] = 0xFF000000;  // Black
                            break;
                        default:
                            std::cout << "UNKNOWN VALUE, USING BLACK AS DEFAULT" << std::endl;
                            pixelBuffer[pixelIndex] = 0xFF000000;  // Black
                            break;
                        }
                    }
                }
            }
        }
    }

    for (const auto& sprite : Sprites) 
    {
        uint8_t priority = sprite.attributes & 0x80;
        uint8_t yFlip = sprite.attributes & 0x40;
        uint8_t xFlip = sprite.attributes & 0x20;
        uint8_t dmgPalette = sprite.attributes & 0x10;

        for (int y = 0; y < 8; ++y) 
        {
            for (int x = 0; x < 8; ++x) 
            {

                int scaledX = (sprite.x - 8) * scale + x * scale;
                int scaledY = (sprite.y - 16) * scale + y * scale;

                uint8_t pixelValue = sprite.tile[y][x];

                if (scaledX >= 0 && scaledX < (160 * scale) && scaledY >= 0 && scaledY < (144 * scale))
                {
                    for (int sy = 0; sy < scale; ++sy)
                    {
                        for (int sx = 0; sx < scale; ++sx)
                        {
                            int pixelIndex = (scaledY + sy) * screenWidth + (scaledX + sx);
                            if (pixelValue != 0) // For sprites, 0 is transparent (do not render it)
                            {
                                switch (pixelValue)
                                {
                                case 1:
                                    pixelBuffer[pixelIndex] = 0xFFC0C0C0;  // Light gray
                                    break;
                                case 2:
                                    pixelBuffer[pixelIndex] = 0xFF808080;  // Dark gray
                                    break;
                                case 3:
                                    pixelBuffer[pixelIndex] = 0xFF000000;  // Black
                                    break;
                                default:
                                    std::cout << "UNKNOWN VALUE, USING BLACK AS DEFAULT" << std::endl;
                                    pixelBuffer[pixelIndex] = 0xFF000000;  // Black
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void Engine::setBuffer(std::vector<std::vector<std::vector<uint8_t>>>& tiles)
{
    int tileSize = 8;
    int tilesPerRow = 32;
    int tilesPrinted = 0;
    int scale = 3;

    for (int tileIndex = 0; tileIndex < tiles.size(); ++tileIndex) {
        int tileRow = tileIndex / tilesPerRow;
        int tileCol = tileIndex % tilesPerRow;

        for (int y = 0; y < tileSize; ++y) {
            for (int x = 0; x < tileSize; ++x) {
                uint8_t pixelValue = tiles[tileIndex][y][x];

                int scaledX = tileCol * tileSize * scale + x * scale;
                int scaledY = tileRow * tileSize * scale + y * scale;

                for (int sy = 0; sy < scale; ++sy) {
                    for (int sx = 0; sx < scale; ++sx) {

                        int pixelIndex = (scaledY + sy) * screenWidth + (scaledX + sx);
                        switch (pixelValue) {
                        case 0:
                            pixelBuffer[pixelIndex] = 0xFFFFFFFF;  // White
                            break;
                        case 1:
                            pixelBuffer[pixelIndex] = 0xFFC0C0C0;  // Light gray
                            break;
                        case 2:
                            pixelBuffer[pixelIndex] = 0xFF808080;  // Dark gray
                            break;
                        case 3:
                            pixelBuffer[pixelIndex] = 0xFF000000;  // Black
                            break;
                        default:
                            std::cout << "UNKNOWN VALUE, USING BLACK AS DEFAULT" << std::endl;
                            pixelBuffer[pixelIndex] = 0xFF000000;  // Black
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
}
