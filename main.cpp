#include <string>
#include "Cpu.h"
#include "gameboy.h"

int main(int argc, char* argv[]) {

	std::string rom_path = "C:\\Users\\Stacy\\Desktop\\cppProjects\\DMG_Gameboy_TestRoms\\03-op sp,hl.gb";

	Bus bus;
	Cpu cpu(bus);
	gameboy gameboy(cpu);

	gameboy.loadRom(rom_path);

	return 0;
}