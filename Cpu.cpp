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
	IE = 0xFFFF;
	IF = 0xFF0F;

	write(lcdc, 0x91);
	write(stat, 0x85);
	tileMapAddress = 0x9800;

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
	}
	else
	{
		for (uint16_t addr = 0x0000; addr < rom.size(); addr++)
		{
			write(addr, rom[addr]);
		}
		hasLoadedRom = true;
	}
	setTitle();
}

void Cpu::loadNintendoLogo()
{
	// Nintendo Logo in Hex
	const std::string nintendoLogoHex = "00000000000000000000000000000000F000F000FC00FC00FC00FC00F300F3003C003C003C003C003C003C003C003C00F000F000F000F00000000000F300F300000000000000000000000000CF00CF00000000000F000F003F003F000F000F000000000000000000C000C0000F000F00000000000000000000000000F000F000000000000000000000000000F300F300000000000000000000000000C000C000030003000300030003000300FF00FF00C000C000C000C000C000C000C300C300000000000000000000000000FC00FC00F300F300F000F000F000F000F000F0003C003C00FC00FC00FC00FC003C003C00F300F300F300F300F300F300F300F300F300F300C300C300C300C300C300C300CF00CF00CF00CF00CF00CF00CF00CF003C003C003F003F003C003C000F000F003C003C00FC00FC0000000000FC00FC00FC00FC00F000F000F000F000F000F000F300F300F300F300F300F300F000F000C300C300C300C300C300C300FF00FF00CF00CF00CF00CF00CF00CF00C300C3000F000F000F000F000F000F00FC00FC003C004200B900A500B900A50042003C000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";

	std::vector<uint8_t> nintendoLogoData;
	for (size_t i = 0; i < nintendoLogoHex.length(); i += 2)
	{
		std::string byteString = nintendoLogoHex.substr(i, 2);
		uint8_t byte = std::stoi(byteString, nullptr, 16);
		nintendoLogoData.push_back(byte);
	}

	// Loads into RAM
	uint16_t vramAddress = 0x8000;
	for (size_t i = 0; i < nintendoLogoData.size(); ++i)
	{
		write(vramAddress++, nintendoLogoData[i]);
	}

	// Nintendo Logo TileMap
	const std::string nintendoTileMap = "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000102030405060708090A0B0C19000000000000000000000000000000000000000D0E0F101112131415161718000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";

	// Creates and Loads Tiles into RAM
	setVramTiles();

	std::vector<uint8_t> nintendoLogoTileMap;
	for (size_t i = 0; i < nintendoTileMap.length(); i += 2)
	{
		std::string byteString = nintendoTileMap.substr(i, 2);
		uint8_t byte = std::stoi(byteString, nullptr, 16);
		nintendoLogoTileMap.push_back(byte);
	}

	for (size_t i = 0; i < nintendoLogoTileMap.size(); ++i)
	{
		write(tileMapAddress++, nintendoLogoTileMap[i]);
	}

	tileMapAddress = 0x9800;

	std::cout << "VRAM TILES SIZE: " << vramTiles.size() << std::endl;
}

void Cpu::setVramTiles()
{
	std::vector<std::vector<std::vector<uint8_t>>> tiles;
	std::vector<std::vector<uint8_t>> tile;

	for (size_t i = 0x8000; i <= 0x8FFF; i += 2)
	{
		std::vector<uint8_t> tileRow = createTileRow(read(i), read(i + 1));

		tile.push_back(tileRow);

		if (tile.size() == 8)
		{
			tiles.push_back(tile);
			tile.clear();
		}
	}

	vramTiles = tiles;
}

void Cpu::write(uint16_t addr, uint8_t data)
{
	/*
	if (addr >= 0x0000 && addr <= 0x3FFF)
	{
		std::cout << "WROTE TO ROM BANK 00" << std::endl;
	}

	if (addr >= 0x4000 && addr <= 0x7FFF)
	{
		std::cout << "WROTE TO ROM BANK 01-NN" << std::endl;
	}

	if (addr >= 0x8000 && addr <= 0x97FF)
	{
		std::cout << "WROTE TO TILE RAME" << std::endl;
	}

	if (addr >= 0xC000 && addr <= 0xCFFF)
	{
		std::cout << "WROTE TO WORK RAM 0" << std::endl;
	}

	if (addr >= 0xD000 && addr <= 0xDFFF)
	{
		std::cout << "WROTE TO WORK RAM 1" << std::endl;
	}

	if (addr >= 0xFF80 && addr <= 0xFFFE)
	{
		std::cout << "WROTE TO HIGH RAM" << std::endl;
	}
	*/

	if ((stepMode) && (addr >= 0xFF80) && (addr <= 0xFFFE))
	{
		std::cout << "WROTE TO HIGH RAM" << std::endl;
	}

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
	return (registers.f & (1 << flag)) != 0;
}

void Cpu::runInstruction()
{
	if (cycles == 0)
	{	
		if (pc == 0x100 && !hasLoadedRom)
		{
			loadRom();
			stepMode = true;
		}
		// FOR BLARRGS CPU TEST OUTPUTS
		handleSerialPortOutput();

		// HANDLE DEBUG STEP MODE
		handleDebugStepMode();

		// PROGRAM COUTER TRACE
		handlePCTrace();

		// HANDLES INTERRUPTS
		handleInterrupts();

		// RUN INSTRUCTION IF NOT HALTED
		if (!halted)
		{
			if (!wasCB)
			{
				cpuInstructions.runInstruction(*this);
			}
			else
			{
				cpuInstructions.runCBInstruction(*this);
				wasCB = false;
			}
		}
		else 
		{
			std::cout << "CPU HALTED; WAITING FOR INTERRUPT" << std::endl;
			cycles += 4;
			cyclesRan += 4;
		}
		//

		
		if (cyclesRan >= 456)
		{
			write(ly, read(ly) + 1);
			cyclesRan = 0;

			if (read(ly) % 8 == 0)
			{
				setVramTiles();

				for (size_t addr = tileMapAddress; addr < tileMapAddress + 32; ++addr)
				{
					//std::cout << "VRAM TILES SIZE: " << vramTiles.size() << std::endl;
					//std::cout << "ADDR: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(addr) << std::endl;
					backgroundTiles.push_back(vramTiles[read(addr)]);
				}
				tileMapAddress += 32;

				//std::cout << "SCANLINE GENERATED BG TILES SIZE: 0x" << std::hex << std::setw(2) << std::setfill('0') << backgroundTiles.size() << std::endl;

				scanlinesGenerated++;

				//std::cout << "TILE MAP ADDRESS: " << tileMapAddress << std::endl;

			}

			if (read(ly) >= 0x99 && !inVblank)
			{
				
				//std::cout << "FRAME GENERATED BG TILES SIZE: 0x" << std::hex << std::setw(2) << std::setfill('0') << backgroundTiles.size() << std::endl;
				totalFramesGenerated++;

				//std::cout << "VRAM TILES SIZE: " << vramTiles.size() << std::endl;

				std::cout << "TOTAL FRAMES GENERATED: " << totalFramesGenerated << std::endl;

				
				engine.setBuffer(backgroundTiles);

				bool running = true;
				auto startTime = std::chrono::steady_clock::now();

				while (running) {

					auto currentTime = std::chrono::steady_clock::now();
					auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
					if (elapsedTime >= 1000 / 60) {
						running = false;
					}

					SDL_Event event;
					while (SDL_PollEvent(&event)) {
						if (event.type == SDL_QUIT) {
							running = false;
						}
					}

					engine.render();
				}
				
				tileMapAddress = 0x9800;
				backgroundTiles.clear();

				// SETS VBLANK IN INTERRUPT FLAG
				uint8_t IFValue = read(IF);
				IFValue |= (1 << 0);
				write(IF, IFValue);
			}
		}
		
	}
	cycles -= 4;
}


void Cpu::handlePCTrace()
{
	std::stringstream ss;
	if (wasCB)
	{
		ss << "pc: 0x" << std::hex << std::setw(4) << std::setfill('0') << pc << ", opcode: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(read(pc)) << ", instruction: " << cpuInstructions.getCBInstructionName(*this, pc);
	}
	else
	{
		ss << "pc: 0x" << std::hex << std::setw(4) << std::setfill('0') << pc << ", opcode: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(read(pc)) << ", instruction: " << cpuInstructions.getInstructionName(*this, pc);
	}
	std::string result = ss.str();
	pcDeque.push_back(result);

	while (pcDeque.size() > 2000)
	{
		pcDeque.pop_front();
	}
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

			halted = false;
			cpuInstructions.RST40(*this);
		}
	}
}

void Cpu::handleSerialPortOutput()
{
	if (read(0xFF01) != previousFF01)
	{
		FF01Changes.push_back(read(0xFF01));
		previousFF01 = read(0xFF01);
	}
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

}

void Cpu::printTrace()
{
	for (size_t addr = 0; addr < pcDeque.size(); addr++)
	{
		std::cout << pcDeque[addr] << std::endl;
	}
}

void Cpu::printSerialPorts()
{
	std::cout << "0xFF02: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(read(0xFF02)) << std::endl << std::endl;
	
	for (size_t addr = 0; addr < FF01Changes.size(); addr++)
	{
		std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(FF01Changes[addr]) << " ";
	}

	std::cout << std::endl;
	std::cout << std::endl;

	for (size_t addr = 0x9800; addr <= 0x9BFF; addr++)
	{
		std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(read(addr)) << " ";
	}
}

void Cpu::writeStateToLog() {
	std::ofstream logfile("C:\\Users\\Stacy\\Desktop\\cppProjects\\DMG_Gameboy_TestRoms\\cpu_log.txt", std::ios_base::app); // Append mode

	if (logfile.is_open()) {
		logfile << "A:" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(registers.a)
			<< " F:" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(registers.f)
			<< " B:" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(registers.b)
			<< " C:" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(registers.c)
			<< " D:" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(registers.d)
			<< " E:" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(registers.e)
			<< " H:" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(registers.h)
			<< " L:" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(registers.l)
			<< " SP:" << std::hex << std::setw(4) << std::setfill('0') << stkp
			<< " PC:" << std::hex << std::setw(4) << std::setfill('0') << pc
			<< " PCMEM:";

		for (uint16_t addr = pc; addr <= pc + 3; addr++) 
		{
			logfile << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(read(addr)) << ",";
		}

		logfile << std::endl;

		logfile.close();
	}
	else {
		std::cerr << "Error: Unable to open logfile for writing." << std::endl;
	}
}

std::vector<uint8_t> Cpu::createTileRow(uint8_t lsb, uint8_t msb)
{
	std::vector<uint8_t> tileRow;

	for (int i = 7; i >= 0; i--)
	{
		uint8_t lsbBit = (lsb >> i) & 0x01;
		uint8_t msbBit = (msb >> i) & 0x01;

		uint8_t color = (msbBit << 1) | lsbBit;

		tileRow.push_back(color);
	}

	return tileRow;

}

void Cpu::setTitle()
{
	std::string title;
	for (uint16_t addr = 0x0134; addr <= 0x0143; ++addr) {
		title += static_cast<char>(read(addr));
	}

	romTitle = title;
}

std::string Cpu::getTitle()
{
	return romTitle;
}

/* PLACEHOLDER TO TEST FUNCTIONS
	std::cout << "Registers:" << std::endl;

	std::cout << "AF: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(registers.af);
	std::cout << " [ Z: " << isFlagSet(Zero) << ", N: " << isFlagSet(Subtraction) << ", H: " << isFlagSet(HalfCarry) << ", C: " << isFlagSet(Carry) << " ]" << std::endl;
	std::cout << "BC: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(registers.bc) << std::endl;
	std::cout << "DE: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(registers.de) << std::endl;
	std::cout << "HL: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(registers.hl) << std::endl;
	std::cout << "PC: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(pc) << std::endl;
	std::cout << "SP: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(stkp) << std::endl << std::endl;

	std::cout << "LCDC: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(read(lcdc)) << std::endl;
	std::cout << "STAT: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(read(stat)) << std::endl;
	std::cout << "LY: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(read(ly)) << std::endl << std::endl;

	std::cout << "Opcode: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(read(pc)) << std::endl;

	setHasNotBroken(false);
*/