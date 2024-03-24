#include "Bus.h"
#include "Cpu.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <conio.h>

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
	ime = 0x00;

	write(lcdc, 0x91);
	write(stat, 0x85);

	tileMapAddress = 0x9800;

	using a = Cpu;

	lookup = {
		INSTRUCTION{ "NOP", &a::NOP }, INSTRUCTION{ "LD BC, d16", &a::LDrrd16 }, INSTRUCTION{ "LD (BC), A", &a::LDarrR }, INSTRUCTION{ "INC BC", &a::INCrr }, INSTRUCTION{ "INC B", &a::INCr }, INSTRUCTION{ "DEC B", &a::DECr }, INSTRUCTION{ "LD B, d8", &a::LDrd8 }, INSTRUCTION{ "RLCA; UNIMPLEMENTED", &a::XXX }, INSTRUCTION{ "LD (a16), SP", &a::LDa16SP }, INSTRUCTION{ "ADD HL, BC; UNIMPLEMENTED", &a::XXX }, INSTRUCTION{ "LD A, (BC)", &a::LDraRR }, INSTRUCTION{ "DEC BC", &a::DECrr }, INSTRUCTION{ "INC C", &a::INCr }, INSTRUCTION{ "DEC C", &a::DECr }, INSTRUCTION{ "LD C, d8", &a::LDrd8 }, INSTRUCTION{ "RRCA; UNIMPLEMENTED", &a::XXX },
		INSTRUCTION{ "STOP 0; UNIMPLEMENTED", &a::XXX }, INSTRUCTION{ "LD DE, d16", &a::LDrrd16 }, INSTRUCTION{ "LD (DE), d16", &a::LDarrR }, INSTRUCTION{ "INC DE", &a::INCrr }, INSTRUCTION{ "INC D", &a::INCr }, INSTRUCTION{ "DEC D", &a::DECr }, INSTRUCTION{ "LD D, d8", &a::LDrd8 }, INSTRUCTION{ "RLA; UNIMPLEMENTED", &a::XXX }, INSTRUCTION{ "JR r8", &a::JRcr8 }, INSTRUCTION{ "ADD HL, BC; UNIMPLEMENTED", &a::XXX }, INSTRUCTION{ "LD A, (DE)", &a::LDraRR }, INSTRUCTION{ "DEC DE", &a::DECrr }, INSTRUCTION{ "INC E", &a::INCr }, INSTRUCTION{ "DEC E", &a::DECr }, INSTRUCTION{ "LD E, d8", &a::LDrd8 }, INSTRUCTION{ "RRA; UNIMPLEMENTED", &a::XXX },
		INSTRUCTION{ "JR NZ, r8", &a::JRcr8 }, INSTRUCTION{ "LD HL, d16", &a::LDrrd16 }, INSTRUCTION{ "LD (HL+), d16", &a::LDarrR }, INSTRUCTION{ "INC HL", &a::INCrr }, INSTRUCTION{ "INC H", &a::INCr }, INSTRUCTION{ "DEC H", &a::DECr }, INSTRUCTION{ "LD H, d8", &a::LDrd8 }, INSTRUCTION{ "DAA; UNIMPLEMENTED", &a::XXX }, INSTRUCTION{ "JR Z r8", &a::JRcr8 }, INSTRUCTION{ "ADD HL, HL; UNIMPLEMENTED", &a::XXX }, INSTRUCTION{ "LD A, (HL+)", &a::LDraRR }, INSTRUCTION{ "DEC HL", &a::DECrr }, INSTRUCTION{ "INC L", &a::INCr }, INSTRUCTION{ "DEC L", &a::DECr }, INSTRUCTION{ "LD L, d8", &a::LDrd8 }, INSTRUCTION{ "CPL; UNIMPLEMENTED", &a::XXX },
		INSTRUCTION{ "JR NC, r8", &a::JRcr8 }, INSTRUCTION{ "LD SP, d16", &a::LDrrd16 }, INSTRUCTION{ "LD (HL-), d16", &a::LDarrR }, INSTRUCTION{ "INC SP", &a::INCrr }, INSTRUCTION{ "INC (HL)", &a::INCr }, INSTRUCTION{ "DEC (HL)", &a::DECr }, INSTRUCTION{ "LD (HL), d8", &a::LDrd8 }, INSTRUCTION{ "SCF; UNIMPLEMENTED", &a::XXX }, INSTRUCTION{ "JR C r8", &a::JRcr8 }, INSTRUCTION{ "ADD HL, SP; UNIMPLEMENTED", &a::XXX }, INSTRUCTION{ "LD A, (HL-)", &a::LDraRR }, INSTRUCTION{ "DEC SP", &a::DECrr }, INSTRUCTION{ "INC A", &a::INCr }, INSTRUCTION{ "DEC A", &a::DECr }, INSTRUCTION{ "LD A, d8", &a::LDrd8 }, INSTRUCTION{ "CCF; UNIMPLEMENTED", &a::XXX },
		INSTRUCTION{ "LD B, B", &a::LDrR }, INSTRUCTION{ "LD B, C", &a::LDrR }, INSTRUCTION{ "LD B, D", &a::LDrR }, INSTRUCTION{ "LD B, E", &a::LDrR }, INSTRUCTION{ "LD B, H", &a::LDrR }, INSTRUCTION{ "LD B, L", &a::LDrR }, INSTRUCTION{ "LD B, (HL)", &a::LDrR }, INSTRUCTION{ "LD B, A", &a::LDrR }, INSTRUCTION{ "LD C, B", &a::LDrR }, INSTRUCTION{ "LD C, C", &a::LDrR }, INSTRUCTION{ "LD C, D", &a::LDrR }, INSTRUCTION{ "LD C, E", &a::LDrR }, INSTRUCTION{ "LD C, H", &a::LDrR }, INSTRUCTION{ "LD C, L", &a::LDrR }, INSTRUCTION{ "LD C, (HL)", &a::LDrR }, INSTRUCTION{ "LD C, A", &a::LDrR },
		INSTRUCTION{ "LD D, B", &a::LDrR }, INSTRUCTION{ "LD D, C", &a::LDrR }, INSTRUCTION{ "LD D, D", &a::LDrR }, INSTRUCTION{ "LD D, E", &a::LDrR }, INSTRUCTION{ "LD D, H", &a::LDrR }, INSTRUCTION{ "LD D, L", &a::LDrR }, INSTRUCTION{ "LD D, (HL)", &a::LDrR }, INSTRUCTION{ "LD D, A", &a::LDrR }, INSTRUCTION{ "LD E, B", &a::LDrR }, INSTRUCTION{ "LD E, C", &a::LDrR }, INSTRUCTION{ "LD E, D", &a::LDrR }, INSTRUCTION{ "LD E, E", &a::LDrR }, INSTRUCTION{ "LD E, H", &a::LDrR }, INSTRUCTION{ "LD E, L", &a::LDrR }, INSTRUCTION{ "LD E, (HL)", &a::LDrR }, INSTRUCTION{ "LD E, A", &a::LDrR },
		INSTRUCTION{ "LD H, B", &a::LDrR }, INSTRUCTION{ "LD H, C", &a::LDrR }, INSTRUCTION{ "LD H, D", &a::LDrR }, INSTRUCTION{ "LD H, E", &a::LDrR }, INSTRUCTION{ "LD H, H", &a::LDrR }, INSTRUCTION{ "LD H, L", &a::LDrR }, INSTRUCTION{ "LD H, (HL)", &a::LDrR }, INSTRUCTION{ "LD H, A", &a::LDrR }, INSTRUCTION{ "LD L, B", &a::LDrR }, INSTRUCTION{ "LD L, C", &a::LDrR }, INSTRUCTION{ "LD L, D", &a::LDrR }, INSTRUCTION{ "LD L, E", &a::LDrR }, INSTRUCTION{ "LD L, H", &a::LDrR }, INSTRUCTION{ "LD L, L", &a::LDrR }, INSTRUCTION{ "LD L, (HL)", &a::LDrR }, INSTRUCTION{ "LD L, A", &a::LDrR },
		INSTRUCTION{ "LD (HL), B", &a::LDarrR }, INSTRUCTION{ "LD (HL), C", &a::LDarrR }, INSTRUCTION{ "LD (HL), D", &a::LDarrR }, INSTRUCTION{ "LD (HL), E", &a::LDarrR }, INSTRUCTION{ "LD (HL), H", &a::LDarrR }, INSTRUCTION{ "LD (HL), L", &a::LDarrR }, INSTRUCTION{ "HALT; UNIMPLEMENTED", &a::XXX }, INSTRUCTION{ "LD (HL), A", &a::LDarrR }, INSTRUCTION{ "LD A, B", &a::LDrR }, INSTRUCTION{ "LD A, C", &a::LDrR }, INSTRUCTION{ "LD A, D", &a::LDrR }, INSTRUCTION{ "LD A, E", &a::LDrR }, INSTRUCTION{ "LD A, H", &a::LDrR }, INSTRUCTION{ "LD A, L", &a::LDrR }, INSTRUCTION{ "LD A, (HL)", &a::LDrR }, INSTRUCTION{ "LD A, A", &a::LDrR },
		INSTRUCTION{ "ADD A, B", &a::ADDrR }, INSTRUCTION{ "ADD A, C", &a::ADDrR }, INSTRUCTION{ "ADD A, D", &a::ADDrR }, INSTRUCTION{ "ADD A, E", &a::ADDrR }, INSTRUCTION{ "ADD A, H", &a::ADDrR }, INSTRUCTION{ "ADD A, L", &a::ADDrR }, INSTRUCTION{ "ADD A, (HL)", &a::ADDrR }, INSTRUCTION{ "ADD A, A", &a::ADDrR }, INSTRUCTION{ "ADC A, B", &a::ADDrR }, INSTRUCTION{ "ADC A, C", &a::ADDrR }, INSTRUCTION{ "ADC A, D", &a::ADDrR }, INSTRUCTION{ "ADC A, E", &a::ADDrR }, INSTRUCTION{ "ADC A, H", &a::ADDrR }, INSTRUCTION{ "ADC A, L", &a::ADDrR }, INSTRUCTION{ "ADC A, (HL)", &a::ADDrR }, INSTRUCTION{ "ADC A, A", &a::ADDrR },
		INSTRUCTION{ "SUB B" , &a::SUBr }, INSTRUCTION{ "SUB C", &a::SUBr }, INSTRUCTION{ "SUB D", &a::SUBr }, INSTRUCTION{ "SUB E", &a::SUBr }, INSTRUCTION{ "SUB H", &a::SUBr }, INSTRUCTION{ "SUB L", &a::SUBr }, INSTRUCTION{ "SUB (HL)", &a::SUBr }, INSTRUCTION{ "SUB A", &a::SUBr }, INSTRUCTION{ "SBC A, B", &a::SUBr }, INSTRUCTION{ "SBC A, C", &a::SUBr }, INSTRUCTION{ "SBC A, D", &a::SUBr }, INSTRUCTION{ "SBC A, E", &a::SUBr }, INSTRUCTION{ "SBC A, H", &a::SUBr }, INSTRUCTION{ "SBC A, L", &a::SUBr }, INSTRUCTION{ "SBC A, (HL)", &a::SUBr }, INSTRUCTION{ "SBC A, A", &a::SUBr },
		INSTRUCTION{ "AND B", &a::ANDr }, INSTRUCTION{ "AND C", &a::ANDr }, INSTRUCTION{ "AND D", &a::ANDr }, INSTRUCTION{ "AND E", &a::ANDr }, INSTRUCTION{ "AND H", &a::ANDr }, INSTRUCTION{ "AND L", &a::ANDr }, INSTRUCTION{ "AND (HL)", &a::ANDr }, INSTRUCTION{ "AND A", &a::ANDr }, INSTRUCTION{ "XOR B", &a::XORr }, INSTRUCTION{ "XOR C", &a::XORr }, INSTRUCTION{ "XOR D", &a::XORr }, INSTRUCTION{ "XOR E", &a::XORr }, INSTRUCTION{ "XOR H", &a::XORr }, INSTRUCTION{ "XOR L", &a::XORr }, INSTRUCTION{ "XOR (HL)", &a::XORr }, INSTRUCTION{ "XOR A", &a::XORr },
		INSTRUCTION{ "OR B", &a::ORr }, INSTRUCTION{ "OR C", &a::ORr }, INSTRUCTION{ "OR D", &a::ORr }, INSTRUCTION{ "OR E", &a::ORr }, INSTRUCTION{ "OR H", &a::ORr }, INSTRUCTION{ "OR L", &a::ORr }, INSTRUCTION{ "OR (HL)", &a::ORr }, INSTRUCTION{ "OR A", &a::ORr }, INSTRUCTION{ "CP B", &a::CPr }, INSTRUCTION{ "CP C", &a::CPr }, INSTRUCTION{ "CP D", &a::CPr }, INSTRUCTION{ "CP E", &a::CPr }, INSTRUCTION{ "CP H", &a::CPr }, INSTRUCTION{ "CP L", &a::CPr }, INSTRUCTION{ "CP (HL)", &a::CPr }, INSTRUCTION{ "CP A", &a::CPr },
		INSTRUCTION{ "RET NZ", &a::RETc }, INSTRUCTION{ "POP BC", &a::POPrr }, INSTRUCTION{ "JP NZ, a16", &a::JPca16 }, INSTRUCTION{ "JP a16", &a::JPca16 }, INSTRUCTION{ "CALL NZ, a16", &a::CALLca16 }, INSTRUCTION{ "PUSH BC", &a::PUSHrr }, INSTRUCTION{ "ADD A, d8", &a::ADDrR }, INSTRUCTION{ "RST 00H", &a::RSTn }, INSTRUCTION{ "RET Z", &a::RETc }, INSTRUCTION{ "RET", &a::RETc }, INSTRUCTION{ "JP Z, a16", &a::JPca16 }, INSTRUCTION{ "PREFIX CB; UNIMPLEMENTED", &a::XXX }, INSTRUCTION{ "CALL Z, a16", &a::CALLca16 }, INSTRUCTION{ "CALL a16", &a::CALLca16 }, INSTRUCTION{ "ADC A, d8; UNIMPLEMENTED", &a::XXX }, INSTRUCTION{ "RST 08H", &a::RSTn },
		INSTRUCTION{ "RET NC", &a::RETc }, INSTRUCTION{ "POP DE", &a::POPrr }, INSTRUCTION{ "JP NC, a16", &a::JPca16 }, INSTRUCTION{ "ILLEGAL", &a::XXX }, INSTRUCTION{ "CALL NC, a16", &a::CALLca16 }, INSTRUCTION{ "PUSH DE", &a::PUSHrr }, INSTRUCTION{ "SUB d8", &a::SUBr }, INSTRUCTION{ "RST 10H", &a::RSTn }, INSTRUCTION{ "RET C", &a::RETc }, INSTRUCTION{ "RETI; UNIMPLEMENTED", &a::XXX }, INSTRUCTION{ "JP C, a16", &a::JPca16 }, INSTRUCTION{ "ILLEGAL", &a::XXX }, INSTRUCTION{ "CALL C, a16", &a::CALLca16 }, INSTRUCTION{ "ILLEGAL", &a::XXX }, INSTRUCTION{ "SBC A, d8; UNIMPLEMENTED", &a::XXX }, INSTRUCTION{ "RST 18H", &a::RSTn },
		INSTRUCTION{ "LDH (a8), A", &a::LDH }, INSTRUCTION{ "POP HL", &a::POPrr }, INSTRUCTION{ "LD (C), A", &a::LD_C_A }, INSTRUCTION{ "ILLEGAL", &a::XXX }, INSTRUCTION{ "ILLEGAL", &a::XXX }, INSTRUCTION{ "PUSH HL", &a::PUSHrr }, INSTRUCTION{ "AND d8", &a::ANDr }, INSTRUCTION{ "RST 20H", &a::RSTn }, INSTRUCTION{ "ADD SP, r8; UNIMPLEMENTED", &a::XXX }, INSTRUCTION{ "JP (HL)", &a::JPaHL }, INSTRUCTION{ "LD (a16), A", &a::LDa16A }, INSTRUCTION{ "ILLEGAL", &a::XXX }, INSTRUCTION{ "ILLEGAL", &a::XXX }, INSTRUCTION{ "ILLEGAL", &a::XXX }, INSTRUCTION{ "XOR d8", &a::XORr }, INSTRUCTION{ "RST 28H", &a::RSTn },
		INSTRUCTION{ "LDH A, (a8)", &a::LDH }, INSTRUCTION{ "POP AF", &a::POPrr }, INSTRUCTION{ "LD A, (C)", &a::LD_C_A }, INSTRUCTION{ "DI", &a::DI }, INSTRUCTION{ "ILLEGAL", &a::XXX }, INSTRUCTION{ "PUSH AF", &a::PUSHrr }, INSTRUCTION{ "OR d8; UNIMPLEMENTED", &a::XXX }, INSTRUCTION{ "RST 30H", &a::RSTn }, INSTRUCTION{ "LD HL, SP+r8; UNIMPLEMENTED", &a::XXX }, INSTRUCTION{ "LD SP, HL; UNIMPLEMENTED", &a::XXX }, INSTRUCTION{ "LD A, (a16)", &a::LDa16A }, INSTRUCTION{ "EI; UNIMPLEMENTED", &a::XXX }, INSTRUCTION{ "ILLEGAL", &a::XXX }, INSTRUCTION{ "ILLEGAL", &a::XXX }, INSTRUCTION{ "CP d8", &a::CPr }, INSTRUCTION{ "RST 38H", &a::RSTn }
	};
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
	return (registers.f & (1 << flag)) != 0;
}

void Cpu::runInstruction()
{
	if (cycles == 0)
	{
		if (stepMode) 
		{
			std::cout << "Press spacebar to execute the next instruction..." << std::endl;
			while (!_kbhit() || _getch() != ' ');
		}

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
		std::cout << "INSTRUCTION: " << lookup[read(pc)].name << std::endl << std::endl;

		(this->*lookup[read(pc)].operate)();
		
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
	pc++;
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
		{
			registers.bc = value;
			break;
		}
		case 0x11: // LD DE, d16
		{
			registers.de = value;
			break;
		}
		case 0x21: // LD HL, d16
		{
			registers.hl = value;
			break;
		}
		case 0x31: // LD SP, d16
		{
			stkp = value;
			break;
		}
		default:
			std::cout << "Unknown LDrrd16 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	cycles += 12;
	cyclesRan += 12;
}

void Cpu::LDraRR()
{
	uint8_t opcode = read(pc++);

	switch (opcode)
	{
		case 0x0A:
		{
			registers.a = read(registers.bc);
			break;
		}
		case 0x1A:
		{
			registers.a = read(registers.de);
			break;
		}
		case 0x2A:
		{
			registers.a = read(registers.hl++);
			break;
		}
		case 0x3A:
		{
			registers.a = read(registers.hl--);
			break;
		}
		default:
			std::cout << "Unknown LDraRR instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	cycles += 8;
}

void Cpu::LDarrR()
{
	uint8_t opcode = read(pc);
	pc++;

	switch (opcode)
	{
		case 0x02: // LD (BC), A
		{
			write(registers.bc, registers.a);
			break;
		}
		case 0x12: // LD (DE), A
		{
			write(registers.de, registers.a);
			break;
		}
		case 0x22: // LD (HL+), A
		{
			write(registers.hl++, registers.a);
			break;
		}
		case 0x32: // LD (HL-), A
		{
			write(registers.hl--, registers.a);
			break;
		}
		case 0x70: // LD (HL), B
		{
			write(registers.hl, registers.b);
			break;
		}
		case 0x71: // LD (HL), C
		{
			write(registers.hl, registers.c);
			break;
		}
		case 0x72: // LD (HL), D
		{
			write(registers.hl, registers.d);
			break;
		}
		case 0x73: // LD (HL), E
		{
			write(registers.hl, registers.e);
			break;
		}
		case 0x74: // LD (HL), H
		{
			write(registers.hl, registers.h);
			break;
		}
		case 0x75: // LD (HL), L
		{
			write(registers.hl, registers.l);
			break;
		}
		case 0x77: // LD (HL), A
		{
			write(registers.hl, registers.a);
			break;
		}
		default:
			std::cout << "Unknown LDarrR instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	cycles += 8;
	cyclesRan += 8;
}

void Cpu::INCrr()
{
	uint8_t opcode = read(pc);
	pc++;

	switch (opcode)
	{
		case 0x03: // INC BC
		{
			registers.bc++;
			break;
		}
		case 0x13: // INC DE
		{
			registers.de++;
			break;
		}
		case 0x23: // INC HL
		{
			registers.hl++;
			break;
		}
		case 0x33: // INC SP (Stack Pointer)
		{
			stkp++;
			break;
		}
		default:
			std::cout << "Unknown INCrr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	cycles += 8;
	cyclesRan += 8;
}

void Cpu::DECrr()
{
	uint8_t opcode = read(pc);
	pc++;

	switch (opcode)
	{
	case 0x0B: // DEC BC
	{
		registers.bc--;
		break;
	}
	case 0x1B: // DEC DE
	{
		registers.de--;
		break;
	}
	case 0x2B: // DEC HL
	{
		registers.hl--;
		break;
	}
	case 0x3B: // DEC SP (Stack Pointer)
	{
		stkp--;
		break;
	}
	default:
		std::cout << "Unknown DECrr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		setHasNotBroken(false);
		break;
	}

	cycles += 8;
	cyclesRan += 8;
}

void Cpu::INCr()
{
	uint8_t opcode = read(pc);
	pc++;

	uint8_t* regPtr = nullptr;

	switch (opcode)
	{
		case 0x04: // INC B
		{
			regPtr = &registers.b;
			break;
		}
		case 0x14: // INC D
		{
			regPtr = &registers.d;
			break;
		}
		case 0x24: // INC H
		{
			regPtr = &registers.h;
			break;
		}
		case 0x0C: // INC C
		{
			regPtr = &registers.c;
			break;
		}
		case 0x1C: // INC E
		{
			regPtr = &registers.e;
			break;
		}
		case 0x2C: // INC L
		{
			regPtr = &registers.l;
			break;
		}
		case 0x3C: // INC A
		{
			regPtr = &registers.a;
			break;
		}
		default:
			std::cout << "Unknown INCr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	if (regPtr != nullptr)
	{
		(*regPtr)++;

		setFlag(Zero, (*regPtr) == 0);
		setFlag(Subtraction, false);
		setFlag(HalfCarry, ((*regPtr) & 0x0F) == 0);
	}

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::DECr()
{
	uint8_t opcode = read(pc);
	pc++;

	uint8_t* regPtr = nullptr;

	switch (opcode)
	{
		case 0x05: // DEC B
		{
			regPtr = &registers.b;
			break;
		}
		case 0x15: // DEC D
		{
			regPtr = &registers.d;
			break;
		}
		case 0x25: // DEC H
		{
			regPtr = &registers.h;
			break;
		}
		case 0x0D: // DEC C
		{
			regPtr = &registers.c;
			break;
		}
		case 0x1D: // DEC E
		{
			regPtr = &registers.e;
			break;
		}
		case 0x2D: // DEC L
		{
			regPtr = &registers.l;
			break;
		}
		case 0x3D: // DEC A
		{
			regPtr = &registers.a;
			break;
		}
		default:
			std::cout << "Unknown DECr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	if (regPtr != nullptr)
	{
		(*regPtr)--;

		setFlag(Zero, (*regPtr) == 0);
		setFlag(Subtraction, true);
		setFlag(HalfCarry, ((*regPtr) & 0x0F) == 0);
	}

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::INCaRR()
{
	uint8_t opcode = read(pc);
	pc++;

	switch (opcode)
	{
		case 0x34: // INC (HL)
		{
			uint8_t value = read(registers.hl);
			value++;
			write(registers.hl, value);

			setFlag(Zero, value == 0);
			setFlag(Subtraction, false);
			setFlag(HalfCarry, (value & 0x0F) == 0);

			break;
		}
		default:
			std::cout << "Unknown INCaRR instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	cycles += 12;
	cyclesRan += 12;
}

void Cpu::DECaRR()
{
	uint8_t opcode = read(pc);
	pc++;

	switch (opcode)
	{
		case 0x35: // DEC (HL)
		{
			uint8_t value = read(registers.hl);
			value--;
			write(registers.hl, value);

			setFlag(Zero, value == 0);
			setFlag(Subtraction, true);
			setFlag(HalfCarry, (value & 0x0F) == 0);

			break;
		}
		default:
			std::cout << "Unknown DECaRR instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	cycles += 12;
	cyclesRan += 12;
}

void Cpu::LDrd8()
{
	uint8_t opcode = read(pc);
	pc++;

	uint8_t value = read(pc);
	pc++;

	switch (opcode)
	{
		case 0x06:
		{
			registers.b = value;
			break;
		}
		case 0x16:
		{
			registers.d = value;
			break;
		}
		case 0x26:
		{
			registers.h = value;
			break;
		}
		case 0x36:
		{
			write(registers.hl, value);
			cycles += 4;
			cyclesRan += 4;
			break;
		}
		case 0x0E:
		{
			registers.c = value;
			break;
		}
		case 0x1E:
		{
			registers.e = value;
			break;
		}
		case 0x2E:
		{
			registers.l = value;
			break;
		}
		case 0x3E:
		{
			registers.a = value;
			break;
		}
		default:
			std::cout << "Unknown LDrd8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	cycles += 8;
	cyclesRan += 8;
}

void Cpu::RLCA() // TODO: FINISH RLCA(); ROTATE REG A AND SET CARRY FLAG
{
	uint8_t opcode = read(pc);
	pc++;

	switch (opcode)
	{
		default:
			std::cout << "Unknown RLCA instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}
}

void Cpu::LDrR()
{
	uint8_t opcode = read(pc);
	pc++;

	switch (opcode)
	{
		case 0x40:
		{
			registers.b = registers.b;
			break;
		}
		case 0x41:
		{
			registers.b = registers.c;
			break;
		}
		case 0x42:
		{
			registers.b = registers.d;
			break;
		}
		case 0x43:
		{
			registers.b = registers.e;
			break;
		}
		case 0x44:
		{
			registers.b = registers.h;
			break;
		}
		case 0x45:
		{
			registers.b = registers.l;
			break;
		}
		case 0x46:
		{
			registers.b = read(registers.hl);
			cycles += 4;
			cyclesRan += 4;
			break;
		}
		case 0x47:
		{
			registers.b = registers.a;
			break;
		}
		case 0x48:
		{
			registers.c = registers.b;
			break;
		}
		case 0x49:
		{
			registers.c = registers.c;
			break;
		}
		case 0x4A:
		{
			registers.c = registers.d;
			break;
		}
		case 0x4B:
		{
			registers.c = registers.e;
			break;
		}
		case 0x4C:
		{
			registers.c = registers.h;
			break;
		}
		case 0x4D:
		{
			registers.c = registers.l;
			break;
		}
		case 0x4E:
		{
			registers.c = read(registers.hl);
			cycles += 4;
			cyclesRan += 4;
			break;
		}
		case 0x4F:
		{
			registers.c = registers.a;
			break;
		}
		case 0x50:
		{
			registers.d = registers.b;
			break;
		}
		case 0x51:
		{
			registers.d = registers.c;
			break;
		}
		case 0x52:
		{
			registers.d = registers.d;
			break;
		}
		case 0x53:
		{
			registers.d = registers.e;
			break;
		}
		case 0x54:
		{
			registers.d = registers.h;
			break;
		}
		case 0x55:
		{
			registers.d = registers.l;
			break;
		}
		case 0x56:
		{
			registers.d = read(registers.hl);
			cycles += 4;
			cyclesRan += 4;
			break;
		}
		case 0x57:
		{
			registers.d = registers.a;
			break;
		}
		case 0x58:
		{
			registers.e = registers.b;
			break;
		}
		case 0x59:
		{
			registers.e = registers.c;
			break;
		}
		case 0x5A:
		{
			registers.e = registers.d;
			break;
		}
		case 0x5B:
		{
			registers.e = registers.e;
			break;
		}
		case 0x5C:
		{
			registers.e = registers.h;
			break;
		}
		case 0x5D:
		{
			registers.e = registers.l;
			break;
		}
		case 0x5E:
		{
			registers.d = read(registers.hl);
			cycles += 4;
			cyclesRan += 4;
			break;
		}
		case 0x5F:
		{
			registers.e = registers.a;
			break;
		}
		case 0x60:
		{
			registers.h = registers.b;
			break;
		}
		case 0x61:
		{
			registers.h = registers.c;
			break;
		}
		case 0x62:
		{
			registers.h = registers.d;
			break;
		}
		case 0x63:
		{
			registers.h = registers.e;
			break;
		}
		case 0x64:
		{
			registers.h = registers.h;
			break;
		}
		case 0x65:
		{
			registers.h = registers.l;
			break;
		}
		case 0x66:
		{
			registers.h = read(registers.hl);
			cycles += 4;
			cyclesRan += 4;
			break;
		}
		case 0x67:
		{
			registers.h = registers.a;
			break;
		}
		case 0x68:
		{
			registers.l = registers.b;
			break;
		}
		case 0x69:
		{
			registers.l = registers.c;
			break;
		}
		case 0x6A:
		{
			registers.l = registers.d;
			break;
		}
		case 0x6B:
		{
			registers.l = registers.e;
			break;
		}
		case 0x6C:
		{
			registers.l = registers.h;
			break;
		}
		case 0x6D:
		{
			registers.l = registers.l;
			break;
		}
		case 0x6E:
		{
			registers.l = read(registers.hl);
			cycles += 4;
			cyclesRan += 4;
			break;
		}
		case 0x6F:
		{
			registers.l = registers.a;
			break;
		}
		case 0x78:
		{
			registers.a = registers.b;
			break;
		}
		case 0x79:
		{
			registers.a = registers.c;
			break;
		}
		case 0x7A:
		{
			registers.a = registers.d;
			break;
		}
		case 0x7B:
		{
			registers.a = registers.e;
			break;
		}
		case 0x7C:
		{
			registers.a = registers.h;
			break;
		}
		case 0x7D:
		{
			registers.a = registers.l;
			break;
		}
		case 0x7E:
		{
			registers.a = read(registers.hl);
			break;
		}
		case 0x7F:
		{
			registers.a = registers.a;
			break;
		}
		default:
			std::cout << "Unknown LDrR instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::ADDrR()
{
	uint8_t opcode = read(pc);
	pc++;

	uint8_t carry = isFlagSet(Carry) ? 1 : 0;
	uint16_t result = 0;

	switch (opcode)
	{
		case 0x80:
		{
			result = registers.a + registers.b;
			setFlag(HalfCarry, (registers.a & 0x0F) + (registers.b & 0x0F) > 0x0F);
			break;
		}
		case 0x81:
		{
			result = registers.a + registers.c;
			setFlag(HalfCarry, (registers.a & 0x0F) + (registers.c & 0x0F) > 0x0F);
			break;
		}
		case 0x82:
		{
			result = registers.a + registers.d;
			setFlag(HalfCarry, (registers.a & 0x0F) + (registers.d & 0x0F) > 0x0F);
			break;
		}
		case 0x83:
		{
			result = registers.a + registers.e;
			setFlag(HalfCarry, (registers.a & 0x0F) + (registers.e & 0x0F) > 0x0F);
			break;
		}
		case 0x84:
		{
			result = registers.a + registers.h;
			setFlag(HalfCarry, (registers.a & 0x0F) + (registers.h & 0x0F) > 0x0F);
			break;
		}
		case 0x85:
		{
			result = registers.a + registers.l;
			setFlag(HalfCarry, (registers.a & 0x0F) + (registers.l & 0x0F) > 0x0F);
			break;
		}
		case 0x86:
		{
			result = registers.a + read(registers.hl);
			setFlag(HalfCarry, (registers.a & 0x0F) + (read(registers.hl) & 0x0F) > 0x0F);

			cycles += 4;
			cyclesRan += 4;
			break;
		}
		case 0x87:
		{
			result = registers.a + registers.a;
			setFlag(HalfCarry, (registers.a & 0x0F) + (registers.a & 0x0F) > 0x0F);
			break;
		}
		case 0x88:
		{
			result = registers.a + registers.b + carry;

			setFlag(HalfCarry, (registers.a & 0x0F) + (registers.b & 0x0F) + carry > 0x0F);
			break;
		}
		case 0x89:
		{
			result = registers.a + registers.c + carry;

			setFlag(HalfCarry, (registers.a & 0x0F) + (registers.c & 0x0F) + carry > 0x0F);
			break;
		}
		case 0x8A:
		{
			result = registers.a + registers.d + carry;

			setFlag(HalfCarry, (registers.a & 0x0F) + (registers.d & 0x0F) + carry > 0x0F);
			break;
		}
		case 0x8B:
		{
			result = registers.a + registers.e + carry;

			setFlag(HalfCarry, (registers.a & 0x0F) + (registers.e & 0x0F) + carry > 0x0F);
			break;
		}
		case 0x8C:
		{
			result = registers.a + registers.h + carry;

			setFlag(HalfCarry, (registers.a & 0x0F) + (registers.h & 0x0F) + carry > 0x0F);
			break;
		}
		case 0x8D:
		{
			result = registers.a + registers.l + carry;

			setFlag(HalfCarry, (registers.a & 0x0F) + (registers.l & 0x0F) + carry > 0x0F);
			break;
		}
		case 0x8E:
		{
			result = registers.a + read(registers.hl) + carry;

			setFlag(HalfCarry, (registers.a & 0x0F) + (read(registers.hl) & 0x0F) + carry > 0x0F);
			
			cycles += 4;
			cycles += 4;
			break;
		}
		case 0x8F:
		{
			result = registers.a + registers.a + carry;

			setFlag(HalfCarry, (registers.a & 0x0F) + (registers.a & 0x0F) + carry > 0x0F);
			break;
		}
		case 0xC6:
		{
			uint8_t d8 = read(pc++);

			result = registers.a + d8;

			setFlag(HalfCarry, (registers.a & 0x0F) + (d8 & 0x0F) > 0x0F);

			cycles += 4;
			cyclesRan += 4;
			break;
		}
		default:
			std::cout << "Unknown ADDrR instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	setFlag(Zero, (result & 0xFF) == 0x00);
	setFlag(Subtraction, false);
	setFlag(Carry, result > 0xFF);

	registers.a = result & 0xFF;

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::SUBr()
{
	uint8_t opcode = read(pc);
	pc++;

	uint8_t result;

	switch (opcode)
	{
		case 0x90:
		{
			result = registers.a - registers.b;
			setFlag(HalfCarry, (registers.a & 0x0F) < (registers.b & 0x0F));
			setFlag(Carry, registers.b > registers.a);

			break;
		}
		case 0x91:
		{
			result = registers.a - registers.c;
			setFlag(HalfCarry, (registers.a & 0x0F) < (registers.c & 0x0F));
			setFlag(Carry, registers.c > registers.a);

			break;
		}
		case 0x92:
		{
			result = registers.a - registers.d;
			setFlag(HalfCarry, (registers.a & 0x0F) < (registers.d & 0x0F));
			setFlag(Carry, registers.d > registers.a);

			break;
		}
		case 0x93:
		{
			result = registers.a - registers.e;
			setFlag(HalfCarry, (registers.a & 0x0F) < (registers.e & 0x0F));
			setFlag(Carry, registers.e > registers.a);

			break;
		}
		case 0x94:
		{
			result = registers.a - registers.h;
			setFlag(HalfCarry, (registers.a & 0x0F) < (registers.h & 0x0F));
			setFlag(Carry, registers.h > registers.a);

			break;
		}
		case 0x95:
		{
			result = registers.a - registers.l;
			setFlag(HalfCarry, (registers.a & 0x0F) < (registers.l & 0x0F));
			setFlag(Carry, registers.l > registers.a);

			break;
		}
		case 0x96:
		{
			result = registers.a - read(registers.hl);
			setFlag(HalfCarry, (registers.a & 0x0F) < (read(registers.hl) & 0x0F));
			setFlag(Carry, read(registers.hl) > registers.a);

			break;
		}
		case 0x97:
		{
			result = registers.a - registers.a;
			setFlag(HalfCarry, (registers.a & 0x0F) < (registers.a & 0x0F));
			setFlag(Carry, registers.a > registers.a);

			cycles += 4;
			cyclesRan += 4;

			break;
		}
		case 0x98:
		{
			uint8_t carry = isFlagSet(Carry) ? 1 : 0;
			result = registers.a - registers.b - carry;

			setFlag(HalfCarry, (registers.a & 0x0F) < (registers.b & 0x0F) + carry);
			setFlag(Carry, registers.b + carry > registers.a);

			break;
		}
		case 0x99:
		{
			uint8_t carry = isFlagSet(Carry) ? 1 : 0;
			result = registers.a - registers.c - carry;

			setFlag(HalfCarry, (registers.a & 0x0F) < (registers.c & 0x0F) + carry);
			setFlag(Carry, registers.c + carry > registers.a);

			break;
		}
		case 0x9A:
		{
			uint8_t carry = isFlagSet(Carry) ? 1 : 0;
			result = registers.a - registers.d - carry;

			setFlag(HalfCarry, (registers.a & 0x0F) < (registers.d & 0x0F) + carry);
			setFlag(Carry, registers.d + carry > registers.a);

			break;
		}
		case 0x9B:
		{
			uint8_t carry = isFlagSet(Carry) ? 1 : 0;
			result = registers.a - registers.e - carry;

			setFlag(HalfCarry, (registers.a & 0x0F) < (registers.e & 0x0F) + carry);
			setFlag(Carry, registers.e + carry > registers.a);

			break;
		}
		case 0x9C:
		{
			uint8_t carry = isFlagSet(Carry) ? 1 : 0;
			result = registers.a - registers.h - carry;

			setFlag(HalfCarry, (registers.a & 0x0F) < (registers.h & 0x0F) + carry);
			setFlag(Carry, registers.h + carry > registers.a);

			break;
		}
		case 0x9D:
		{
			uint8_t carry = isFlagSet(Carry) ? 1 : 0;
			result = registers.a - registers.l - carry;

			setFlag(HalfCarry, (registers.a & 0x0F) < (registers.l & 0x0F) + carry);
			setFlag(Carry, registers.l + carry > registers.a);

			break;
		}
		case 0x9E:
		{
			uint8_t carry = isFlagSet(Carry) ? 1 : 0;
			result = registers.a - read(registers.hl) - carry;

			setFlag(HalfCarry, (registers.a & 0x0F) < (read(registers.hl) & 0x0F) + carry);
			setFlag(Carry, read(registers.hl) + carry > registers.a);

			cycles += 4;
			cycles += 4;

			break;
		}
		case 0x9F:
		{
			uint8_t carry = isFlagSet(Carry) ? 1 : 0;
			result = registers.a - registers.a - carry;

			setFlag(HalfCarry, (registers.a & 0x0F) < (registers.a & 0x0F) + carry);
			setFlag(Carry, registers.a + carry > registers.a);
			
			break;
		}
		case 0xD6:
		{
			uint8_t d8 = read(pc++);

			result = registers.a - d8;

			setFlag(HalfCarry, (registers.a & 0x0F) < (d8 & 0x0F));

			cycles += 4;
			cyclesRan += 4;
			break;
		}
		default:
			std::cout << "Unknown SUBr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	setFlag(Zero, result == 0x00);
	setFlag(Subtraction, true);

	registers.a = result;

	cycles += 4;
	cyclesRan += 4;

}

void Cpu::ANDr()
{
	uint8_t opcode = read(pc);
	pc++;

	uint8_t hlValue;
	uint8_t d8;

	uint8_t* regPtr = nullptr;

	switch (opcode)
	{
		case 0xA0:
		{
			regPtr = &registers.b;
			break;
		}
		case 0xA1:
		{
			regPtr = &registers.c;
			break;
		}
		case 0xA2:
		{
			regPtr = &registers.d;
			break;
		}
		case 0xA3:
		{
			regPtr = &registers.e;
			break;
		}
		case 0xA4:
		{
			regPtr = &registers.h;
			break;
		}
		case 0xA5:
		{
			regPtr = &registers.l;
			break;
		}
		case 0xA6:
		{
			hlValue = read(registers.hl);
			regPtr = &hlValue;

			cycles += 4;
			cyclesRan += 4;

			break;
		}
		case 0xA7:
		{
			regPtr = &registers.a;
			break;
		}
		case 0xE6:
		{
			d8 = read(pc++);
			regPtr = &d8;

			cycles += 4;
			cyclesRan += 4;
			break;
		}
		default:
			std::cout << "Unknown ANDr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	registers.a &= (*regPtr);

	setFlag(Zero, registers.a == 0x00);
	setFlag(Subtraction, false);
	setFlag(HalfCarry, true);
	setFlag(Carry, false);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::XORr()
{
	uint8_t opcode = read(pc);
	pc++;

	uint8_t d8;
	uint8_t hlValue;

	uint8_t* regPtr = nullptr;

	switch (opcode)
	{
		case 0xA8:
		{
			regPtr = &registers.b;
			break;
		}
		case 0xA9:
		{
			regPtr = &registers.c;
			break;
		}
		case 0xAA:
		{
			regPtr = &registers.d;
			break;
		}
		case 0xAB:
		{
			regPtr = &registers.e;
			break;
		}
		case 0xAC:
		{
			regPtr = &registers.h;
			break;
		}
		case 0xAD:
		{
			regPtr = &registers.l;
			break;
		}
		case 0xAE:
		{
			hlValue = read(registers.hl);

			regPtr = &hlValue;
			cycles += 4;
			cyclesRan += 4;
			break;
		}
		case 0xAF:
		{
			regPtr = &registers.a;
			break;
		}
		case 0xDE:
		{
			d8 = read(pc++);

			regPtr = &d8;

			cycles += 4;
			cyclesRan += 4;
			break;
		}
		default:
			std::cout << "Unknown XORr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	if (regPtr == nullptr)
	{
		return;
	}

	registers.a ^= (*regPtr);

	setFlag(Zero, registers.a == 0x00);
	setFlag(Subtraction, false);
	setFlag(HalfCarry, false);
	setFlag(Carry, false);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::ORr()
{
	uint8_t opcode = read(pc);
	pc++;

	uint8_t hlValue;
	uint8_t d8;

	uint8_t* regPtr = nullptr;

	switch (opcode)
	{
		case 0xB0:
		{
			regPtr = &registers.b;
			break;
		}
		case 0xB1:
		{
			regPtr = &registers.c;
			break;
		}
		case 0xB2:
		{
			regPtr = &registers.d;
			break;
		}
		case 0xB3:
		{
			regPtr = &registers.e;
			break;
		}
		case 0xB4:
		{
			regPtr = &registers.h;
			break;
		}
		case 0xB5:
		{
			regPtr = &registers.l;
			break;
		}
		case 0xB6:
		{
			hlValue = read(registers.hl);

			regPtr = &hlValue;
			cycles += 4;
			cyclesRan += 4;
			break;
		}
		case 0xB7:
		{
			regPtr = &registers.a;
			break;
		}
		case 0xF6:
		{
			d8 = read(pc++);
			regPtr = &d8;

			cycles += 4;
			cyclesRan += 4;
			break;
		}
		default:
			std::cout << "Unknown ORr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	registers.a |= (*regPtr);

	setFlag(Zero, registers.a == 0x00);
	setFlag(Subtraction, false);
	setFlag(HalfCarry, false);
	setFlag(Carry, false);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::CPr()
{
	uint8_t opcode = read(pc++);

	uint8_t d8;
	uint8_t hlValue;

	uint8_t* regPtr = nullptr;

	switch (opcode)
	{
		case 0xB8:
		{
			regPtr = &registers.b;
			break;
		}
		case 0xB9:
		{
			regPtr = &registers.c;
			break;
		}
		case 0xBA:
		{
			regPtr = &registers.d;
			break;
		}
		case 0xBB:
		{
			regPtr = &registers.e;
			break;
		}
		case 0xBC:
		{
			regPtr = &registers.h;
			break;
		}
		case 0xBD:
		{
			regPtr = &registers.l;
			break;
		}
		case 0xBE:
		{
			hlValue = registers.hl;
			regPtr = &hlValue;

			cycles += 4;
			cyclesRan += 4;
			break;
		}
		case 0xBF:
		{
			regPtr = &registers.a;
			break;
		}
		case 0xFE:
		{
			d8 = read(pc++);
			regPtr = &d8;

			cycles += 4;
			cyclesRan += 4;
			break;
		}
		default:
			std::cout << "Unknown CPr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	if (regPtr == nullptr)
	{
		return;
	}

	uint8_t result = registers.a - (*regPtr);

	setFlag(Zero, result == 0x00);
	setFlag(Subtraction, true);
	setFlag(HalfCarry, (registers.a & 0x0F) < (*regPtr & 0x0F));
	setFlag(Carry, (*regPtr) > registers.a);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::RETc()
{
	uint8_t opcode = read(pc);
	pc++;

	switch (opcode)
	{
		case 0xC0:
		{
			if (!isFlagSet(Zero))
			{
				uint8_t lo = read(stkp++);
				uint8_t hi = read(stkp++);

				pc = (hi << 8) | lo;

				cycles += 12;
				cyclesRan += 12;
			}
		}
		case 0xD0:
		{
			if (!isFlagSet(Carry))
			{
				uint8_t lo = read(stkp++);
				uint8_t hi = read(stkp++);

				pc = (hi << 8) | lo;

				cycles += 12;
				cyclesRan += 12;
			}

			break;
		}
		case 0xC8:
		{
			if (isFlagSet(Zero))
			{
				uint8_t lo = read(stkp++);
				uint8_t hi = read(stkp++);

				pc = (hi << 8) | lo;

				cycles += 12;
				cyclesRan += 12;
			}

			break;
		}
		case 0xC9:
		{
			uint8_t lo = read(stkp++);
			uint8_t hi = read(stkp++);

			pc = (hi << 8) | lo;

			cycles += 8;
			cyclesRan += 8;

			break;
		}
		case 0xD8:
		{
			if (isFlagSet(Zero))
			{
				uint8_t lo = read(stkp++);
				uint8_t hi = read(stkp++);

				pc = (hi << 8) | lo;

				cycles += 12;
				cyclesRan += 12;
			}

			break;
		}
		case 0xD9:
		{
			uint8_t lo = read(stkp++);
			uint8_t hi = read(stkp++);

			pc = (hi << 8) | lo;
			ime = 1;
			cycles += 8;
			cyclesRan += 8;

			break;
		}
		default:
			std::cout << "Unknown ORr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	cycles += 8;
	cyclesRan += 8;
}

void Cpu::POPrr()
{
	uint8_t opcode = read(pc);
	pc++;

	uint8_t lo = read(stkp++);
	uint8_t hi = read(stkp++);

	switch (opcode)
	{
		case 0xC1: // POP BC
		{
			registers.bc = (hi << 8) | lo;
			break;
		}
		case 0xD1: // POP DE
		{
			registers.de = (hi << 8) | lo;
			break;
		}
		case 0xE1: // POP HL
		{
			registers.hl = (hi << 8) | lo;
			break;
		}
		case 0xF1: // POP AF
		{
			registers.af = (hi << 8) | lo;
			break;
		}
		default:
			std::cout << "Unknown POPrr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	cycles += 12;
	cyclesRan += 12;
}

void Cpu::JPca16()
{
	uint8_t opcode = read(pc);
	pc++;

	uint8_t lo = read(pc);
	pc++;
	uint8_t hi = read(pc);
	pc++;

	switch (opcode)
	{
		case 0xC2: // JP NZ,a16
		{
			if (!isFlagSet(Zero))
			{
				pc = (hi << 8) | lo;
				cycles += 4;
				cyclesRan += 4;
			}

			break;
		}
		case 0xC3: // JP a16
		{
			pc = (hi << 8) | lo;
			cycles += 4;
			cyclesRan += 4;

			break;
		}
		case 0xCA: // JP Z,a16
		{
			if (isFlagSet(Zero))
			{
				pc = (hi << 8) | lo;
				cycles += 4;
				cyclesRan += 4;
			}

			break;
		}
		case 0xD2: // JP NC,a16
		{
			if (!isFlagSet(Carry))
			{
				pc = (hi << 8) | lo;
				cycles += 4;
				cyclesRan += 4;
			}

			break;
		}
		case 0xDA: // JP C,a16
		{
			if (isFlagSet(Carry))
			{
				pc = (hi << 8) | lo;
				cycles += 4;
				cyclesRan += 4;
			}

			break;
		}
		default:
			std::cout << "Unknown JPca16 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	cycles += 12;
	cyclesRan += 12;
}

void Cpu::JPaHL()
{
	uint8_t opcode = read(pc++);

	switch (opcode)
	{
		case 0xE9:
		{
			pc = registers.hl;
		}
		default:
			std::cout << "Unknown JPaHL instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::CALLca16()
{
	uint8_t opcode = read(pc++);

	uint8_t lo = read(pc++);

	uint8_t hi = read(pc++);

	uint16_t nn = (hi << 8) | lo;

	switch (opcode)
	{
		case 0xC4: // CALL NZ, a16
		{
			if (!isFlagSet(Zero))
			{
				write(--stkp, (pc >> 8) & 0xFF);
				write(--stkp, pc & 0xFF);

				pc = nn;

				cycles += 12;
				cyclesRan += 12;
			}

			break;
		}
		case 0xCC: // CALL Z, a16
		{
			if (isFlagSet(Zero))
			{
				write(--stkp, (pc >> 8) & 0xFF);
				write(--stkp, pc & 0xFF);

				pc = nn;

				cycles += 12;
				cyclesRan += 12;
			}

			break;
		}
		case 0xCD: // CALL a16
		{
			write(--stkp, (pc >> 8) & 0xFF);
			write(--stkp, pc & 0xFF);

			pc = nn;

			cycles += 12;
			cyclesRan += 12;

			break;
		}
		case 0xD4: // CALL NC, a16
		{
			if (!isFlagSet(Carry))
			{
				write(--stkp, (pc >> 8) & 0xFF);
				write(--stkp, pc & 0xFF);

				pc = nn;

				cycles += 12;
				cyclesRan += 12;
			}

			break;
		}
		case 0xDC: // CALL C, a16
		{
			if (isFlagSet(Carry))
			{
				write(--stkp, (pc >> 8) & 0xFF);
				write(--stkp, pc & 0xFF);

				pc = nn;

				cycles += 12;
				cyclesRan += 12;
			}

			break;
		}
		default:
			std::cout << "Unknown CALLca16 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	cycles += 12;
	cyclesRan += 12;
}

void Cpu::PUSHrr()
{
	uint8_t opcode = read(pc++);

	switch (opcode)
	{
		case 0xC5:
		{
			write(--stkp, (registers.bc >> 8) & 0xFF);
			write(--stkp, registers.bc & 0xFF);

			break;
		}
		case 0xD5:
		{
			write(--stkp, (registers.de >> 8) & 0xFF);
			write(--stkp, registers.de & 0xFF);

			break;
		}
		case 0xE5:
		{
			write(--stkp, (registers.hl >> 8) & 0xFF);
			write(--stkp, registers.hl & 0xFF);

			break;
		}
		case 0xF5:
		{
			write(--stkp, (registers.af >> 8) & 0xFF);
			write(--stkp, registers.af & 0xFF);

			break;
		}
		default:
			std::cout << "Unknown PUSHrr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	cycles += 16;
	cyclesRan += 16;
}

void Cpu::RSTn()
{
	uint8_t opcode = read(pc++);
	uint8_t n;
	switch (opcode)
	{
		case 0xC7:
		{
			n = 0x00;
			break;
		}
		case 0xCF:
		{
			n = 0x08;
			break;
		}
		case 0xD7:
		{
			n = 0x10;
			break;
		}
		case 0xDF:
		{
			n = 0x18;
			break;
		}
		case 0xE7:
		{
			n = 0x20;
			break;
		}
		case 0xEF:
		{
			n = 0x28;
			break;
		}
		case 0xF7:
		{
			n = 0x30;
			break;
		}
		case 0xFF:
		{
			n = 0x38;
			break;
		}
		default:
			std::cout << "Unknown RSTn instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	write(--stkp, (pc >> 8) & 0xFF);
	write(--stkp, pc & 0xFF);

	pc = (0x00 << 8) | n;

	cycles += 16;
	cyclesRan += 16;
}

void Cpu::LDH()
{
	uint8_t opcode = read(pc++);
	uint16_t a8 = (0xFF << 8) | read(pc++);

	switch (opcode)
	{
		case 0xE0: // LDH (a8), A
		{
			write(a8, registers.a);
			break;
		}
		case 0xF0: // LDH A, (a8)
		{
			registers.a = read(a8);
			break;
		}
		default:
			std::cout << "Unknown LDH instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	cycles += 12;
	cyclesRan += 12;
}

void Cpu::LD_C_A()
{
	uint8_t opcode = read(pc++);

	switch (opcode)
	{
	case 0xE2: // LD (C),A
	{
		write(0xFF00 + registers.c, registers.a);
		break;
	}
	case 0xF2: // LD A,(C)
	{
		registers.a = read(0xFF00 + registers.c);
		break;
	}
	default:
		std::cout << "Unknown LD (C),A instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		setHasNotBroken(false);
		break;
	}

	cycles += 8;
	cyclesRan += 8;
}

void Cpu::XXX()
{
	uint8_t opcode = read(pc++);

	std::cout << "Unknown/Illegal instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
	setHasNotBroken(false);
}

void Cpu::LDa16SP()
{
	uint8_t opcode = read(pc++);

	uint8_t lo = read(pc++);
	uint8_t hi = read(pc++);

	uint16_t a16 = (hi << 8) | lo;

	write(a16, stkp & 0xFF);
	write(a16 + 1, (stkp >> 8) & 0xFF);

	cycles += 20;
	cyclesRan += 20;
}

void Cpu::JRcr8()
{
	uint8_t opcode = read(pc++);
	int8_t offset = static_cast<int8_t>(read(pc++));

	switch (opcode)
	{
		case 0x20:
		{
			if (!isFlagSet(Zero))
			{
				uint16_t targetAddress = static_cast<uint16_t>(pc) + offset;

				pc = targetAddress;
				cycles += 4;
				cyclesRan += 4;
			}

			break;
		}
		case 0x30:
		{
			if (!isFlagSet(Carry))
			{
				uint16_t targetAddress = static_cast<uint16_t>(pc) + offset;

				pc = targetAddress;
				cycles += 4;
				cyclesRan += 4;
			}

			break;
		}
		case 0x18:
		{
			uint16_t targetAddress = static_cast<uint16_t>(pc) + offset;

			pc = targetAddress;
			cycles += 4;
			cyclesRan += 4;

			break;
		}
		case 0x28:
		{
			if (isFlagSet(Zero))
			{
				uint16_t targetAddress = static_cast<uint16_t>(pc) + offset;

				pc = targetAddress;
				cycles += 4;
				cyclesRan += 4;
			}

			break;
		}
		case 0x38:
		{
			if (isFlagSet(Carry))
			{
				uint16_t targetAddress = static_cast<uint16_t>(pc) + offset;

				pc = targetAddress;
				cycles += 4;
				cyclesRan += 4;
			}

			break;
		}
		default:
			std::cout << "Unknown JRcr8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	cycles += 8;
	cyclesRan += 8;
}

void Cpu::DI()
{
	uint8_t opcode = read(pc++);

	ime = 0x00;

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::LDa16A()
{
	uint8_t opcode = read(pc++);

	uint8_t lo = read(pc++);
	uint8_t hi = read(pc++);

	switch (opcode)
	{
		case 0xEA: // LD (a16), A
		{
			write((hi << 8) | lo, registers.a);
			break;
		}
		case 0xFA: // LD A, (a16)
		{
			registers.a = read((hi << 8) | lo);
			break;
		}
		default:
			std::cout << "Unknown LDa16A instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	cycles += 16;
	cyclesRan += 16;
}
