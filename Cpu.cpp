#include "Bus.h"
#include "Cpu.h"
#include "CpuInstructions.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <conio.h>
#include <fstream>
#include <sstream>

Cpu::Cpu(Bus& bus, CpuInstructions& cpuInstructions ) : bus(bus), cpuInstructions(cpuInstructions), pc(0x0000), stkp(0xFFFE)
{
	registers.af = 0x0000;
	registers.bc = 0x0000;
	registers.de = 0x0000;
	registers.hl = 0x0000;

	lcdc = 0xFF40;
	stat = 0xFF41;
	ly = 0xFF44;
	//vBlankIRQ = 0xFF85;

	ime = false;

	write(lcdc, 0x91);
	write(stat, 0x85);
	write(DIV, 0x00);
	tileMapAddress = 0x9800;

	previousJoyPadState = 0x0F;
	loadBootRom();
}

Cpu::~Cpu()
{

}

void Cpu::loadBootRom()
{
	std::vector<uint8_t> bootRomData = {
		0x31, 0xfe, 0xff, 0xaf, 0x21, 0xff, 0x9f, 0x32, 0xcb, 0x7c, 0x20, 0xfb, 0x21, 0x26, 0xff, 0x0e,
		0x11, 0x3e, 0x80, 0x32, 0xe2, 0x0c, 0x3e, 0xf3, 0xe2, 0x32, 0x3e, 0x77, 0x77, 0x3e, 0xfc, 0xe0,
		0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1a, 0xcd, 0x95, 0x00, 0xcd, 0x96, 0x00, 0x13, 0x7b,
		0xfe, 0x34, 0x20, 0xf3, 0x11, 0xd8, 0x00, 0x06, 0x08, 0x1a, 0x13, 0x22, 0x23, 0x05, 0x20, 0xf9,
		0x3e, 0x19, 0xea, 0x10, 0x99, 0x21, 0x2f, 0x99, 0x0e, 0x0c, 0x3d, 0x28, 0x08, 0x32, 0x0d, 0x20,
		0xf9, 0x2e, 0x0f, 0x18, 0xf3, 0x67, 0x3e, 0x64, 0x57, 0xe0, 0x42, 0x3e, 0x91, 0xe0, 0x40, 0x04,
		0x1e, 0x02, 0x0e, 0x0c, 0xf0, 0x44, 0xfe, 0x90, 0x20, 0xfa, 0x0d, 0x20, 0xf7, 0x1d, 0x20, 0xf2,
		0x0e, 0x13, 0x24, 0x7c, 0x1e, 0x83, 0xfe, 0x62, 0x28, 0x06, 0x1e, 0xc1, 0xfe, 0x64, 0x20, 0x06,
		0x7b, 0xe2, 0x0c, 0x3e, 0x87, 0xe2, 0xf0, 0x42, 0x90, 0xe0, 0x42, 0x15, 0x20, 0xd2, 0x05, 0x20,
		0x4f, 0x16, 0x20, 0x18, 0xcb, 0x4f, 0x06, 0x04, 0xc5, 0xcb, 0x11, 0x17, 0xc1, 0xcb, 0x11, 0x17,
		0x05, 0x20, 0xf5, 0x22, 0x23, 0x22, 0x23, 0xc9, 0xce, 0xed, 0x66, 0x66, 0xcc, 0x0d, 0x00, 0x0b,
		0x03, 0x73, 0x00, 0x83, 0x00, 0x0c, 0x00, 0x0d, 0x00, 0x08, 0x11, 0x1f, 0x88, 0x89, 0x00, 0x0e,
		0xdc, 0xcc, 0x6e, 0xe6, 0xdd, 0xdd, 0xd9, 0x99, 0xbb, 0xbb, 0x67, 0x63, 0x6e, 0x0e, 0xec, 0xcc,
		0xdd, 0xdc, 0x99, 0x9f, 0xbb, 0xb9, 0x33, 0x3e, 0x3c, 0x42, 0xb9, 0xa5, 0xb9, 0xa5, 0x42, 0x3c,
		0x21, 0x04, 0x01, 0x11, 0xa8, 0x00, 0x1a, 0x13, 0xbe, 0x20, 0xfe, 0x23, 0x7d, 0xfe, 0x34, 0x20,
		0xf5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xfb, 0x86, 0x20, 0xfe, 0x3e, 0x01, 0xe0, 0x50
	};

	for (size_t addr = 0x0000; addr < bootRomData.size(); addr++)
	{
		write(addr, bootRomData[addr]);
	}
}

void Cpu::loadRom(std::vector<uint8_t> romData = std::vector<uint8_t>())
{
	if (pc < 0x100)
	{
		rom = romData;

		for (size_t addr = 0x0104; addr <= 0x0133; addr++)
		{
			write(addr, rom[addr]);
		}

		for (size_t addr = 0x0134; addr <= 0x014D; addr++)
		{
			write(addr, rom[addr]);
		}

		bus.setRom(romData);
	}
	else
	{
		for (uint16_t addr = 0x0000; addr < 0x8000; addr++)
		{
			write(addr, rom[addr]);
		}
		hasLoadedRom = true;
		bus.hasLoadedRom = true;
	}
	setTitle();
}

void Cpu::write(uint16_t addr, uint8_t data)
{
	if (addr >= 0x8000 && addr <= 0x9FFF)
	{
		gpu.vramWrite(addr, data);
	}

	bus.write(addr, data);

	if (addr == 0xFF46)
	{
		handleDMATransfer();
	}
}

uint8_t Cpu::read(uint16_t addr)
{
	if (addr >= 0x8000 && addr <= 0x9FFF)
	{
		return gpu.vram[addr];
	}

	return bus.read(addr);
}

void Cpu::handleDMATransfer()
{
	uint8_t sourceOffset = read(0xFF46);

	uint16_t sourceAddr = (sourceOffset << 8) | 0x00;

	for (size_t addr = 0xFE00; addr <= 0xFE9F; addr++)
	{
		gpu.vramWrite(addr, read(sourceAddr++));
	}

	gpu.updateSprites(read(lcdc));
}

std::vector<std::vector<uint16_t>> Cpu::getBackgroundTiles()
{
	return gpu.backgroundTiles;
}

std::vector<Sprite> Cpu::getSprites()
{
	return gpu.Sprites;
}

std::vector<std::vector<uint16_t>> Cpu::getTileSet()
{
	return gpu.tileSet;
}

uint8_t Cpu::getSCY()
{
	return read(SCY);
}

uint8_t Cpu::getSCX()
{
	return read(SCX);
}

bool Cpu::getIs8x16Mode()
{
	return gpu.is8x16Mode;
}

void Cpu::setFrameReady(bool value)
{
	frameReady = false;
	gpu.frameReady = false;
}

void Cpu::clearGpuBackgroundTiles()
{
	gpu.backgroundTiles.clear();
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
	return (registers.f & (1 << flag)) != 0;
}

void Cpu::runInstruction()
{
	if (cycles == 0)
	{	
		// HANDLES INTERRUPTS
		handleInterrupts();

		if (pc == 0x100 && !hasLoadedRom)
		{
			loadRom();
			return;
		}

		
		if (hasLoadedRom)
		{
			//handlePCTrace();
		}

		frameReady = gpu.frameReady;

		// RUN INSTRUCTION IF NOT HALTED
		if (!halted)
		{
			if (!wasCB)
			{
				cpuInstructions.runInstruction(*this, read(0xFF00), read(IE) && read(IF));
			}
			if (wasCB)
			{
				cpuInstructions.runCBInstruction(*this);
				wasCB = false;
			}
		}
		else 
		{
			//std::cout << "CPU HALTED; WAITING FOR INTERRUPT" << std::endl;
			cycles += 4;
			cyclesRan += 4;
		}
		//

		uint8_t updatedLy = gpu.update(cyclesRan, read(ly), inVblank, read(lcdc), read(PALETTE));
		write(ly, updatedLy);
		cyclesRan = 0;
		
		if (gpu.InVblank() && !inVblank)
		{
			uint8_t IFValue = read(IF);
			IFValue |= (1 << 0);
			write(IF, IFValue);
		}
		
	}
	cycles -= 4;
}

void Cpu::writeToJoyPad(uint8_t data)
{
	bus.writeToJoyPad(data);

	uint8_t IFValue = read(IF);
	IFValue |= (1 << 4);
	write(IF, IFValue);
	stopped = false;
}

void Cpu::handleInterrupts()
{
	if (ime && read(IE) && read(IF) && !wasCB)
	{
		uint8_t ifired = read(IE) & read(IF);

		if (ifired & 0x01) // VBLANK
		{
			uint8_t IFvalue = read(IF);
			IFvalue &= (255 - 0x01);
			write(IF, IFvalue);

			gpu.setVblank(false);

			halted = false;
			cpuInstructions.RST40(*this);
		}

		
		if (ifired & 0x10) // JOYPAD
		{
			uint8_t IFvalue = read(IF);
			IFvalue &= (255 - 0x10);
			write(IF, IFvalue);

			halted = false;

			cpuInstructions.RST60(*this);
		}
		
	}
}

void Cpu::setRomType()
{
	romType = rom[0x0147];

	//std::cout << "ROM TYPE 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(romType) << std::endl;
}

void Cpu::handlePCTrace()
{
	std::stringstream ss;
	if (wasCB)
	{
		ss << "pc: 0x" << std::hex << std::setw(4) << std::setfill('0') << pc << ", opcode: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(read(pc)) << ", instruction: " << "NOT IMPL YET";
	}
	else
	{
		ss << "pc: 0x" << std::hex << std::setw(4) << std::setfill('0') << pc << ", opcode: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(read(pc)) << ", instruction: " << "NOT IMPL YET";
	}
	std::string result = ss.str();
	pcDeque.push_back(result);

	while (pcDeque.size() > 2000)
	{
		pcDeque.pop_front();
	}
}

void Cpu::incrementDivReg()
{
	bus.incrementDivReg();
}

void Cpu::handleDebugStepMode()
{
	if (stepMode && !inCALL)
	{
		std::cout << "Press spacebar to execute the next instruction or 'r' to run to breakpoint..." << std::endl;
		char key;
		printStatus();
		while (true) 
		{
			if (_kbhit()) 
			{
				key = _getch();
				if (key == ' ') 
				{
					break;
				}
				else if (key == 'r') 
				{
					stepMode = false;
					break;
				}
				else if (key == 's')
				{
					setHasNotBroken(false);
					return;
				}
			}
		}
	}
	if (_kbhit())
	{
		char keyPressed = _getch();
		if (keyPressed == 's')
		{
			setHasNotBroken(false);
			return;
		}
	}
}

void Cpu::printVram()
{

	for (uint16_t addr = 0x0000; addr <= 0x7FFF; addr++)
	{
		std::cout << "0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(read(addr)) << std::endl;
	}
	std::cout << "0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(bus.getRamSize()) << std::endl;
}

void Cpu::printRombank()
{
	bus.printRombank();
}

void Cpu::printStatus()
{
	std::cout << "Registers:" << std::endl;

	std::cout << "AF: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(registers.af);
	std::cout << " [ Z: " << isFlagSet(Zero) << ", N: " << isFlagSet(Subtraction) << ", H: " << isFlagSet(HalfCarry) << ", C: " << isFlagSet(Carry) << " ]" << std::endl;
	std::cout << "BC: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(registers.bc) << std::endl;
	std::cout << "DE: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(registers.de) << std::endl;
	std::cout << "HL: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(registers.hl) << std::endl;
	std::cout << "PC: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(pc) << std::endl;
	std::cout << "STKP: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(stkp) << std::endl << std::endl;

	std::cout << "LCDC: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(read(lcdc)) << std::endl;
	std::cout << "STAT: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(read(stat)) << std::endl;
	std::cout << "LY: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(read(ly)) << std::endl << std::endl;

	std::cout << "IME: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ime) << std::endl;
	std::cout << "IE: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(read(IE)) << std::endl;
	std::cout << "IF: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(read(IF)) << std::endl;

	std::cout << "INSTRUCTION: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(read(pc)) << std::endl;

}

void Cpu::printTrace()
{
	for (size_t addr = 0; addr < pcDeque.size(); addr++)
	{
		std::cout << pcDeque[addr] << std::endl;
	}
}

void Cpu::setTitle()
{
	std::string title;
	for (uint16_t addr = 0x0134; addr <= 0x0143; ++addr) {
		title += static_cast<char>(read(addr));
	}

	romTitle = title;

	return;
}

std::string Cpu::getTitle()
{
	return romTitle;
}