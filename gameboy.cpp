#include "Gameboy.h"
#include <iostream>
#include <fstream>
#include <array>
#include <iomanip>
#include <SDL.h>
#include <chrono>
#include <thread>
#include "imgui_impl_sdl2.h"

gameboy::gameboy(Cpu& cpu) : cpu(cpu)
{

}

gameboy::~gameboy()
{

}

void gameboy::emulate()
{
    // Emulate until cpu reports an error

    bool running = true;
    uint64_t lastDivIncTime = 0;
    uint64_t incrementInterval = 1000000 / 16384;

    uint8_t vramData[6144];

    while (running && cpu.getHasNotBroken())
    {

        SDL_Event event;
        // HANDLE INPUTS
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);

            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_RETURN: cpu.key_press(JOYPAD_SELECT); break;
                case SDLK_TAB: cpu.key_press(JOYPAD_START); break;
                case SDLK_b: cpu.key_press(JOYPAD_B); break;
                case SDLK_a: cpu.key_press(JOYPAD_A); break;
                case SDLK_DOWN: cpu.key_press(JOYPAD_DOWN); break;
                case SDLK_UP: cpu.key_press(JOYPAD_UP); break;
                case SDLK_LEFT: cpu.key_press(JOYPAD_LEFT); break;
                case SDLK_RIGHT: cpu.key_press(JOYPAD_RIGHT); break;
                }
                cpu.update_joypad_memory();
            }
            else if (event.type == SDL_KEYUP)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_RETURN: cpu.key_release(JOYPAD_SELECT); break;
                case SDLK_TAB: cpu.key_release(JOYPAD_START); break;
                case SDLK_b: cpu.key_release(JOYPAD_B); break;
                case SDLK_a: cpu.key_release(JOYPAD_A); break;
                case SDLK_DOWN: cpu.key_release(JOYPAD_DOWN); break;
                case SDLK_UP: cpu.key_release(JOYPAD_UP); break;
                case SDLK_LEFT: cpu.key_release(JOYPAD_LEFT); break;
                case SDLK_RIGHT: cpu.key_release(JOYPAD_RIGHT); break;
                }
                cpu.update_joypad_memory();
            }
        }

        if (!engine.ramViewerVisible && !engine.vramViewerVisible)
        {
            auto currentTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

            if ((currentTime - lastDivIncTime) >= incrementInterval)
            {
                cpu.incrementDivReg();
                lastDivIncTime = currentTime;
            }
            // RUN INSTRUCTION
            cpu.runInstruction();
        }

        // RENDER WHEN NEEDED
        if (cpu.frameReady || engine.ramViewerVisible || engine.vramViewerVisible)
        {
            cpu.setFrameReady(false);

            engine.setBuffer(cpu.getFrameBuffer());
            engine.render(*ram);

            int vramDataIndex = 0;
            for (uint16_t i = 0x8000; i <= 0x97FF; i++)
            {
                vramData[vramDataIndex++] = gpu->vram[i];
            }

            if (engine.vramViewerVisible)
            {
                vramViewer.createWindow();
                vramViewer.setBuffer(vramData);
                vramViewer.renderDebug();
            }
            else
            {
                vramViewer.destroyWindow();
            }

            //SDL_Delay(8);
        }
    }
    cpu.printTrace();
}

void gameboy::write(uint16_t addr, uint8_t data)
{
	cpu.write(addr, data);
}

uint8_t gameboy::read(uint16_t addr)
{
	return cpu.read(addr);
}
