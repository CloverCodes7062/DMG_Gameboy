#pragma once

#include <cstdint>
#include "Bus.h"
#include <string>

class Cpu
{
public:
	Cpu(Bus& bus);
	~Cpu();

	void write(uint16_t addr, uint8_t data);
	uint8_t read(uint16_t addr);

	void runInstruction(uint8_t instruction);

	// Load entire rom into rom vector for access
	void loadRom(std::vector<uint8_t> romData);

	// Set Title
	void setTitle();

	// Helper functions
	bool getHasNotBroken();
	uint16_t getPc();
	void setHasNotBroken(bool value);
	std::string getTitle();

private:
	Bus& bus;
	bool hasNotBroken = true;
	uint16_t pc = 0x100;
	std::vector<uint8_t> rom;
	std::string romTitle;
};