#pragma once
#include <cstdint>
#include "Mbc.h"

class MMU
{
public:
	MMU(Mbc& passedMbc, std::vector<uint8_t>& passedRam);
	~MMU();

	Mbc& mbc;
	std::vector<uint8_t>& ram;

	uint8_t read(uint16_t addr);
	void write(uint16_t addr, uint8_t data);
	void writeToJoyPad(uint8_t data);

	void setRomLoaded();
	void incrementDivReg();
};

