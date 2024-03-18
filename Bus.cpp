#include "Bus.h"
#include <fstream>
#include <iostream>

Bus::Bus() : ram(0x10000, 0)
{

}

Bus::~Bus()
{

}

void Bus::write(uint16_t addr, uint8_t data)
{
	std::cout << "Writing to ram" << std::endl;

	ram[addr] = data;
}

uint8_t Bus::read(uint16_t addr)
{
	return ram[addr];
}
