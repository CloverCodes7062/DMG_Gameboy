#include "Bus.h"
#include "Cpu.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>

Cpu::Cpu(Bus& bus) : bus(bus), pc(0x100), stkp(0xFFFE)
{
	registers.af = 0x01B0;
	registers.bc = 0x0013;
	registers.de = 0x00D8;
	registers.hl = 0x014D;

	lcdc = 0xFF40;
	stat = 0xFF41;
	ly = 0xFF44;

	ie = 0xFFFF;

	write(lcdc, 0x91);
	write(stat, 0x85);

	tileMapAddress = 0x9800;

	loadNintendoLogo();

	using a = Cpu;
	lookup = 
	{

	}
}

Cpu::~Cpu()
{

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
	/* GENERATES NINTENDO LOGO AND RENDERS IT ON BACKGROUND
	std::vector<std::vector<std::vector<uint8_t>>> BgTiles;

	for (size_t addr = 0x9800; addr <= 0x9BFF; addr++)
	{
		BgTiles.push_back(vramTiles[read(addr)]);
	}

	backgroundTiles = BgTiles;

	std::cout << "TILES GENERATED" << std::endl;
	
	engine.setBuffer(backgroundTiles);

	bool running = true;

	while (running) {

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
		}

		engine.render();
	}
	*/
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
	//printVram();

	if (cycles == 0)
	{
		runOpcode();

		//std::cout << "CYCLES RAN: " << cyclesRan << std::endl;

		// LY REG CURRENT IMPL
		/*
		if (cyclesRan >= 456)
		{
			std::cout << "INCREMENTING LY" << std::endl;

			std::cout << "LY: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(read(ly)) << std::endl << std::endl;

			write(ly, read(ly) + 1);

			std::cout << "LY: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(read(ly)) << std::endl << std::endl;
			cyclesRan = 0;

			if (read(ly) % 8 == 0)
			{
				setVramTiles();

				for (size_t addr = tileMapAddress; addr < tileMapAddress + 32; ++addr)
				{
					std::cout << "VRAM TILES SIZE: " << vramTiles.size() << std::endl;
					std::cout << "ADDR: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(addr) << std::endl;
					backgroundTiles.push_back(vramTiles[read(addr)]);
				}
				tileMapAddress += 32;

				std::cout << "SCANLINE GENERATED BG TILES SIZE: 0x" << std::hex << std::setw(2) << std::setfill('0') << backgroundTiles.size() << std::endl;

				scanlinesGenerated++;

				std::cout << "TILE MAP ADDRESS: " << tileMapAddress << std::endl;

			}

			if (read(ly) >= 0x90)
			{
				std::cout << "FRAME GENERATED BG TILES SIZE: 0x" << std::hex << std::setw(2) << std::setfill('0') << backgroundTiles.size() << std::endl;
				totalFramesGenerated++;

				std::cout << "VRAM TILES SIZE: " << vramTiles.size() << std::endl;

				std::cout << "TOTAL FRAMES GENERATED: " << totalFramesGenerated << std::endl;

				engine.setBuffer(backgroundTiles);

				bool running = true;
				auto startTime = std::chrono::steady_clock::now();

				while (running) {

					auto currentTime = std::chrono::steady_clock::now();
					auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
					if (elapsedTime >= 3) {
						running = false;
					}

					SDL_Event event;
					while (SDL_PollEvent(&event)) {
						if (event.type == SDL_QUIT) {
							running = false;
						}
					}

					engine.render();

					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}

				// HANDLE VBLANK
				write(ly, 0x00);
				setFlag(Zero, true);
				backgroundTiles.clear();
				tileMapAddress = 0x9800;
			}
		}
		*/
		
	}
	cycles--;
}

void Cpu::printVram()
{

	for (uint16_t addr = 0x0000; addr <= 0x7FFF; addr++)
	{
		std::cout << "0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(read(addr)) << std::endl;
	}
	std::cout << "0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(bus.getRamSize()) << std::endl;
}


void Cpu::runOpcode()
{
	if (read(0xFF01) != 0x00)
	{
		std::cout << "RESULT FROM BLARGG TEST: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(read(0xFF01)) << std::endl;
		setHasNotBroken(false);
	}

	if (read(0xFF02) != 0x00)
	{
		std::cout << "RESULT FROM BLARGG TEST: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(read(0xFF01)) << std::endl;
		setHasNotBroken(false);
	}

	uint8_t instruction = read(pc);
	
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

	std::cout << "Opcode: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(instruction) << std::endl << std::endl;

	pc++;
	
	if (!instruction)
	{
		std::cout << "NOP READ" << std::endl;
		cycles += 4;
		cyclesRan += 4;
	}
	else
	{
		if (!wasCB)
		{
			std::cout << "Reading From Regular Table" << std::endl;

			switch (instruction)
			{
			case 0xC3: // Jump to address hi | lo
			{
				uint8_t lo = read(pc);
				std::cout << "read(pc): 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(read(pc)) << std::endl << std::endl;
				pc++;
				uint8_t hi = read(pc);
				std::cout << "read(pc): 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(read(pc)) << std::endl << std::endl;
				pc++;
				uint16_t address = (hi << 8) | lo;

				pc = address;
				cycles += 16;
				cyclesRan += 16;

				break;
			}
			case 0xFE:
			{
				uint8_t compare_value = read(pc);
				uint8_t reg_a_value = registers.a;

				uint8_t result = reg_a_value - compare_value;

				setFlag(Zero, result == 0x00);
				setFlag(Subtraction, true);
				setFlag(HalfCarry, (reg_a_value & 0x0F) < (compare_value & 0x0F));
				setFlag(Carry, reg_a_value < compare_value);

				pc++;
				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0x28:
			{
				int8_t offset = static_cast<int8_t>(read(pc));
				if (isFlagSet(Zero))
				{
					pc += offset + 1;
					cycles += 12;
					cyclesRan += 12;
				}
				else
				{
					pc++;
					cycles += 8;
					cyclesRan += 8;
				}

				break;
			}
			case 0x2F:
			{
				registers.a = ~registers.a;

				setFlag(Subtraction, true);
				setFlag(HalfCarry, true);

				cycles += 4;
				cyclesRan += 4;

				break;
			}
			case 0xAF:
			{
				uint8_t result = registers.a ^ registers.a;
				registers.a = result;

				setFlag(Zero, result == 0x00);
				setFlag(Subtraction, false);
				setFlag(HalfCarry, false);
				setFlag(Carry, false);

				cycles += 4;
				cyclesRan += 4;
				break;
			}
			case 0xA1:
			{
				uint8_t result = registers.a & registers.c;
				registers.a = result;

				setFlag(Zero, result == 0x00);
				setFlag(Subtraction, false);
				setFlag(HalfCarry, true);
				setFlag(Carry, false);

				cycles += 4;
				cyclesRan += 4;
				break;
			}
			case 0xA9:
			{
				uint8_t result = registers.a ^ registers.c;
				registers.a = result;

				setFlag(Zero, result == 0x00);
				setFlag(Subtraction, false);
				setFlag(HalfCarry, false);
				setFlag(Carry, false);

				cycles += 4;
				cyclesRan += 4;
				break;
			}
			case 0x18:
			{
				int8_t offset = static_cast<int8_t>(read(pc));
				pc += offset + 1;
				cycles += 12;
				cyclesRan += 12;
				break;
			}
			case 0xEA:
			{
				uint8_t lo = read(pc);
				uint8_t hi = read(pc + 1);
				uint16_t address = (hi << 8) | lo;

				write(address, registers.a);

				pc += 2;
				cycles += 16;
				cyclesRan += 16;

				break;
			}
			case 0xF3:
			{
				write(ie, 0); // TODO: HANDLE INTERRUPTS
				cycles += 4;
				cyclesRan += 4;

				break;
			}
			case 0xFB:
			{
				write(ie, 1); // TODO: HANDLE INTERRUPTS
				cycles += 4;
				cyclesRan += 4;

				break;
			}
			case 0xE0:
			{
				uint16_t address = (0xFF << 8) | read(pc);
				write(address, registers.a);
				pc++;
				cycles += 12;
				cyclesRan += 12;

				break;
			}
			case 0x3E:
			{
				registers.a = read(pc);
				pc++;
				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0xCD:
			{
				uint8_t lo = read(pc);
				pc++;
				uint8_t hi = read(pc);
				pc++;
				uint16_t address = (hi << 8) | lo;

				stkp--;
				write(stkp, (pc >> 8) & 0xFF);
				stkp--;
				write(stkp, pc & 0xFF);

				pc = address;
				cycles += 24;
				cyclesRan += 24;

				std::cout << pc << std::endl;
				break;
			}
			case 0xE6:
			{
				uint8_t n = read(pc);
				uint8_t result = registers.a & n;
				registers.a = result;

				setFlag(Zero, result == 0);
				setFlag(Subtraction, false);
				setFlag(HalfCarry, true);
				setFlag(Carry, false);

				pc++;
				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0xF0:
			{
				uint16_t address = 0xFF00 + read(pc);
				pc++;

				uint8_t n = read(address);

				registers.a = n;
				cycles += 12;
				cyclesRan += 12;

				break;
			}
			case 0x47:
			{
				registers.b = registers.a;

				cycles += 4;
				cyclesRan += 4;
				break;
			}
			case 0x4F:
			{
				registers.c = registers.a;

				cycles += 4;
				cyclesRan += 4;
				break;
			}
			case 0xCB:
			{
				cycles += 4;
				cyclesRan += 4;
				wasCB = true;
				break;
			}
			case 0x01:
			{
				uint8_t lo = read(pc);
				pc++;
				uint8_t hi = read(pc);
				pc++;
				uint16_t value = (hi << 8) | lo;
				registers.bc = value;

				cycles += 12;
				cyclesRan += 12;

				break;
			}
			case 0x0B:
			{
				registers.bc--;

				cycles += 8;
				cyclesRan += 8;

				break;
			}

			case 0x21:
			{
				uint8_t lo = read(pc);
				uint8_t hi = read(pc + 1);
				
				registers.hl = (hi << 8) | lo;

				pc += 2;
				cycles += 12;
				cyclesRan += 12;
				break;
			}
			case 0x0E:
			{
				registers.c = read(pc);

				pc++;
				cycles += 8;
				cyclesRan += 12;

				break;
			}
			case 0x06:
			{
				registers.b = read(pc);

				pc++;
				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0x32:
			{
				write(registers.hl, registers.a);

				registers.hl -= 1;
				cycles += 8;
				cyclesRan += 8;
				break;
			}
			case 0x05:
			{
				registers.b--;

				setFlag(Zero, registers.b == 0x00);
				setFlag(Subtraction, true);
				setFlag(HalfCarry, (registers.b & 0x0F) == 0x0F);

				cycles += 4;
				cyclesRan += 4;

				break;
			}
			case 0x20:
			{
				int8_t offset = static_cast<int8_t>(read(pc));
				pc++;

				if (!isFlagSet(Zero))
				{
					pc += offset;
					cycles += 12;
					cyclesRan += 12;
				}
				else
				{
					cycles += 8;
					cyclesRan += 8;
				}

				break;
			}
			case 0x0D:
			{
				registers.c--;

				setFlag(Zero, registers.c == 0x00);
				setFlag(Subtraction, true);
				setFlag(HalfCarry, (registers.c & 0x0F) == 0x0F);

				cycles += 4;
				cyclesRan += 4;
				break;
			}
			case 0x31:
			{
				uint8_t lo = read(pc);
				uint8_t hi = read(pc + 1);
				pc += 2;

				stkp = (hi << 8) | lo;

				cycles += 12;
				cyclesRan += 12;

				break;
			}
			case 0x36:
			{
				write(registers.hl, read(pc));
				pc++;

				cycles += 12;
				cyclesRan += 12;
				break;
			}
			case 0x2A:
			{
				registers.a = read(registers.hl);
				registers.hl++;

				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0xE2:
			{
				uint8_t lo = registers.c;
				uint8_t hi = 0xFF;
				
				uint16_t addr = (hi << 8) | lo;

				write(addr, registers.a);

				cycles += 8;
				pc++;

				break;
			}
			case 0x78:
			{
				registers.a = registers.b;

				cycles += 4;
				cyclesRan += 4;
				break;
			}
			case 0x79:
			{
				registers.a = registers.c;

				cycles += 4;
				cyclesRan += 4;
				break;
			}
			case 0x5F:
			{
				registers.e = registers.a;

				cycles += 4;
				cyclesRan += 4;
				break;
			}
			case 0xB0:
			{
				uint8_t result = registers.a | registers.b;
				registers.a = result;

				setFlag(Zero, result == 0x00);
				setFlag(Subtraction, false);
				setFlag(HalfCarry, false);
				setFlag(Carry, false);

				cycles += 4;
				cyclesRan += 4;

				break;
			}
			case 0xB1:
			{
				uint8_t result = registers.a | registers.c;
				registers.a = result;

				setFlag(Zero, result == 0x00);
				setFlag(Subtraction, false);
				setFlag(HalfCarry, false);
				setFlag(Carry, false);

				cycles += 4;
				cyclesRan += 4;

				break;
			}
			case 0xB7:
			{
				uint8_t result = registers.a | registers.a;
				registers.a = result;

				setFlag(Zero, result == 0x00);
				setFlag(Subtraction, false);
				setFlag(HalfCarry, false);
				setFlag(Carry, false);

				cycles += 4;
				cyclesRan += 4;

				break;
			}
			case 0xC9:
			{
				uint8_t lsb = read(stkp);
				stkp++;
				uint8_t msb = read(stkp);
				stkp++;

				uint16_t result = (msb << 8) | lsb;

				pc = result;

				cycles += 16;
				cyclesRan += 16;

				break;
			}
			case 0xEF:
			{
				stkp--;
				write(stkp, (pc >> 8) & 0xFF);
				stkp--;
				write(stkp, pc & 0xFF);

				pc = 0x28;

				cycles += 16;
				cyclesRan += 16;

				break;
			}
			case 0x87:
			{
				uint8_t result = registers.a + registers.a;
				registers.a = result;

				setFlag(Zero, result == 0);
				setFlag(Subtraction, false);
				setFlag(HalfCarry, (registers.a & 0x0F) + (registers.a & 0x0F) > 0x0F);
				setFlag(Carry, result < registers.a);

				cycles += 4;
				cyclesRan += 4;
				
				break;
			}
			case 0xE1:
			{
				uint8_t lsb = read(stkp);
				stkp++;

				uint8_t msb = read(stkp);
				stkp++;

				registers.hl = (msb << 8) | lsb;

				cycles += 12;
				cyclesRan += 12;

				break;
			}
			case 0x16:
			{
				registers.d = read(pc);

				pc++;
				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0x19:
			{
				uint32_t result = registers.hl + registers.de;
				registers.hl = result & 0xFFFF;

				setFlag(Subtraction, false);
				setFlag(HalfCarry, (((registers.hl & 0xFFF) + (registers.de & 0xFFF)) & 0x1000) == 0x1000);
				setFlag(Carry, (result & 0x10000) == 0x10000);

				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0x56:
			{
				registers.d = read(registers.hl);

				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0x5E:
			{
				registers.e = read(registers.hl);

				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0x23:
			{
				registers.hl++;
				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0xD5:
			{
				stkp--;
				write(stkp, (registers.de >> 8) & 0xFF);
				stkp--;
				write(stkp, registers.de & 0xFF);

				cycles += 16;
				cyclesRan += 16;

				break;
			}
			case 0xE9:
			{
				pc = registers.hl;
				cycles += 4;
				cyclesRan += 4;

				break;
			}
			case 0x7C:
			{
				registers.a = registers.h;

				cycles += 4;
				cyclesRan += 4;

				break;
			}
			case 0x7D:
			{
				registers.a = registers.l;

				cycles += 4;
				cyclesRan += 4;

				break;
			}
			case 0xC5:
			{
				stkp--;
				write(stkp, (registers.bc >> 8) & 0xFF);
				stkp--;
				write(stkp, registers.bc & 0xFF);

				cycles += 16;
				cyclesRan += 16;

				break;
			}
			case 0xE5:
			{
				stkp--;
				write(stkp, (registers.hl >> 8) & 0xFF);
				stkp--;
				write(stkp, registers.hl & 0xFF);

				cycles += 16;
				cyclesRan += 16;

				break;
			}
			case 0xF5:
			{
				stkp--;
				write(stkp, (registers.af >> 8) & 0xFF);
				stkp--;
				write(stkp, registers.af & 0xFF);

				cycles += 16;
				cyclesRan += 16;

				break;
			}
			case 0xF1:
			{
				uint8_t lo = read(stkp++);
				uint8_t hi = read(stkp++);
				registers.af = (hi << 8) | lo;

				cycles += 12;
				cyclesRan += 12;

				break;
			}
			case 0xC1:
			{
				uint8_t lo = read(stkp++);
				uint8_t hi = read(stkp++);
				registers.bc = (hi << 8) | lo;

				cycles += 12;
				cyclesRan += 12;

				break;
			}
			case 0x03:
			{
				registers.bc++;

				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0xFA:
			{
				uint8_t lo = read(pc);
				pc++;
				uint8_t hi = read(pc);
				pc++;

				uint16_t address = (hi << 8) | lo;

				registers.a = read(address);

				cycles += 16;
				cyclesRan += 16;

				break;
			}
			case 0x11:
			{
				uint8_t lo = read(pc);
				pc++;
				uint8_t hi = read(pc);
				pc++;

				uint16_t value = (hi << 8) | lo;

				registers.de = value;

				cycles += 12;
				cyclesRan += 12;

				break;
			}
			case 0x12:
			{
				write(registers.de, registers.a);

				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0x14:
			{
				uint8_t originalValue = registers.d;

				registers.d++;

				setFlag(Zero, registers.d == 0x00);
				setFlag(Subtraction, false);
				setFlag(HalfCarry, ((originalValue & 0x0F) + 1) > 0x0F);

				cycles += 4;
				cyclesRan += 4;

				break;
			}
			case 0x1C:
			{
				uint8_t originalValue = registers.e;

				registers.e++;

				setFlag(Zero, registers.e == 0x00);
				setFlag(Subtraction, false);
				setFlag(HalfCarry, ((originalValue & 0x0F) + 1) > 0x0F);

				cycles += 4;
				cyclesRan += 4;

				break;
			}
			case 0xC4:
			{
				uint8_t lo = read(pc);
				pc++;
				uint8_t hi = read(pc);
				pc++;

				uint16_t address = (hi << 8) | lo;

				if (!isFlagSet(Zero))
				{
					pc = address;
					cycles += 24;
					cyclesRan += 24;

					break;
				}
				else
				{
					cycles += 12;
					cyclesRan += 24;

					break;
				}
			}
			case 0x77:
			{
				write(registers.hl, registers.a);

				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0x24:
			{
				registers.h++;

				setFlag(Zero, registers.h == 0x00);
				setFlag(Subtraction, false);
				setFlag(HalfCarry, (registers.h & 0x0F) == 0x00);

				cycles += 4;
				cyclesRan += 4;

				break;
			}
			case 0x2C:
			{
				registers.l++;

				setFlag(Zero, registers.l == 0x00);
				setFlag(Subtraction, false);
				setFlag(HalfCarry, (registers.l & 0x0F) == 0x00);

				cycles += 4;
				cyclesRan += 4;

				break;
			}
			case 0x13:
			{
				registers.de++;

				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0x22:
			{
				write(registers.hl++, registers.a);

				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0x1A:
			{
				registers.a = read(registers.de);

				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0x46:
			{
				registers.b = read(registers.hl);
				
				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0xC6:
			{
				uint8_t immediateValue = read(pc);
				pc++;

				uint16_t result = registers.a + immediateValue;

				setFlag(Zero, (result & 0xFF) == 0x00);
				setFlag(Subtraction, false);
				setFlag(HalfCarry, (registers.a & 0x0F) + (immediateValue & 0x0F) > 0x0F);
				setFlag(Carry, result > 0xFF);

				registers.a = result & 0xFF;

				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0xD6:
			{
				uint8_t immediateValue = read(pc);
				pc++;

				uint16_t result = registers.a - immediateValue;

				setFlag(Zero, (result & 0xFF) == 0x00);
				setFlag(Subtraction, true);
				setFlag(HalfCarry, (registers.a & 0x0F) < (immediateValue & 0x0F));
				setFlag(Carry, result > 0xFF);

				registers.a = result & 0xFF;

				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0x2D:
			{
				registers.l--;

				setFlag(Zero, registers.l == 0x00);
				setFlag(Subtraction, true);
				setFlag(HalfCarry, (registers.l & 0x0F) == 0x0F);

				cycles += 4;
				cyclesRan += 4;

				break;
			}
			case 0x4E:
			{
				registers.c = read(registers.hl);

				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0xAE:
			{
				uint8_t value = read(registers.hl);

				registers.a ^= value;

				setFlag(Zero, registers.a == 0x00);
				setFlag(Subtraction, false);
				setFlag(HalfCarry, false);
				setFlag(Carry, false);

				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0x26:
			{
				registers.h = read(pc);
				pc++;

				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0x1F:
			{
				uint8_t originalValue = registers.a;

				registers.a = (registers.a >> 1) | (isFlagSet(Carry) << 7);

				setFlag(Zero, false);
				setFlag(Subtraction, false);
				setFlag(HalfCarry, false);
				setFlag(Carry, originalValue & 0x01);

				cycles += 4;
				cyclesRan += 4;

				break;
			}
			case 0x30:
			{
				int8_t offset = static_cast<int8_t>(read(pc));
				pc++;

				if (!isFlagSet(Carry))
				{

					pc += offset;

					cycles += 12;
					cyclesRan += 12;
				}
				else
				{
					cycles += 8;
					cyclesRan += 8;
				}

				break;
			}
			case 0xEE:
			{
				uint8_t value = read(pc);
				pc++;

				registers.a ^= value;

				setFlag(Zero, registers.a == 0);
				setFlag(Subtraction, false);
				setFlag(HalfCarry, false);
				setFlag(Carry, false);

				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0x70:
			{
				write(registers.hl, registers.b);

				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0x7A:
			{
				registers.a = registers.d;

				cycles += 4;
				cyclesRan += 4;

				break;
			}
			case 0x7B:
			{
				registers.a = registers.e;

				cycles += 4;
				cyclesRan += 4;

				break;
			}
			case 0x57:
			{
				registers.d = registers.a;

				cycles += 4;
				cyclesRan += 4;

				break;
			}
			case 0x25:
			{
				registers.h--;

				setFlag(Zero, (registers.hl & 0xFF) == 0);
				setFlag(Subtraction, true);
				setFlag(HalfCarry, (registers.hl & 0x0F) == 0x0F);

				cycles += 4;
				cyclesRan += 4;

				break;
			}
			default:
				std::cout << "Unknown instruction 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(instruction) << " found" << std::endl;
				setHasNotBroken(false);
				break;
			}
		}
		else
		{
			std::cout << "Reading From CB Table" << std::endl;

			switch (instruction)
			{
			case 0x87:
			{
				registers.a &= ~(1 << 0);
				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0x37:
			{
				uint8_t lowerNibble = registers.a & 0x0F;
				uint8_t upperNibble = (registers.a & 0xF0) >> 4;

				uint8_t result = (lowerNibble << 4) | upperNibble;

				registers.a = result;

				setFlag(Zero, result == 0);
				setFlag(Subtraction, false);
				setFlag(HalfCarry, false);
				setFlag(Carry, false);

				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0x38:
			{
				uint8_t originalValue = registers.b;
				registers.b >>= 1;

				setFlag(Zero, registers.b == 0);
				setFlag(Subtraction, false);
				setFlag(HalfCarry, false);
				setFlag(Carry, originalValue & 0x01);

				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0x1A:
			{
				uint8_t originalValue = registers.d;

				registers.d = (registers.d >> 1) | (isFlagSet(Carry) << 7);

				setFlag(Zero, registers.d == 0);
				setFlag(Subtraction, false);
				setFlag(HalfCarry, false);
				setFlag(Carry, originalValue & 0x01);

				cycles += 8;
				cyclesRan += 8;

				break;
			}
			case 0x19:
			{
				uint8_t originalValue = registers.c;

				registers.c = (registers.c >> 1) | (isFlagSet(Carry) << 7);

				setFlag(Zero, registers.c == 0);
				setFlag(Subtraction, false);
				setFlag(HalfCarry, false);
				setFlag(Carry, originalValue & 0x01);

				cycles += 8;
				cyclesRan += 8;

				break;
			}
			default:
				std::cout << "Unknown CB instruction 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(instruction) << " found" << std::endl;
				setHasNotBroken(false);
				break;
			}

			wasCB = false;
		}
		
	}
}

void Cpu::loadRom(std::vector<uint8_t> romData)
{
	for (uint16_t addr = 0x0000; addr < romData.size(); addr++)
	{
		write(addr, romData[addr]);
	}
	setTitle();
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

void Cpu::NOP()
{
	cycles += 4;
	cyclesRan += 4;
}

void Cpu::LDrrd16()
{
	uint8_t opcode = read(pc);
	pc++;
	uint8_t lo = read(pc);
	pc++;
	uint8_t hi = read(pc);
	pc++;

	uint16_t value = (hi << 8) | lo;

	switch (opcode)
	{
		case 0x01: // LD BC, d16
			registers.bc = value;
			break;
		case 0x11: // LD DE, d16
			registers.de = value;
			break;
		case 0x21: // LD HL, d16
			registers.hl = value;
			break;
		case 0x31: // LD SP, d16
			stkp = value;
			break;
		default:
			std::cout << "Unknown LDrrd16 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	cycles += 12;
	cyclesRan += 12;
}

void Cpu::LDarrR()
{
	uint8_t opcode = read(pc);
	pc++;

	switch (opcode)
	{
		case 0x02: // LD (BC), A
			write(registers.bc, registers.a);
			break;
		case 0x12: // LD (DE), A
			write(registers.de, registers.a);
			break;
		case 0x22: // LD (HL+), A
			write(registers.hl++, registers.a);
			break;
		case 0x32: // LD (HL-), A
			write(registers.hl--, registers.a);
			break;
		case 0x70: // LD (HL), B
			write(registers.hl, registers.b);
			break;
		case 0x71: // LD (HL), C
			write(registers.hl, registers.c);
			break;
		case 0x72: // LD (HL), D
			write(registers.hl, registers.d);
			break;
		case 0x73: // LD (HL), E
			write(registers.hl, registers.e);
			break;
		case 0x74: // LD (HL), H
			write(registers.hl, registers.h);
			break;
		case 0x75: // LD (HL), L
			write(registers.hl, registers.l);
			break;
		case 0x77: // LD (HL), A
			write(registers.hl, registers.a);
			break;
		default:
			std::cout << "Unknown LDarrR instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	cycles += 8;
	cyclesRan += 8;
}
