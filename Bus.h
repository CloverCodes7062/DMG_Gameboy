#pragma once
#include <vector>
#include "Mbc.h"

class Bus
{
public:
	Bus();
	~Bus();

	void write(uint16_t addr, uint8_t data); // Writes to ram
	uint8_t read(uint16_t addr); // Reads from ram and returns a uint8_t
	uint32_t getRamSize(); // Returns Ram Size
	void setRomType(uint8_t type); // Sets the Rom Type
	uint8_t romType; // Rom Cart Type
	void setRomLoaded(); // Sets Rom Loaded to True (Used to prevent illegal writes to ROM after loading)
	bool romLoaded = false;
	void writeToJoyPad(uint8_t data); // Writes to the Joypad
	bool hasLoadedRom = false;
	void incrementDivReg();

	void setRom(std::vector<uint8_t> romData);

	void printRombank();

	bool ram_enabled = false;
private:
	std::vector<uint8_t> rom;
	std::vector<uint8_t> ram;
	uint16_t rom_bank_offset_hi = 0x4000;
	uint8_t rom_banks_size;

	uint16_t rom_bank = 0x00;
};


