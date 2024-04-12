#include <string>
#include "Cpu.h"
#include "gameboy.h"
#include "Mbc.h"
#include "MMU.h"
#include <iostream>

int main(int argc, char* argv[]) {

	std::string rom_path = "C:\\Users\\Stacy\\Desktop\\cppProjects\\DMG_Gameboy_TestRoms\\Kirby.gb";
	//std::string rom_path = "C:\\Users\\Stacy\\Desktop\\cppProjects\\DMG_Gameboy_TestRoms\\passed\\11-op a,(hl).gb";

	std::vector<uint8_t> ram(0x10000, 0);
    std::vector<uint8_t> rom;

    FILE* file;
    if (fopen_s(&file, rom_path.c_str(), "rb") != 0) {
        std::cout << "ROM not loaded:" << std::endl;
        return -1;
    }
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    std::vector<uint8_t> rom_data(fileSize);
    fread(rom_data.data(), 1, fileSize, file);
    fclose(file);

    rom = rom_data;

    ram[0xFF00] = 0x0F;
    ram[0xFF0F] = 0xE0;
    ram[0xFFFF] = 0x00;

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

    std::cout << "ROM TYPE: " << std::dec << romType << std::endl;

    switch (rom[0x149])
    {
        case 00:
            ramBanks = 0;
            break;
        case 01:
            ramBanks = 0;
            break;
        case 02:
            ramBanks = 1;
            break;
        case 03:
            ramBanks = 4;
            break;
        case 04:
            ramBanks = 16;
            break;
        case 05:
            ramBanks = 8;
            break;
        default:
            std::cout << "INVALID RAM SIZE" << std::endl;
            break;
    }

    Mbc mbc(ram, rom, romType, romBanks, ramBanks);
	MMU mmu(mbc, ram);

	CpuInstructions cpuInstructions;
	Cpu cpu(cpuInstructions, mmu, mbc, rom);
	gameboy gameboy(cpu);

    gameboy.emulate();

	return 0;
}