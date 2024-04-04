#include "Gameboy.h"
#include <iostream>
#include <fstream>
#include <array>
#include <iomanip>
#include <SDL.h>
#include <chrono>

gameboy::gameboy(Cpu& cpu) : cpu(cpu)
{

}

gameboy::~gameboy()
{

}

void gameboy::loadRom(const std::string& filepath)
{
    FILE* file;

    if (fopen_s(&file, filepath.c_str(), "rb") != 0) {
        std::cout << "File not loaded:" << std::endl;
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    std::vector<uint8_t> rom_data(fileSize);
    fread(rom_data.data(), 1, fileSize, file);
    fclose(file);

    cpu.loadRom(rom_data);
    std::cout << "ROM TITLE: " << cpu.getTitle() << std::endl;

    emulate();
}

void gameboy::emulate()
{
    // Emulate until cpu reports an error

    std::vector<std::vector<std::vector<uint8_t>>> backgroundTiles;
    std::vector<Sprite> Sprites;
    std::vector<std::vector<std::vector<uint8_t>>> tileSet;

    bool running = true;

    while (running)
    {
        while (cpu.getHasNotBroken())
        {
            // HANDLE INPUTS
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }
            }

            // RUN INSTRUCTION
            cpu.runInstruction();

            // RENDER WHEN NEEDED
            if (cpu.frameReady)
            {
                cpu.setFrameReady(false);

                backgroundTiles = cpu.getBackgroundTiles();
                Sprites = cpu.getSprites();
                tileSet = cpu.getTileSet();

                engine.setBuffer(backgroundTiles, Sprites);
                engine.render();

                cpu.clearGpuBackgroundTiles();
            }
        }
        running = false;
    }

    cpu.printStatus();
    cpu.printTrace();
    cpu.printSerialPorts();
}

/*
void gameboy::updateVramViewer(std::vector<std::vector<std::vector<uint8_t>>> tileSet)
{
    vramViewerEngine.setBuffer(tileSet);

    bool running = true;
    auto startTime = std::chrono::steady_clock::now();

    while (running) {

        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
        if (elapsedTime >= 1000 / 10000) {
            running = false;
        }

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        vramViewerEngine.render();
    }
}
*/

void gameboy::write(uint16_t addr, uint8_t data)
{
	cpu.write(addr, data);
}

uint8_t gameboy::read(uint16_t addr)
{
	return cpu.read(addr);
}
