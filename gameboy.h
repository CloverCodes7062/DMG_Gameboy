#pragma once
#include "Cpu.h"
#include <string>
#include "Engine.h"

class gameboy 
{
public:
	gameboy(Cpu& cpu);
	~gameboy();

	void loadRom(const std::string& filepath); // Loads Rom
	void write(uint16_t addr, uint8_t data); // Calls cpu.write, (writes data to ram at address addr)  
	uint8_t read(uint16_t addr); // Calls cpu.read (reads data from ram at address addr)

	void emulate(); // Starts emulation

	// SDL Engine For Game View
	Engine engine = Engine(256 * 3, 256 * 3); // CHANGE SCALE IN ENGINE AS WELL
	//
	//Engine vramViewerEngine = Engine(256 * 3, 256 * 3);

	void updateVramViewer(std::vector<std::vector<std::vector<uint8_t>>> tileSet);

private:
	Cpu& cpu;
};

