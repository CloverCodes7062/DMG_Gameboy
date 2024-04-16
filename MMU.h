#pragma once
#include <cstdint>
#include "Mbc.h"
#include <chrono>
#include "Gpu.h"
#include "MiscRegs.h"

class Gpu;

class MMU
{
public:
	MMU(Mbc& passedMbc, std::vector<uint8_t>& passedRam);
	~MMU();

	Mbc& mbc;
	Gpu* gpu;
	std::vector<uint8_t>& ram;
	uint8_t joypad = 0xFF;
	uint8_t read(uint16_t addr);
	void write(uint16_t addr, uint8_t data);

	void setRomLoaded();
	void incrementDivReg();

	void handleDMATransfer();

};

