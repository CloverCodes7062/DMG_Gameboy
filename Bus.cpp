#include "Bus.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <conio.h>

Bus::Bus() : ram(0x10000, 0)
{
}

Bus::~Bus()
{

}

void Bus::write(uint16_t addr, uint8_t data)
{
	//std::cout << "Writing to RAM: " << std::hex << std::setw(4) << std::setfill('0') << addr << std::endl;
	//std::cout << "Data: " << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data) << std::endl;

	ram[addr] = data;
}

uint8_t Bus::read(uint16_t addr)
{
	return ram[addr];
}

uint32_t Bus::getRamSize()
{
	return ram.size();
}
