#include "Bus.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <conio.h>

Bus::Bus() : ram(0x10000, 0)
{
	ram[0xFF00] = 0x0F;
	ram[0xFF0F] = 0xE0;
	ram[0xFFFF] = 0x00;
	//ram[0xFF00] &= ~(1 << 3); Sets Bit 3 (Select/Down)
}

Bus::~Bus()
{

}

void Bus::write(uint16_t addr, uint8_t data)
{
	//std::cout << "Writing to RAM: " << std::hex << std::setw(4) << std::setfill('0') << addr << std::endl;
	//std::cout << "Data: " << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data) << std::endl;
	if (addr == 0xFF00)
	{
		return;
	}

	if (addr == 0xFF04)
	{
		ram[addr] = 0x00;
		return;
	}

	if (addr <= 0x8000 && hasLoadedRom)
	{
		return;
	}
	else
	{
		ram[addr] = data;
	}
}

uint8_t Bus::read(uint16_t addr)
{
	return ram[addr];
}

uint32_t Bus::getRamSize()
{
	return ram.size();
}

void Bus::setRomType(uint8_t type)
{
	romType = type;
}

void Bus::setRomLoaded()
{
	romLoaded = true;
}

void Bus::writeToJoyPad(uint8_t data)
{
	ram[0xFF00] = data;
}

void Bus::incrementDivReg()
{
	uint8_t divValue = ram[0xFF04];
	divValue++;

	ram[0xFF04] = divValue;
}