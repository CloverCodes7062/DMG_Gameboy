#include "Gameboy.h"
#include <iostream>
#include <fstream>
#include <array>
#include <iomanip>
#include <SDL.h>
#include <chrono>
#include <thread>

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

    bool running = true;
    uint64_t lastDivIncTime = 0;
    uint64_t incrementInterval = 1000000 / 16384;

    while (running && cpu.getHasNotBroken())
    {

        // HANDLE INPUTS
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_s:
                {
                    cpu.setHasNotBroken(false);
                    break;
                }
                case SDLK_RETURN:
                {
                    cpu.writeToJoyPad(0x17);
                    break;
                }
                case SDLK_TAB:
                {
                    cpu.writeToJoyPad(0x1B);
                    break;
                }
                case SDLK_b:
                {
                    cpu.writeToJoyPad(0x1D);
                    break;
                }
                case SDLK_a:
                {
                    cpu.writeToJoyPad(0x1E);
                    break;
                }
                case SDLK_DOWN:
                {
                    cpu.writeToJoyPad(0x27);
                    break;
                }
                case SDLK_UP:
                {
                    cpu.writeToJoyPad(0x2B);
                    break;
                }
                case SDLK_LEFT:
                {
                    cpu.writeToJoyPad(0x2D);
                    break;
                }
                case SDLK_RIGHT:
                {
                    cpu.writeToJoyPad(0b11101111 & 0b11111110);
                    break;
                }
                }
            }
            else if (event.type == SDL_KEYUP)
            {
                cpu.writeToJoyPad(0xFF);
            }
        }

        auto currentTime = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now().time_since_epoch()
        ).count();

        if ((currentTime - lastDivIncTime) >= incrementInterval)
        {
            cpu.incrementDivReg();
            lastDivIncTime = currentTime;
        }
        // RUN INSTRUCTION
        cpu.runInstruction();

        // RENDER WHEN NEEDED
        if (cpu.frameReady)
        {
            cpu.setFrameReady(false);

            engine.setBuffer(cpu.getFrameBuffer());
            engine.render();
        }
    }

    cpu.printStatus();
    cpu.printTrace();
    cpu.printRombank();
}

void gameboy::write(uint16_t addr, uint8_t data)
{
	cpu.write(addr, data);
}

uint8_t gameboy::read(uint16_t addr)
{
	return cpu.read(addr);
}
