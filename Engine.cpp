#include "Engine.h"
#include <iostream>
#include <bitset>
#include "Scale.h"
#include <string>
#include <sstream>
#include <iomanip>

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

void Engine::setBuffer(std::vector<uint16_t> tileRows)
{
    int pixelBufferIndex = 0;
    for (uint16_t tileRow : tileRows)
    {
        //std::cout << "TILE ROW: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(tileRow) << std::endl;
        for (int i = 0; i < 8; i++)
        {
            uint8_t color = (tileRow >> ((7 - i) * 2)) & 3;

            switch (color)
            {
            case 0:
                pixelBuffer[pixelBufferIndex++] = 0xFFFFFFFF; // White
                break;
            case 1:
                pixelBuffer[pixelBufferIndex++] = 0xFFC0C0C0; // Light gray
                break;
            case 2:
                pixelBuffer[pixelBufferIndex++] = 0xFF808080; // Dark gray
                break;
            case 3:
                pixelBuffer[pixelBufferIndex++] = 0xFF000000; // Black
                break;
            default:
                std::cout << "UNKNOWN VALUE, USING BLACK AS DEFAULT: " << std::dec << static_cast<int>(color) << std::endl;
                //std::cout << "TILE ROW WITH BAD VALUES: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(tileRow) << std::endl;
                pixelBuffer[pixelBufferIndex++] = 0xFF000000; // Black
                break;
            }
        }
    }
}

void Engine::setBuffer(std::vector<uint32_t> frameBuffer)
{
    pixelBuffer = frameBuffer;
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
