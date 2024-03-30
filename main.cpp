#include <string>
#include "Cpu.h"
#include "gameboy.h"

int main(int argc, char* argv[]) {

	std::string rom_path = "C:\\Users\\Stacy\\Desktop\\cppProjects\\DMG_Gameboy_TestRoms\\Tetris.gb";
	//std::string rom_path = "C:\\Users\\Stacy\\Desktop\\cppProjects\\DMG_Gameboy_TestRoms\\passed\\10-bit ops.gb";

	Bus bus;
	CpuInstructions cpuInstructions;
	Cpu cpu(bus, cpuInstructions);
	gameboy gameboy(cpu);

	gameboy.loadRom(rom_path);

	return 0;
}