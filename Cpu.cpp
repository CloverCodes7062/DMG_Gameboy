#include "Bus.h"
#include "Cpu.h"

Cpu::Cpu(Bus& bus) : bus(bus)
{

}

Cpu::~Cpu()
{

}

void Cpu::write(uint16_t addr, uint8_t data)
{
	bus.write(addr, data);
}

uint8_t Cpu::read(uint16_t addr)
{
	return bus.read(addr);
}

bool Cpu::getHasNotBroken()
{
	return hasNotBroken;
}

uint16_t Cpu::getPc()
{
	return pc;
}

void Cpu::setHasNotBroken(bool value)
{
	hasNotBroken = value;
}

void Cpu::runInstruction(uint8_t instruction)
{

}

void Cpu::loadRom(std::vector<uint8_t> romData)
{
	rom = romData;
	setTitle();
}

void Cpu::setTitle()
{
	std::string title;
	for (uint16_t addr = 0x0134; addr <= 0x0143; ++addr) {
		title += static_cast<char>(rom[addr]);
	}

	romTitle = title;
}

std::string Cpu::getTitle()
{
	return romTitle;
}