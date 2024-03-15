#include "Gameboy.h"
#include <iostream>
#include <fstream>
#include <array>
#include <iomanip>

gameboy::gameboy(Cpu& cpu) : cpu(cpu)
{

}

gameboy::~gameboy()
{

}

void gameboy::loadRom(const std::string& filepath)
{
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "File not loaded:" << std::endl;
        return;
    }

    file.seekg(0x0104, std::ios::beg); //Start the file at 0x104 for the header

    std::string title;
    bool hasNotPrintedTitle = true;

    // Read and print the ROM header from 0x0100 to 0x14F (inclusive)
    for (uint16_t addr = 0x104; addr <= 0x14F; ++addr)
    {
        uint8_t data;
        if (file.read(reinterpret_cast<char*>(&data), sizeof(data)))
        {
            if (addr > 0x0143 && title.size() > 0 && hasNotPrintedTitle) // Check if the title has been accumulated
            {
                std::cout << "Title: " << title << std::endl;
                hasNotPrintedTitle = false;
            }

            std::cout << "Data at addr: " << std::hex << std::setw(2) << addr << " " << std::setw(2) << static_cast<int>(data) << std::endl;

            if (addr >= 0x0134 && addr <= 0x0143) // Check if the address is within the range for the title
            {
                if (data != 0x00) // Append only if it's not 0x00 (null)
                {
                    title += static_cast<char>(data); // Append the ASCII character to the title string
                }
                else
                {
                    title += " "; // Replace null bytes with spaces
                }
            }
        }
        else
        {
            std::cout << "Error reading ROM file" << std::endl;
            return;
        }
    }

    romTitle = title;

    emulate();
}

void gameboy::emulate()
{
    file.seekg(0x0100, std::ios::beg); //Start the file at 0x100 to begin emulation

    while (cpu.getHasNotBroken())
    {
        std::cout << "TESTING" << std::endl;

        uint8_t instruction;
        file.seekg(cpu.getPc(), std::ios::beg);
        if (!file.read(reinterpret_cast<char*>(&instruction), sizeof(instruction)))
        {
            std::cerr << "Error reading instruction from file." << std::endl;
            break;
        }

        // Execute the instruction
        cpu.runInstruction(instruction);
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
