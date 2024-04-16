#include <string>
#include "Cpu.h"
#include "gameboy.h"
#include "Mbc.h"
#include "MMU.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {

	std::string rom_path = "C:\\Users\\Stacy\\Desktop\\cppProjects\\DMG_Gameboy_TestRoms\\Pokemon Red.gb";
	//std::string rom_path = "C:\\Users\\Stacy\\Desktop\\cppProjects\\DMG_Gameboy_TestRoms\\passed\\11-op a,(hl).gb";

	std::vector<uint8_t> ram(0x10000, 0);

    std::ifstream rom_file(rom_path, std::ios::binary);
    if (!rom_file) {
        std::cout << "ROM not loaded." << std::endl;
        return -1;
    }

    std::vector<uint8_t> rom((std::istreambuf_iterator<char>(rom_file)), std::istreambuf_iterator<char>());
    rom_file.close();

    for (size_t addr = 0x0104; addr <= 0x0133; addr++)
    {
        ram[addr] = rom[addr];
    }
    for (size_t addr = 0x0134; addr <= 0x014D; addr++)
    {
        ram[addr] = rom[addr];
    }

    int romType = rom[0x0147];
    int romSize = (32 * (1 << rom[0x0148]));
    int romBanks = romSize / 16;
    int ramBanks;

    switch (rom[0x149])
    {
        case 00: ramBanks = 0; break;
        case 01: ramBanks = 0; break;
        case 02: ramBanks = 1; break;
        case 03: ramBanks = 4; break;
        case 04: ramBanks = 16; break;
        case 05: ramBanks = 8; break;
        default: std::cout << "INVALID RAM SIZE" << std::endl; break;
    }

    std::cout << "ROM TYPE: " << std::dec << romType << std::endl;
    std::cout << "ROM SIZE: " << std::dec << romSize << std::endl;
    std::cout << "RAM TYPE: " << std::dec << static_cast<int>(rom[0x0149]) << std::endl;

    Mbc mbc(ram, rom, romType, romBanks, ramBanks);
    Gpu gpu = Gpu();
    MMU mmu(mbc, ram);

    mmu.gpu = &gpu;
    gpu.mmu = &mmu;

	CpuInstructions cpuInstructions;
	Cpu cpu(cpuInstructions, mmu, mbc, rom, gpu);
	gameboy gameboy(cpu);

    gameboy.gpu = &gpu;

    gameboy.emulate();

	return 0;
}