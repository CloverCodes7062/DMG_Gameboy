#pragma once
#include <vector>

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
private:
	std::vector<uint8_t> ram;
};


