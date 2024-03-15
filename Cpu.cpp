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
