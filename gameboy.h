#pragma once
#include "Cpu.h"
#include <string>

class gameboy 
{
public:
	gameboy(Cpu& cpu);
	~gameboy();

	void loadRom(const std::string& filepath); // Loads Rom
	void write(uint16_t addr, uint8_t data); // Calls cpu.write, (writes data to ram at address addr)  
	uint8_t read(uint16_t addr); // Calls cpu.read (reads data from ram at address addr)

	void emulate(); // Starts emulation

private:
	Cpu& cpu;
};

