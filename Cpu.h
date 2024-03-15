#pragma once

#include <cstdint>
#include "Bus.h"

class Cpu 
{
public:
	Cpu(Bus& bus);
	~Cpu();

	void write(uint16_t addr, uint8_t data);
	uint8_t read(uint16_t addr);
	bool getHasNotBroken();
	uint16_t getPc();
	void setHasNotBroken(bool value);

	void runInstruction(uint8_t instruction);

private:
	Bus& bus;
	bool hasNotBroken = true;
	uint16_t pc = 0x100;
};