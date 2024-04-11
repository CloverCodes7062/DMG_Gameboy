#include "MMU.h"
#include <iostream>

MMU::MMU(Mbc& passedMbc, std::vector<uint8_t>& passedRam) 
	: mbc(passedMbc), ram(passedRam)
{

}

MMU::~MMU()
{

}

uint8_t MMU::read(uint16_t addr)
{
	return mbc.read_byte(addr);
}

void MMU::write(uint16_t addr, uint8_t data)
{
	if (addr == 0xFF00)
	{
		return;
	}

	if (addr == 0xFF04)
	{
		ram[0xFF04] = 0x00;
		return;
	}

	if (addr >= 0xFEA0 && addr <= 0xFEFF)
	{
		std::cout << "ATTEMPTED TO WRITE TO UNUSABLE RAM LOCATION" << std::endl;
		return;
	}

	mbc.write_byte(addr, data);
}

void MMU::incrementDivReg()
{
	uint8_t divValue = ram[0xFF04];
	divValue++;

	ram[0xFF04] = divValue;
}

void MMU::writeToJoyPad(uint8_t data)
{
	ram[0xFF00] = data;
}

void MMU::setRomLoaded()
{
	mbc.setRomLoaded();
}
