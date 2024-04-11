#include "Bus.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <conio.h>
#include "Mbc.h"

Bus::Bus() : ram(0x10000, 0)
{
	ram[0xFF00] = 0x0F;
	ram[0xFF0F] = 0xE0;
	ram[0xFFFF] = 0x00;
}

Bus::~Bus()
{

}

void Bus::write(uint16_t addr, uint8_t data)
{
	if (romType == 0x13) // MBC3 + RAM + BATTERY
	{
		if (addr >= 0x0000 && addr <= 0x1FFF)
		{
			if (data == 0x0A)
			{
				ram_enabled = true;
			}
			else if (data == 0x00)
			{
				ram_enabled = false;
			}
		}
		else if (addr >= 0x2000 && addr <= 0x3FFF)
		{
			uint8_t bankNumber = (data);
			rom_bank = (bankNumber == 0) ? 1 : bankNumber;
		}
		else if (addr >= 0x4000 && addr <= 0x5FFF)
		{
			if (data >= 0x00 && data <= 0x03)
			{
				ram_bank = data;
			}
			else if (data >= 0x08 && data <= 0x0C)
			{

			}
		}
	}
}

uint8_t Bus::read(uint16_t addr)
{
	if (romType == 0x13) // MBC3 + RAM + BATTERY
	{
		if (addr >= 0x4000 && addr <= 0x7FFF)
		{
			int romBankOffset = (rom_bank == 0) ? 0x4000 : (rom_bank * 0x4000);
			return rom[static_cast<int>((addr & 0x3FFF) + romBankOffset)];
		}
		else if (addr >= 0xA000 && addr <= 0xBFFF)
		{
			int ramBankOffset = (ram_bank * 0x2000);
			return rom[static_cast<int>((addr & 0x3FFF) + ramBankOffset)];
		}
		else
		{
			return ram[addr];
		}
	}
}
