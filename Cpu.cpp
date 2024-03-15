#include "Bus.h"
#include "Cpu.h"
#include <iostream>
#include <iomanip>

Cpu::Cpu(Bus& bus) : bus(bus), pc(0x100), stkp(0xFFFE)
{
	registers.af = 0x0100;
	registers.bc = 0x0014;
	registers.de = 0x0000;
	registers.hl = 0xC060;
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

void Cpu::setFlag(Cpu::Flags flag, bool value)
{
	if (value) {
		registers.f |= (1 << flag);
	}
	else {
		registers.f &= ~(1 << flag);
	}
}

bool Cpu::isFlagSet(Cpu::Flags flag) const
{
	return (registers.f & (1 << flag)) != 0;;
}

void Cpu::runInstruction()
{
	if (cycles == 0)
	{
		runOpcode();
	}
	cycles--;
}

void Cpu::runOpcode()
{
	uint8_t instruction = rom[pc];
	pc++;

	std::cout << "Registers:" << std::endl;
	std::cout << "AF: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(registers.af) << std::endl;
	std::cout << "BC: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(registers.bc) << std::endl;
	std::cout << "DE: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(registers.de) << std::endl;
	std::cout << "HL: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(registers.hl) << std::endl;
	std::cout << "PC: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(pc) << std::endl;
	std::cout << "SP: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(stkp) << std::endl;

	std::cout << "Opcode: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(instruction) << std::endl;
	
	if (!instruction)
	{
		std::cout << "NOP READ" << std::endl;
		cycles += 4;
	}
	else
	{

		switch (instruction)
		{
		case 0xC3: // Jump to address hi | lo, only if zero flag is not set
		{
			uint8_t lo = rom[pc];
			uint8_t hi = rom[pc + 1];
			uint16_t address = (hi << 8) | lo;
			if (!isFlagSet(Zero))
			{
				pc = address;
				cycles += 16;
			}
			else 
			{
				pc += 2;
				cycles += 12;
			}

			break;
		}
		case 0xFE:
		{
			uint8_t compare_value = rom[pc];
			uint8_t reg_a_value = registers.a;

			uint8_t result = reg_a_value - compare_value;

			setFlag(Zero, result == 0x00);
			setFlag(Subtraction, true);
			setFlag(HalfCarry, (reg_a_value & 0x0F) < (compare_value & 0x0F));
			setFlag(Carry, reg_a_value < compare_value);

			pc ++;
			cycles += 8;

			break;
		}
		case 0x28:
		{
			int8_t offset = static_cast<int8_t>(rom[pc]);
			if (isFlagSet(Zero))
			{
				pc += offset + 1;
				cycles += 12;
			}
			else
			{
				pc++;
				cycles += 8;
			}

			break;
		}
		default:
			std::cout << "Unknown instruction 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(instruction) << " found" << std::endl;
			setHasNotBroken(false);
			break;
		}
	}
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