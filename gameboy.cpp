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

    while (cpu.getHasNotBroken())
    {
        cpu.runInstruction();
    }

    cpu.printStatus();
    cpu.printTrace();
    cpu.printSerialPorts();
}

void gameboy::write(uint16_t addr, uint8_t data)
{
	cpu.write(addr, data);
}

uint8_t gameboy::read(uint16_t addr)
{
	return cpu.read(addr);
}
