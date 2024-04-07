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
	//ram[0xFF00] &= ~(1 << 3); Sets Bit 3 (Select/Down)
}

Bus::~Bus()
{

}

void Bus::write(uint16_t addr, uint8_t data)
{
	if (addr == 0xFF00)
	{
		return;
	}

	if (addr == 0xFF04)
	{
		ram[addr] = 0x00;
		return;
	}

	if (addr >= 0xFEA0 && addr <= 0xFEFF)
	{
		std::cout << "ATTEMPTED TO WRITE TO UNUSABLE RAM LOCATION" << std::endl;
		return;
	}

	if (hasLoadedRom)
	{
		if (romType == 0x00)
		{
			if (addr <= 0x8000)
			{
				return;
			}
			else
			{
				ram[addr] = data;
			}
		}
		else if (romType == 0x01) // MBC1 MAPPER
		{
			if (addr >= 0x2000 && addr <= 0x3FFF)
			{
				uint8_t bankNumber = (data & 0x1F);

				if (rom.size() == (256 * 1024))
				{
					bankNumber &= 0x0F;
				}
				rom_bank = (bankNumber == 0) ? 1 : bankNumber;
			}
			else if (addr >= 0x8000)
			{
				ram[addr] = data;
			}
		}
	}
	else
	{
		ram[addr] = data;
	}
}

uint8_t Bus::read(uint16_t addr)
{
	if (!hasLoadedRom)
	{
		return ram[addr];
	}
	if (romType == 0x00)
	{
		return ram[addr];
	}
	if (romType == 0x01) // MBC1
	{
		if (addr >= 0x4000 && addr <= 0x7FFF)
		{
			int romBankOffset = (rom_bank == 0) ? 0x4000 : (rom_bank * 0x4000);
			return rom[static_cast<int>((addr & 0x3FFF) + romBankOffset)];
		}
		else
		{
			return ram[addr];
		}
	}
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

void Bus::setRom(std::vector<uint8_t> romData)
{
	rom = romData;
	romType = rom[0x0147];	
}

void Bus::printRombank()
{
	std::cout << "ROM BANK NUMBER 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(rom_bank) << std::endl;
}
