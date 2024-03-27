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

    pixelBuffer.resize(screenWidth * screenHeight);

}

Engine::~Engine() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Engine::setBuffer(std::vector<std::vector<std::vector<uint8_t>>>& tiles)
{
    //std::cout << "TILES SIZE: " << tiles.size() << std::endl;

    /* DEBUG PRINTS OUT ALL TILES
    for (auto& vec2D : tiles)
    {
        for (auto& vec1D : vec2D)
        {
            for (auto& value : vec1D)
            {
                std::cout << std::bitset<1>(value) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    */

    int tileSize = 8;
    int tilesPerRow = 32;
    int tilesPrinted = 0;

    for (int tileIndex = 0; tileIndex < tiles.size(); ++tileIndex) {
        int tileRow = tileIndex / tilesPerRow;
        int tileCol = tileIndex % tilesPerRow;

        for (int y = 0; y < tileSize; ++y) {
            for (int x = 0; x < tileSize; ++x) {
                uint8_t pixelValue = tiles[tileIndex][y][x];

                int pixelIndex = ((tileRow * tileSize) + y) * screenWidth + ((tileCol * tileSize) + x);
                pixelBuffer[pixelIndex] = pixelValue == 0 ? 0xFFFFFFFF : 0xFF000000;
            }
        }
    }
}

void Engine::renderTile(int x, int y, std::vector<uint8_t>& tile) {
}

void Engine::render() {
    SDL_UpdateTexture(texture, nullptr, &pixelBuffer[0], screenWidth * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}
