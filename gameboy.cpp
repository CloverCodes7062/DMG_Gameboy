#include "Gameboy.h"
#include <iostream>
#include <fstream>
#include <array>
#include <iomanip>
#include <SDL.h>

gameboy::gameboy(Cpu& cpu) : cpu(cpu)
{

}

gameboy::~gameboy()
{

}

void gameboy::loadRom(const std::string& filepath)
{
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "File not loaded:" << std::endl;
        return;
    }

    std::vector<uint8_t> rom_data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    file.close();

    cpu.loadRom(rom_data);
    std::cout << "ROM TITLE" << cpu.getTitle() << std::endl;

    emulate();
}

void gameboy::emulate()
{
    // Emulate until cpu reports an error

    while (cpu.getHasNotBroken())
    {
        cpu.runInstruction();
    }
}

void gameboy::write(uint16_t addr, uint8_t data)
{
	cpu.write(addr, data);
}

uint8_t gameboy::read(uint16_t addr)
{
	return cpu.read(addr);
}
