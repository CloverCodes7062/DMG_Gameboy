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

Cpu::Cpu(CpuInstructions& cpuInstructions, MMU& mmu, Mbc& mbc, std::vector<uint8_t>& rom, Gpu& gpu ) 
	: cpuInstructions(cpuInstructions), mmu(mmu), mbc(mbc), rom(rom), stkp(0xFFFE), gpu(gpu)
{
	tileMapAddress = 0x9800;
	previousJoyPadState = 0x0F;
	loadBootRom();
}

Cpu::~Cpu()
{

}

void Cpu::loadBootRom()
{
	for (size_t addr = 0x0000; addr < bootRomData.size(); addr++)
	{
		write(addr, bootRomData[addr]);
	}
}

void Cpu::loadRom()
{
	for (uint16_t addr = 0x0000; addr < 0x8000; addr++)
	{
		write(addr, rom[addr]);
	}

	hasLoadedRom = true;
	mbc.hasRomLoaded = true;
}

void Cpu::write(uint16_t addr, uint8_t data)
{
	mmu.write(addr, data);
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

	return mmu.read(addr);
}

void Cpu::handleDMATransfer()
{
	mmu.handleDMATransfer();
}

void Cpu::setFrameReady(bool value)
{
	frameReady = false;
	gpu.frameReady = false;
}

bool Cpu::getHasNotBroken()
{
	return hasNotBroken;
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

		uint8_t updatedLy = gpu.update(cyclesRan, read(LY), inVblank);
		write(LY, updatedLy);
		cyclesRan = 0;

		if (read(0xFF45) == read(0xFF44))
		{
			uint8_t statValue = read(STAT);
			statValue |= 0b100;

			write(STAT, statValue);

			uint8_t IFValue = read(IF);
			IFValue |= (1 << 1);
			write(IF, IFValue);
		}
		
		if (gpu.InVblank() && !inVblank)
		{
			uint8_t IFValue = read(IF);
			IFValue |= (1 << 0);
			write(IF, IFValue);
		}
		
	}
	cycles -= 4;
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

		if (ifired & 0x02) // LCD
		{
			uint8_t IFvalue = read(IF);
			IFvalue &= ~(0x02);
			write(IF, IFvalue);

			halted = false;

			uint8_t STATvalue = read(0xFF41);
			STATvalue &= ~(0x02);
			write(0xFF41, STATvalue);

			cpuInstructions.RST48(*this);
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

void Cpu::update_joypad_memory()
{
	if (joypad_state)
	{
		mmu.joypad = joypad_state;
		joypad_state = 0;

		uint8_t IFValue = read(IF);
		IFValue |= (1 << 4);
		write(IF, IFValue);
		stopped = false;
	}
}

void Cpu::key_press(JoyPadEnums key)
{
	joypad_state = mmu.joypad & ~(0xFF & key);
}

void Cpu::key_release(JoyPadEnums key)
{
	joypad_state = mmu.joypad | (0xFF & key);
}

void Cpu::check(int last_instr_cycles)
{
	joypad_cycles += cyclesRan;

	if (stopped)
	{
		if (joypad_cycles < 65536)
		{
			return;
		}

		joypad_cycles -= 65536;
	}
}

void Cpu::incrementDivReg()
{
	mmu.incrementDivReg();
}

std::vector<uint32_t> Cpu::getFrameBuffer()
{
	return gpu.frameBuffer;
}