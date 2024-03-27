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
	vBlankIRQ = 0xFF85;

	ime = false;
	IE = 0xFFFF;
	IF = 0xFF0F;

	write(lcdc, 0x91);
	write(stat, 0x85);
	tileMapAddress = 0x9800;

	using a = Cpu;

	lookup = {
		INSTRUCTION{ "NOP", &a::NOP }, INSTRUCTION{ "LD BC, d16", &a::LDrrd16 }, INSTRUCTION{ "LD (BC), A", &a::LDarrR }, INSTRUCTION{ "INC BC", &a::INCrr }, INSTRUCTION{ "INC B", &a::INCr }, INSTRUCTION{ "DEC B", &a::DECr }, INSTRUCTION{ "LD B, d8", &a::LDrd8 }, INSTRUCTION{ "RLCA", &a::RLCA }, INSTRUCTION{ "LD (a16), SP", &a::LDa16SP }, INSTRUCTION{ "ADD HL, BC", &a::ADDrrRR }, INSTRUCTION{ "LD A, (BC)", &a::LDraRR }, INSTRUCTION{ "DEC BC", &a::DECrr }, INSTRUCTION{ "INC C", &a::INCr }, INSTRUCTION{ "DEC C", &a::DECr }, INSTRUCTION{ "LD C, d8", &a::LDrd8 }, INSTRUCTION{ "RRCA", &a::RRCA },
		INSTRUCTION{ "STOP 0; UNIMPLEMENTED", &a::XXX }, INSTRUCTION{ "LD DE, d16", &a::LDrrd16 }, INSTRUCTION{ "LD (DE), d16", &a::LDarrR }, INSTRUCTION{ "INC DE", &a::INCrr }, INSTRUCTION{ "INC D", &a::INCr }, INSTRUCTION{ "DEC D", &a::DECr }, INSTRUCTION{ "LD D, d8", &a::LDrd8 }, INSTRUCTION{ "RLA", &a::RLA }, INSTRUCTION{ "JR r8", &a::JRcr8 }, INSTRUCTION{ "ADD HL, BC", &a::ADDrrRR }, INSTRUCTION{ "LD A, (DE)", &a::LDraRR }, INSTRUCTION{ "DEC DE", &a::DECrr }, INSTRUCTION{ "INC E", &a::INCr }, INSTRUCTION{ "DEC E", &a::DECr }, INSTRUCTION{ "LD E, d8", &a::LDrd8 }, INSTRUCTION{ "RRA", &a::RRA },
		INSTRUCTION{ "JR NZ, r8", &a::JRcr8 }, INSTRUCTION{ "LD HL, d16", &a::LDrrd16 }, INSTRUCTION{ "LD (HL+), d16", &a::LDarrR }, INSTRUCTION{ "INC HL", &a::INCrr }, INSTRUCTION{ "INC H", &a::INCr }, INSTRUCTION{ "DEC H", &a::DECr }, INSTRUCTION{ "LD H, d8", &a::LDrd8 }, INSTRUCTION{ "DAA", &a::DAA }, INSTRUCTION{ "JR Z r8", &a::JRcr8 }, INSTRUCTION{ "ADD HL, HL", &a::ADDrrRR }, INSTRUCTION{ "LD A, (HL+)", &a::LDraRR }, INSTRUCTION{ "DEC HL", &a::DECrr }, INSTRUCTION{ "INC L", &a::INCr }, INSTRUCTION{ "DEC L", &a::DECr }, INSTRUCTION{ "LD L, d8", &a::LDrd8 }, INSTRUCTION{ "CPL", &a::CPL },
		INSTRUCTION{ "JR NC, r8", &a::JRcr8 }, INSTRUCTION{ "LD SP, d16", &a::LDrrd16 }, INSTRUCTION{ "LD (HL-), d16", &a::LDarrR }, INSTRUCTION{ "INC SP", &a::INCrr }, INSTRUCTION{ "INC (HL)", &a::INCHL }, INSTRUCTION{ "DEC (HL)", &a::DECHL }, INSTRUCTION{ "LD (HL), d8", &a::LDrd8 }, INSTRUCTION{ "SCF", &a::SCF }, INSTRUCTION{ "JR C r8", &a::JRcr8 }, INSTRUCTION{ "ADD HL, SP", &a::ADDrrRR }, INSTRUCTION{ "LD A, (HL-)", &a::LDraRR }, INSTRUCTION{ "DEC SP", &a::DECrr }, INSTRUCTION{ "INC A", &a::INCr }, INSTRUCTION{ "DEC A", &a::DECr }, INSTRUCTION{ "LD A, d8", &a::LDrd8 }, INSTRUCTION{ "CCF", &a::CCF },
		INSTRUCTION{ "LD B, B", &a::LDrR }, INSTRUCTION{ "LD B, C", &a::LDrR }, INSTRUCTION{ "LD B, D", &a::LDrR }, INSTRUCTION{ "LD B, E", &a::LDrR }, INSTRUCTION{ "LD B, H", &a::LDrR }, INSTRUCTION{ "LD B, L", &a::LDrR }, INSTRUCTION{ "LD B, (HL)", &a::LDrR }, INSTRUCTION{ "LD B, A", &a::LDrR }, INSTRUCTION{ "LD C, B", &a::LDrR }, INSTRUCTION{ "LD C, C", &a::LDrR }, INSTRUCTION{ "LD C, D", &a::LDrR }, INSTRUCTION{ "LD C, E", &a::LDrR }, INSTRUCTION{ "LD C, H", &a::LDrR }, INSTRUCTION{ "LD C, L", &a::LDrR }, INSTRUCTION{ "LD C, (HL)", &a::LDrR }, INSTRUCTION{ "LD C, A", &a::LDrR },
		INSTRUCTION{ "LD D, B", &a::LDrR }, INSTRUCTION{ "LD D, C", &a::LDrR }, INSTRUCTION{ "LD D, D", &a::LDrR }, INSTRUCTION{ "LD D, E", &a::LDrR }, INSTRUCTION{ "LD D, H", &a::LDrR }, INSTRUCTION{ "LD D, L", &a::LDrR }, INSTRUCTION{ "LD D, (HL)", &a::LDrR }, INSTRUCTION{ "LD D, A", &a::LDrR }, INSTRUCTION{ "LD E, B", &a::LDrR }, INSTRUCTION{ "LD E, C", &a::LDrR }, INSTRUCTION{ "LD E, D", &a::LDrR }, INSTRUCTION{ "LD E, E", &a::LDrR }, INSTRUCTION{ "LD E, H", &a::LDrR }, INSTRUCTION{ "LD E, L", &a::LDrR }, INSTRUCTION{ "LD E, (HL)", &a::LDrR }, INSTRUCTION{ "LD E, A", &a::LDrR },
		INSTRUCTION{ "LD H, B", &a::LDrR }, INSTRUCTION{ "LD H, C", &a::LDrR }, INSTRUCTION{ "LD H, D", &a::LDrR }, INSTRUCTION{ "LD H, E", &a::LDrR }, INSTRUCTION{ "LD H, H", &a::LDrR }, INSTRUCTION{ "LD H, L", &a::LDrR }, INSTRUCTION{ "LD H, (HL)", &a::LDrR }, INSTRUCTION{ "LD H, A", &a::LDrR }, INSTRUCTION{ "LD L, B", &a::LDrR }, INSTRUCTION{ "LD L, C", &a::LDrR }, INSTRUCTION{ "LD L, D", &a::LDrR }, INSTRUCTION{ "LD L, E", &a::LDrR }, INSTRUCTION{ "LD L, H", &a::LDrR }, INSTRUCTION{ "LD L, L", &a::LDrR }, INSTRUCTION{ "LD L, (HL)", &a::LDrR }, INSTRUCTION{ "LD L, A", &a::LDrR },
		INSTRUCTION{ "LD (HL), B", &a::LDarrR }, INSTRUCTION{ "LD (HL), C", &a::LDarrR }, INSTRUCTION{ "LD (HL), D", &a::LDarrR }, INSTRUCTION{ "LD (HL), E", &a::LDarrR }, INSTRUCTION{ "LD (HL), H", &a::LDarrR }, INSTRUCTION{ "LD (HL), L", &a::LDarrR }, INSTRUCTION{ "HALT; UNIMPLEMENTED", &a::XXX }, INSTRUCTION{ "LD (HL), A", &a::LDarrR }, INSTRUCTION{ "LD A, B", &a::LDrR }, INSTRUCTION{ "LD A, C", &a::LDrR }, INSTRUCTION{ "LD A, D", &a::LDrR }, INSTRUCTION{ "LD A, E", &a::LDrR }, INSTRUCTION{ "LD A, H", &a::LDrR }, INSTRUCTION{ "LD A, L", &a::LDrR }, INSTRUCTION{ "LD A, (HL)", &a::LDrR }, INSTRUCTION{ "LD A, A", &a::LDrR },
		INSTRUCTION{ "ADD A, B", &a::ADDrR }, INSTRUCTION{ "ADD A, C", &a::ADDrR }, INSTRUCTION{ "ADD A, D", &a::ADDrR }, INSTRUCTION{ "ADD A, E", &a::ADDrR }, INSTRUCTION{ "ADD A, H", &a::ADDrR }, INSTRUCTION{ "ADD A, L", &a::ADDrR }, INSTRUCTION{ "ADD A, (HL)", &a::ADDrR }, INSTRUCTION{ "ADD A, A", &a::ADDrR }, INSTRUCTION{ "ADC A, B", &a::ADDrR }, INSTRUCTION{ "ADC A, C", &a::ADDrR }, INSTRUCTION{ "ADC A, D", &a::ADDrR }, INSTRUCTION{ "ADC A, E", &a::ADDrR }, INSTRUCTION{ "ADC A, H", &a::ADDrR }, INSTRUCTION{ "ADC A, L", &a::ADDrR }, INSTRUCTION{ "ADC A, (HL)", &a::ADDrR }, INSTRUCTION{ "ADC A, A", &a::ADDrR },
		INSTRUCTION{ "SUB B" , &a::SUBr }, INSTRUCTION{ "SUB C", &a::SUBr }, INSTRUCTION{ "SUB D", &a::SUBr }, INSTRUCTION{ "SUB E", &a::SUBr }, INSTRUCTION{ "SUB H", &a::SUBr }, INSTRUCTION{ "SUB L", &a::SUBr }, INSTRUCTION{ "SUB (HL)", &a::SUBr }, INSTRUCTION{ "SUB A", &a::SUBr }, INSTRUCTION{ "SBC A, B", &a::SUBr }, INSTRUCTION{ "SBC A, C", &a::SUBr }, INSTRUCTION{ "SBC A, D", &a::SUBr }, INSTRUCTION{ "SBC A, E", &a::SUBr }, INSTRUCTION{ "SBC A, H", &a::SUBr }, INSTRUCTION{ "SBC A, L", &a::SUBr }, INSTRUCTION{ "SBC A, (HL)", &a::SUBr }, INSTRUCTION{ "SBC A, A", &a::SUBr },
		INSTRUCTION{ "AND B", &a::ANDr }, INSTRUCTION{ "AND C", &a::ANDr }, INSTRUCTION{ "AND D", &a::ANDr }, INSTRUCTION{ "AND E", &a::ANDr }, INSTRUCTION{ "AND H", &a::ANDr }, INSTRUCTION{ "AND L", &a::ANDr }, INSTRUCTION{ "AND (HL)", &a::ANDr }, INSTRUCTION{ "AND A", &a::ANDr }, INSTRUCTION{ "XOR B", &a::XORr }, INSTRUCTION{ "XOR C", &a::XORr }, INSTRUCTION{ "XOR D", &a::XORr }, INSTRUCTION{ "XOR E", &a::XORr }, INSTRUCTION{ "XOR H", &a::XORr }, INSTRUCTION{ "XOR L", &a::XORr }, INSTRUCTION{ "XOR (HL)", &a::XORr }, INSTRUCTION{ "XOR A", &a::XORr },
		INSTRUCTION{ "OR B", &a::ORr }, INSTRUCTION{ "OR C", &a::ORr }, INSTRUCTION{ "OR D", &a::ORr }, INSTRUCTION{ "OR E", &a::ORr }, INSTRUCTION{ "OR H", &a::ORr }, INSTRUCTION{ "OR L", &a::ORr }, INSTRUCTION{ "OR (HL)", &a::ORr }, INSTRUCTION{ "OR A", &a::ORr }, INSTRUCTION{ "CP B", &a::CPr }, INSTRUCTION{ "CP C", &a::CPr }, INSTRUCTION{ "CP D", &a::CPr }, INSTRUCTION{ "CP E", &a::CPr }, INSTRUCTION{ "CP H", &a::CPr }, INSTRUCTION{ "CP L", &a::CPr }, INSTRUCTION{ "CP (HL)", &a::CPr }, INSTRUCTION{ "CP A", &a::CPr },
		INSTRUCTION{ "RET NZ", &a::RETc }, INSTRUCTION{ "POP BC", &a::POPrr }, INSTRUCTION{ "JP NZ, a16", &a::JPca16 }, INSTRUCTION{ "JP a16", &a::JPca16 }, INSTRUCTION{ "CALL NZ, a16", &a::CALLca16 }, INSTRUCTION{ "PUSH BC", &a::PUSHrr }, INSTRUCTION{ "ADD A, d8", &a::ADDrR }, INSTRUCTION{ "RST 00H", &a::RSTn }, INSTRUCTION{ "RET Z", &a::RETc }, INSTRUCTION{ "RET", &a::RETc }, INSTRUCTION{ "JP Z, a16", &a::JPca16 }, INSTRUCTION{ "PREFIX CB", &a::PREFIXCB }, INSTRUCTION{ "CALL Z, a16", &a::CALLca16 }, INSTRUCTION{ "CALL a16", &a::CALLca16 }, INSTRUCTION{ "ADC A, d8", &a::ADDrR }, INSTRUCTION{ "RST 08H", &a::RSTn },
		INSTRUCTION{ "RET NC", &a::RETc }, INSTRUCTION{ "POP DE", &a::POPrr }, INSTRUCTION{ "JP NC, a16", &a::JPca16 }, INSTRUCTION{ "ILLEGAL", &a::XXX }, INSTRUCTION{ "CALL NC, a16", &a::CALLca16 }, INSTRUCTION{ "PUSH DE", &a::PUSHrr }, INSTRUCTION{ "SUB d8", &a::SUBr }, INSTRUCTION{ "RST 10H", &a::RSTn }, INSTRUCTION{ "RET C", &a::RETc }, INSTRUCTION{ "RETI", &a::RETI }, INSTRUCTION{ "JP C, a16", &a::JPca16 }, INSTRUCTION{ "ILLEGAL", &a::XXX }, INSTRUCTION{ "CALL C, a16", &a::CALLca16 }, INSTRUCTION{ "ILLEGAL", &a::XXX }, INSTRUCTION{ "SBC A, d8", &a::SUBr }, INSTRUCTION{ "RST 18H", &a::RSTn },
		INSTRUCTION{ "LDH (a8), A", &a::LDH }, INSTRUCTION{ "POP HL", &a::POPrr }, INSTRUCTION{ "LD (C), A", &a::LD_C_A }, INSTRUCTION{ "ILLEGAL", &a::XXX }, INSTRUCTION{ "ILLEGAL", &a::XXX }, INSTRUCTION{ "PUSH HL", &a::PUSHrr }, INSTRUCTION{ "AND d8", &a::ANDr }, INSTRUCTION{ "RST 20H", &a::RSTn }, INSTRUCTION{ "ADD SP, r8", &a::ADDSPr8 }, INSTRUCTION{ "JP (HL)", &a::JPaHL }, INSTRUCTION{ "LD (a16), A", &a::LDa16A }, INSTRUCTION{ "ILLEGAL", &a::XXX }, INSTRUCTION{ "ILLEGAL", &a::XXX }, INSTRUCTION{ "ILLEGAL", &a::XXX }, INSTRUCTION{ "XOR d8", &a::XORr }, INSTRUCTION{ "RST 28H", &a::RSTn },
		INSTRUCTION{ "LDH A, (a8)", &a::LDH }, INSTRUCTION{ "POP AF", &a::POPAF }, INSTRUCTION{ "LD A, (C)", &a::LD_C_A }, INSTRUCTION{ "DI", &a::DI }, INSTRUCTION{ "ILLEGAL", &a::XXX }, INSTRUCTION{ "PUSH AF", &a::PUSHAF }, INSTRUCTION{ "OR d8", &a::ORr }, INSTRUCTION{ "RST 30H", &a::RSTn }, INSTRUCTION{ "LD HL, SP+r8", &a::LDHLSPr8 }, INSTRUCTION{ "LD SP, HL", &a::LDSPHL }, INSTRUCTION{ "LD A, (a16)", &a::LDa16A }, INSTRUCTION{ "EI", &a::EI }, INSTRUCTION{ "ILLEGAL", &a::XXX }, INSTRUCTION{ "ILLEGAL", &a::XXX }, INSTRUCTION{ "CP d8", &a::CPr }, INSTRUCTION{ "RST 38H", &a::RSTn }
	};

	lookupCB = {
		INSTRUCTION{ "RLC B", &a::RLCr8 }, INSTRUCTION{ "RLC C", &a::RLCr8 }, INSTRUCTION{ "RLC D", &a::RLCr8 }, INSTRUCTION{ "RLC E", &a::RLCr8 }, INSTRUCTION{ "RLC H", &a::RLCr8 }, INSTRUCTION{ "RLC L", &a::RLCr8 }, INSTRUCTION{ "RLC (HL)", &a::RLCr8 }, INSTRUCTION{ "RLC A", &a::RLCr8 }, INSTRUCTION{ "RRC B", &a::RRCr8 }, INSTRUCTION{ "RRC C", &a::RRCr8 }, INSTRUCTION{ "RRC D", &a::RRCr8 }, INSTRUCTION{ "RRC E", &a::RRCr8 }, INSTRUCTION{ "RRC H", &a::RRCr8 }, INSTRUCTION{ "RRC L", &a::RRCr8 }, INSTRUCTION{ "RRC (HL)", &a::RRCr8 }, INSTRUCTION{ "RRC A", &a::RRCr8 },
		INSTRUCTION{ "RL B", &a::RLr8 }, INSTRUCTION{ "RL C", &a::RLr8 }, INSTRUCTION{ "RL D", &a::RLr8 }, INSTRUCTION{ "RL E", &a::RLr8 }, INSTRUCTION{ "RL H", &a::RLr8 }, INSTRUCTION{ "RL L", &a::RLr8 }, INSTRUCTION{ "RL (HL)", &a::RLr8 }, INSTRUCTION{ "RL A", &a::RLr8 }, INSTRUCTION{ "RR B", &a::RRr8 }, INSTRUCTION{ "RR C", &a::RRr8 }, INSTRUCTION{ "RR D", &a::RRr8 }, INSTRUCTION{ "RR E", &a::RRr8 }, INSTRUCTION{ "RR H", &a::RRr8 }, INSTRUCTION{ "RR L", &a::RRr8 }, INSTRUCTION{ "RR (HL)", &a::RRr8 }, INSTRUCTION{ "RR A", &a::RRr8 },
		INSTRUCTION{ "SLA B", &a::SLAr8 }, INSTRUCTION{ "SLA C", &a::SLAr8 }, INSTRUCTION{ "SLA D", &a::SLAr8 }, INSTRUCTION{ "SLA E", &a::SLAr8 }, INSTRUCTION{ "SLA H", &a::SLAr8 }, INSTRUCTION{ "SLA L", &a::SLAr8 }, INSTRUCTION{ "SLA (HL)", &a::SLAr8 }, INSTRUCTION{ "SLA A", &a::SLAr8 }, INSTRUCTION{ "SRA B", &a::SRAr8 }, INSTRUCTION{ "SRA C", &a::SRAr8 }, INSTRUCTION{ "SRA D", &a::SRAr8 }, INSTRUCTION{ "SRA E", &a::SRAr8 }, INSTRUCTION{ "SRA H", &a::SRAr8 }, INSTRUCTION{ "SRA L", &a::SRAr8 }, INSTRUCTION{ "SRA (HL)", &a::SRAr8 }, INSTRUCTION{ "SLA A", &a::SRAr8 },
		INSTRUCTION{ "SWAP B", &a::SWAPr8 }, INSTRUCTION{ "SWAP C", &a::SWAPr8 }, INSTRUCTION{ "SWAP D", &a::SWAPr8 }, INSTRUCTION{ "SWAP E", &a::SWAPr8 }, INSTRUCTION{ "SWAP H", &a::SWAPr8 }, INSTRUCTION{ "SWAP L", &a::SWAPr8 }, INSTRUCTION{ "SWAP (HL)", &a::SWAPr8 }, INSTRUCTION{ "SWAP A", &a::SWAPr8 }, INSTRUCTION{ "SRL B", &a::SRLr8 }, INSTRUCTION{ "SRL C", &a::SRLr8 }, INSTRUCTION{ "SRL D", &a::SRLr8 }, INSTRUCTION{ "SRL E", &a::SRLr8 }, INSTRUCTION{ "SRL H", &a::SRLr8 }, INSTRUCTION{ "SRL L", &a::SRLr8 }, INSTRUCTION{ "SRL (HL)", &a::SRLr8 }, INSTRUCTION{ "SRL A", &a::SRLr8 },
		INSTRUCTION{ "BIT 0, B", &a::BIT0r8 }, INSTRUCTION{ "BIT 0, C", &a::BIT0r8 }, INSTRUCTION{ "BIT 0, D", &a::BIT0r8 }, INSTRUCTION{ "BIT 0, E", &a::BIT0r8 }, INSTRUCTION{ "BIT 0, H", &a::BIT0r8 }, INSTRUCTION{ "BIT 0, L", &a::BIT0r8 }, INSTRUCTION{ "BIT 0, (HL)", &a::BIT0r8 }, INSTRUCTION{ "BIT 0, A", &a::BIT0r8 }, INSTRUCTION{ "BIT 1, B", &a::BIT1r8 }, INSTRUCTION{ "BIT 1, C", &a::BIT1r8 }, INSTRUCTION{ "BIT 1, D", &a::BIT1r8 }, INSTRUCTION{ "BIT 1, E", &a::BIT1r8 }, INSTRUCTION{ "BIT 1, H", &a::BIT1r8 }, INSTRUCTION{ "BIT 1, L", &a::BIT1r8 }, INSTRUCTION{ "BIT 1, (HL)", &a::BIT1r8 }, INSTRUCTION{ "BIT 1, A", &a::BIT1r8 },
		INSTRUCTION{ "BIT 2, B", &a::BIT2r8 }, INSTRUCTION{ "BIT 2, C", &a::BIT2r8 }, INSTRUCTION{ "BIT 2, D", &a::BIT2r8 }, INSTRUCTION{ "BIT 2, E", &a::BIT2r8 }, INSTRUCTION{ "BIT 2, H", &a::BIT2r8 }, INSTRUCTION{ "BIT 2, L", &a::BIT2r8 }, INSTRUCTION{ "BIT 2, (HL)", &a::BIT2r8 }, INSTRUCTION{ "BIT 2, A", &a::BIT2r8 }, INSTRUCTION{ "BIT 3, B", &a::BIT3r8 }, INSTRUCTION{ "BIT 3, C", &a::BIT3r8 }, INSTRUCTION{ "BIT 3, D", &a::BIT3r8 }, INSTRUCTION{ "BIT 3, E", &a::BIT3r8 }, INSTRUCTION{ "BIT 3, H", &a::BIT3r8 }, INSTRUCTION{ "BIT 3, L", &a::BIT3r8 }, INSTRUCTION{ "BIT 3, (HL)", &a::BIT3r8 }, INSTRUCTION{ "BIT 3, A", &a::BIT3r8 },
		INSTRUCTION{ "BIT 4, B", &a::BIT4r8 }, INSTRUCTION{ "BIT 4, C", &a::BIT4r8 }, INSTRUCTION{ "BIT 4, D", &a::BIT4r8 }, INSTRUCTION{ "BIT 4, E", &a::BIT4r8 }, INSTRUCTION{ "BIT 4, H", &a::BIT4r8 }, INSTRUCTION{ "BIT 4, L", &a::BIT4r8 }, INSTRUCTION{ "BIT 4, (HL)", &a::BIT4r8 }, INSTRUCTION{ "BIT 4, A", &a::BIT4r8 }, INSTRUCTION{ "BIT 5, B", &a::BIT5r8 }, INSTRUCTION{ "BIT 5, C", &a::BIT5r8 }, INSTRUCTION{ "BIT 5, D", &a::BIT5r8 }, INSTRUCTION{ "BIT 5, E", &a::BIT5r8 }, INSTRUCTION{ "BIT 5, H", &a::BIT5r8 }, INSTRUCTION{ "BIT 5, L", &a::BIT5r8 }, INSTRUCTION{ "BIT 5, (HL)", &a::BIT5r8 }, INSTRUCTION{ "BIT 5, A", &a::BIT5r8 },
		INSTRUCTION{ "BIT 6, B", &a::BIT6r8 }, INSTRUCTION{ "BIT 6, C", &a::BIT6r8 }, INSTRUCTION{ "BIT 6, D", &a::BIT6r8 }, INSTRUCTION{ "BIT 6, E", &a::BIT6r8 }, INSTRUCTION{ "BIT 6, H", &a::BIT6r8 }, INSTRUCTION{ "BIT 6, L", &a::BIT6r8 }, INSTRUCTION{ "BIT 6, (HL)", &a::BIT6r8 }, INSTRUCTION{ "BIT 6, A", &a::BIT6r8 }, INSTRUCTION{ "BIT 7, B", &a::BIT7r8 }, INSTRUCTION{ "BIT 7, C", &a::BIT7r8 }, INSTRUCTION{ "BIT 7, D", &a::BIT7r8 }, INSTRUCTION{ "BIT 7, E", &a::BIT7r8 }, INSTRUCTION{ "BIT 7, H", &a::BIT7r8 }, INSTRUCTION{ "BIT 7, L", &a::BIT7r8 }, INSTRUCTION{ "BIT 7, (HL)", &a::BIT7r8 }, INSTRUCTION{ "BIT 7, A", &a::BIT7r8 },
		INSTRUCTION{ "RES 0, B", &a::RES0r8 }, INSTRUCTION{ "RES 0, C", &a::RES0r8 }, INSTRUCTION{ "RES 0, D", &a::RES0r8 }, INSTRUCTION{ "RES 0, E", &a::RES0r8 }, INSTRUCTION{ "RES 0, H", &a::RES0r8 }, INSTRUCTION{ "RES 0, L", &a::RES0r8 }, INSTRUCTION{ "RES 0, (HL)", &a::RES0r8 }, INSTRUCTION{ "RES 0, A", &a::RES0r8 }, INSTRUCTION{ "RES 1, B", &a::RES1r8 }, INSTRUCTION{ "RES 1, C", &a::RES1r8 }, INSTRUCTION{ "RES 1, D", &a::RES1r8 }, INSTRUCTION{ "RES 1, E", &a::RES1r8 }, INSTRUCTION{ "RES 1, H", &a::RES1r8 }, INSTRUCTION{ "RES 1, L", &a::RES1r8 }, INSTRUCTION{ "RES 1, (HL)", &a::RES1r8 }, INSTRUCTION{ "RES 1, A", &a::RES1r8 },
		INSTRUCTION{ "RES 2, B", &a::RES2r8 }, INSTRUCTION{ "RES 2, C", &a::RES2r8 }, INSTRUCTION{ "RES 2, D", &a::RES2r8 }, INSTRUCTION{ "RES 2, E", &a::RES2r8 }, INSTRUCTION{ "RES 2, H", &a::RES2r8 }, INSTRUCTION{ "RES 2, L", &a::RES2r8 }, INSTRUCTION{ "RES 2, (HL)", &a::RES2r8 }, INSTRUCTION{ "RES 2, A", &a::RES2r8 }, INSTRUCTION{ "RES 3, B", &a::RES3r8 }, INSTRUCTION{ "RES 3, C", &a::RES3r8 }, INSTRUCTION{ "RES 3, D", &a::RES3r8 }, INSTRUCTION{ "RES 3, E", &a::RES3r8 }, INSTRUCTION{ "RES 3, H", &a::RES3r8 }, INSTRUCTION{ "RES 3, L", &a::RES3r8 }, INSTRUCTION{ "RES 3, (HL)", &a::RES3r8 }, INSTRUCTION{ "RES 3, A", &a::RES3r8 },
		INSTRUCTION{ "RES 4, B", &a::RES4r8 }, INSTRUCTION{ "RES 4, C", &a::RES4r8 }, INSTRUCTION{ "RES 4, D", &a::RES4r8 }, INSTRUCTION{ "RES 4, E", &a::RES4r8 }, INSTRUCTION{ "RES 4, H", &a::RES4r8 }, INSTRUCTION{ "RES 4, L", &a::RES4r8 }, INSTRUCTION{ "RES 4, (HL)", &a::RES4r8 }, INSTRUCTION{ "RES 4, A", &a::RES4r8 }, INSTRUCTION{ "RES 5, B", &a::RES5r8 }, INSTRUCTION{ "RES 5, C", &a::RES5r8 }, INSTRUCTION{ "RES 5, D", &a::RES5r8 }, INSTRUCTION{ "RES 5, E", &a::RES5r8 }, INSTRUCTION{ "RES 5, H", &a::RES5r8 }, INSTRUCTION{ "RES 5, L", &a::RES5r8 }, INSTRUCTION{ "RES 5, (HL)", &a::RES5r8 }, INSTRUCTION{ "RES 5, A", &a::RES5r8 },
		INSTRUCTION{ "RES 6, B", &a::RES6r8 }, INSTRUCTION{ "RES 6, C", &a::RES6r8 }, INSTRUCTION{ "RES 6, D", &a::RES6r8 }, INSTRUCTION{ "RES 6, E", &a::RES6r8 }, INSTRUCTION{ "RES 6, H", &a::RES6r8 }, INSTRUCTION{ "RES 6, L", &a::RES6r8 }, INSTRUCTION{ "RES 6, (HL)", &a::RES6r8 }, INSTRUCTION{ "RES 6, A", &a::RES6r8 }, INSTRUCTION{ "RES 7, B", &a::RES7r8 }, INSTRUCTION{ "RES 7, C", &a::RES7r8 }, INSTRUCTION{ "RES 7, D", &a::RES7r8 }, INSTRUCTION{ "RES 7, E", &a::RES7r8 }, INSTRUCTION{ "RES 7, H", &a::RES7r8 }, INSTRUCTION{ "RES 7, L", &a::RES7r8 }, INSTRUCTION{ "RES 7, (HL)", &a::RES7r8 }, INSTRUCTION{ "RES 7, A", &a::RES7r8 },
		INSTRUCTION{ "SET 0, B", &a::SET0r8 }, INSTRUCTION{ "SET 0, C", &a::SET0r8 }, INSTRUCTION{ "SET 0, D", &a::SET0r8 }, INSTRUCTION{ "SET 0, E", &a::SET0r8 }, INSTRUCTION{ "SET 0, H", &a::SET0r8 }, INSTRUCTION{ "SET 0, L", &a::SET0r8 }, INSTRUCTION{ "SET 0, (HL)", &a::SET0r8 }, INSTRUCTION{ "SET 0, A", &a::SET0r8 }, INSTRUCTION{ "SET 1, B", &a::SET1r8 }, INSTRUCTION{ "SET 1, C", &a::SET1r8 }, INSTRUCTION{ "SET 1, D", &a::SET1r8 }, INSTRUCTION{ "SET 1, E", &a::SET1r8 }, INSTRUCTION{ "SET 1, H", &a::SET1r8 }, INSTRUCTION{ "SET 1, L", &a::SET1r8 }, INSTRUCTION{ "SET 1, (HL)", &a::SET1r8 }, INSTRUCTION{ "SET 1, A", &a::SET1r8 },
		INSTRUCTION{ "SET 2, B", &a::SET2r8 }, INSTRUCTION{ "SET 2, C", &a::SET2r8 }, INSTRUCTION{ "SET 2, D", &a::SET2r8 }, INSTRUCTION{ "SET 2, E", &a::SET2r8 }, INSTRUCTION{ "SET 2, H", &a::SET2r8 }, INSTRUCTION{ "SET 2, L", &a::SET2r8 }, INSTRUCTION{ "SET 2, (HL)", &a::SET2r8 }, INSTRUCTION{ "SET 2, A", &a::SET2r8 }, INSTRUCTION{ "SET 3, B", &a::SET3r8 }, INSTRUCTION{ "SET 3, C", &a::SET3r8 }, INSTRUCTION{ "SET 3, D", &a::SET3r8 }, INSTRUCTION{ "SET 3, E", &a::SET3r8 }, INSTRUCTION{ "SET 3, H", &a::SET3r8 }, INSTRUCTION{ "SET 3, L", &a::SET3r8 }, INSTRUCTION{ "SET 3, (HL)", &a::SET3r8 }, INSTRUCTION{ "SET 3, A", &a::SET3r8 },
		INSTRUCTION{ "SET 4, B", &a::SET4r8 }, INSTRUCTION{ "SET 4, C", &a::SET4r8 }, INSTRUCTION{ "SET 4, D", &a::SET4r8 }, INSTRUCTION{ "SET 4, E", &a::SET4r8 }, INSTRUCTION{ "SET 4, H", &a::SET4r8 }, INSTRUCTION{ "SET 4, L", &a::SET4r8 }, INSTRUCTION{ "SET 4, (HL)", &a::SET4r8 }, INSTRUCTION{ "SET 4, A", &a::SET4r8 }, INSTRUCTION{ "SET 5, B", &a::SET5r8 }, INSTRUCTION{ "SET 5, C", &a::SET5r8 }, INSTRUCTION{ "SET 5, D", &a::SET5r8 }, INSTRUCTION{ "SET 5, E", &a::SET5r8 }, INSTRUCTION{ "SET 5, H", &a::SET5r8 }, INSTRUCTION{ "SET 5, L", &a::SET5r8 }, INSTRUCTION{ "SET 5, (HL)", &a::SET5r8 }, INSTRUCTION{ "SET 5, A", &a::SET5r8 },
		INSTRUCTION{ "SET 6, B", &a::SET6r8 }, INSTRUCTION{ "SET 6, C", &a::SET6r8 }, INSTRUCTION{ "SET 6, D", &a::SET6r8 }, INSTRUCTION{ "SET 6, E", &a::SET6r8 }, INSTRUCTION{ "SET 6, H", &a::SET6r8 }, INSTRUCTION{ "SET 6, L", &a::SET6r8 }, INSTRUCTION{ "SET 6, (HL)", &a::SET6r8 }, INSTRUCTION{ "SET 6, A", &a::SET6r8 }, INSTRUCTION{ "SET 6, B", &a::SET7r8 }, INSTRUCTION{ "SET 7, C", &a::SET7r8 }, INSTRUCTION{ "SET 5, D", &a::SET7r8 }, INSTRUCTION{ "SET 7, E", &a::SET7r8 }, INSTRUCTION{ "SET 7, H", &a::SET7r8 }, INSTRUCTION{ "SET 7, L", &a::SET7r8 }, INSTRUCTION{ "SET 7, (HL)", &a::SET7r8 }, INSTRUCTION{ "SET 7, A", &a::SET7r8 },
	};

	loadNintendoLogo();
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
		if (read(0xFF01) != previousFF01)
		{
			FF01Changes.push_back(read(0xFF01));
			previousFF01 = read(0xFF01);
		}
		if (stepMode) 
		{
			std::cout << "Press spacebar to execute the next instruction..." << std::endl;
			while (!_kbhit() || _getch() != ' ');
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
		if (!wasCB)
		{
			(this->*lookup[read(pc)].operate)();
		}
		else
		{
			(this->*lookupCB[read(pc)].operate)();
			wasCB = false;
		}

		if (ime && read(IE) && read(IF))
		{
			uint8_t ifired = read(IE) & read(IF);

			if (ifired & 0x01)
			{
				uint8_t IFvalue = read(IF);
				IFvalue &= (255 - 0x01);
				write(IF, IFvalue);

				RST40();
			}
		}

		if (cyclesRan >= 456 / 4)
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

			if (read(ly) >= 0x90 && !inVblank)
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
				uint8_t ieValue = read(IE);
				ieValue |= (1 << 0);
				write(IE, ieValue);
			}
		}
	}
	cycles -= 4;
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

void Cpu::RST40()
{
	std::cout << "ENTERING A VBLANK" << std::endl;
	inVblank = true;
	ime = 0;

	write(--stkp, (pc >> 8) & 0xFF);
	write(--stkp, pc & 0xFF);

	pc = 0x0040;

	cycles += 12;
	cyclesRan += 12;
}

void Cpu::RETI()
{
	std::cout << "RETURNING FROM INTERRUPT" << std::endl;
	if (inVblank)
	{
		inVblank = false;
	}
	ime = 1;
	uint8_t lo = read(stkp++);
	uint8_t hi = read(stkp++);

	pc = (hi << 8) | lo;

	cycles += 12;
	cyclesRan += 12;
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

void Cpu::ADDrrRR()
{
	uint8_t opcode = read(pc++);
	uint16_t* r16 = nullptr;

	switch (opcode)
	{
		case 0x09: // ADD HL, BC
		{
			r16 = &registers.bc;
			break;
		}
		case 0x19: // ADD HL, DE
		{
			r16 = &registers.de;
			break;
		}
		case 0x29: // ADD HL, HL
		{
			r16 = &registers.hl;
			break;
		}
		case 0x39: // ADD HL, SP
		{
			r16 = &stkp;
			break;
		}
		default:
			std::cout << "Unknown ADDrrRR instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	if (r16 == nullptr)
	{
		setHasNotBroken(false);
		return;
	}

	uint32_t result = static_cast<uint32_t>(registers.hl) + static_cast<uint32_t>(*r16);

	setFlag(Subtraction, false);
	setFlag(HalfCarry, ((registers.hl & 0xFFF) + (*r16 & 0xFFF)) > 0xFFF);
	setFlag(Carry, result > 0xFFFF);

	registers.hl = static_cast<uint16_t>(result & 0xFFFF);

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

void Cpu::INCHL()
{
	uint8_t opcode = read(pc++);

	uint8_t value = read(registers.hl);
	value++;

	write(registers.hl, value);

	setFlag(Zero, value == 0);
	setFlag(Subtraction, false);
	setFlag(HalfCarry, (value & 0x0F) == 0);

	cycles += 12;
	cyclesRan += 12;
	
}

void Cpu::DECr()
{
	uint8_t opcode = read(pc++);

	switch (opcode)
	{
		case 0x05: // DEC B
		{
			uint8_t value = registers.b;
			value--;
			registers.b = value;

			setFlag(Zero, registers.b == 0x00);
			setFlag(Subtraction, true);
			setFlag(HalfCarry, (registers.b & 0x0F) == 0x0F);

			break;
		}
		case 0x15: // DEC D
		{
			uint8_t value = registers.d;
			value--;
			registers.d = value;

			setFlag(Zero, registers.d == 0x00);
			setFlag(Subtraction, true);
			setFlag(HalfCarry, (registers.d & 0x0F) == 0x0F);

			break;
		}
		case 0x25: // DEC H
		{
			uint8_t value = registers.h;
			value--;
			registers.h = value;

			setFlag(Zero, registers.h == 0x00);
			setFlag(Subtraction, true);
			setFlag(HalfCarry, (registers.h & 0x0F) == 0x0F);

			break;
		}
		case 0x0D: // DEC C
		{
			uint8_t value = registers.c;
			value--;
			registers.c = value;

			setFlag(Zero, registers.c == 0x00);
			setFlag(Subtraction, true);
			setFlag(HalfCarry, (registers.c & 0x0F) == 0x0F);

			break;
		}
		case 0x1D: // DEC E
		{
			uint8_t value = registers.e;
			value--;
			registers.e = value;

			setFlag(Zero, registers.e == 0x00);
			setFlag(Subtraction, true);
			setFlag(HalfCarry, (registers.e & 0x0F) == 0x0F);

			break;
		}
		case 0x2D: // DEC L
		{
			uint8_t value = registers.l;
			value--;
			registers.l = value;

			setFlag(Zero, registers.l == 0x00);
			setFlag(Subtraction, true);
			setFlag(HalfCarry, (registers.l & 0x0F) == 0x0F);

			break;
		}
		case 0x3D: // DEC A
		{
			uint8_t value = registers.a;
			value--;
			registers.a = value;

			setFlag(Zero, registers.a == 0x00);
			setFlag(Subtraction, true);
			setFlag(HalfCarry, (registers.a & 0x0F) == 0x0F);

			break;
		}
		default:
			std::cout << "Unknown DECr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::DECHL()
{
	uint8_t opcode = read(pc++);

	uint8_t value = read(registers.hl);
	value--;
	write(registers.hl, value);

	setFlag(Zero, value == 0);
	setFlag(Subtraction, true);
	setFlag(HalfCarry, (value & 0x0F) == 0);

	cycles += 12;
	cyclesRan += 12;
}

void Cpu::LDHLSPr8()
{
	uint8_t opcode = read(pc++);
	int data = static_cast<int>(static_cast<int8_t>(read(pc++)));

	int result = stkp + data;

	setFlag(Zero, false);
	setFlag(Subtraction, false);
	setFlag(HalfCarry, ((stkp & 0x0F) + (data & 0x0F)) > 0x0F);
	setFlag(Carry, ((stkp & 0xFF) + (data & 0xFF)) > 0xFF);

	registers.hl = static_cast<uint16_t>(result & 0xFFFF);

	cycles += 12;
	cyclesRan += 12;
}

void Cpu::DECB()
{
	uint8_t opcode = read(pc++);

	uint8_t value = registers.b;
	value--;
	registers.b = value;

	setFlag(Zero, registers.b == 0x00);
	setFlag(Subtraction, true);
	setFlag(HalfCarry, (registers.b & 0x0F) == 0x0F);

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
			registers.e = read(registers.hl);
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
		case 0xCE:
		{
			uint8_t d8 = read(pc++);
			result = registers.a + d8 + carry;

			setFlag(HalfCarry, (registers.a & 0x0F) + (d8 & 0x0F) + carry > 0x0F);

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
			setFlag(Carry, d8 > registers.a);

			cycles += 4;
			cyclesRan += 4;
			break;
		}
		case 0xDE:
		{
			uint8_t d8 = read(pc++);

			uint8_t carry = isFlagSet(Carry) ? 1 : 0;
			result = registers.a - d8 - carry;

			setFlag(HalfCarry, (registers.a & 0x0F) < (d8 & 0x0F) + carry);
			setFlag(Carry, d8 + carry > registers.a);

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

	if (regPtr == nullptr)
	{
		return;
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
		case 0xEE:
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

	if (regPtr == nullptr)
	{
		return;
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
			hlValue = read(registers.hl);
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

void Cpu::DAA()
{
	uint8_t value = registers.a;

	bool carry = isFlagSet(Carry);
	bool halfCarry = isFlagSet(HalfCarry);
	bool subtract = isFlagSet(Subtraction);

	if (subtract) 
	{
		if (carry)
		{
			value -= 0x60;
		}
		if (halfCarry)
		{
			value -= 0x06;
		}
	}
	else 
	{
		if (carry || value > 0x99) 
		{
			value += 0x60;
			setFlag(Carry, true);
		}
		if (halfCarry || (value & 0x0F) > 0x09)
		{
			value += 0x06;
		}
	}

	registers.a = value;
	setFlag(Zero, value == 0);
	setFlag(HalfCarry, false);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::ADDSPr8()
{
	uint8_t opcode = read(pc++);
	int data = static_cast<int>(static_cast<int8_t>(read(pc++)));
	int result = stkp + data;

	setFlag(Zero, false);
	setFlag(Subtraction, false);
	setFlag(HalfCarry, ((stkp & 0x0F) + (data & 0x0F)) > 0x0F);
	setFlag(Carry, ((stkp & 0xFF) + (data & 0xFF)) > 0xFF);

	stkp = static_cast<uint16_t>(result & 0xFFFF);

	cycles += 16;
	cyclesRan += 16;
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
			break;
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

void Cpu::POPrr()
{
	uint8_t opcode = read(pc++);

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
	default:
		std::cout << "Unknown POPrr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		setHasNotBroken(false);
		break;
	}

	cycles += 12;
	cyclesRan += 12;
}

void Cpu::PUSHAF()
{
	pc++;

	write(--stkp, (registers.af >> 8) & 0xFF);
	write(--stkp, registers.af & 0xFF);

	cycles += 16;
	cyclesRan += 16;
}

void Cpu::POPAF()
{
	pc++;

	uint8_t lo = read(stkp++);
	uint8_t hi = read(stkp++);

	registers.af = (hi << 8) | lo;
	registers.f &= 0xF0;

	cycles += 12;
	cyclesRan += 12;
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
	uint16_t a8 = 0xFF00 + read(pc++);

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
	uint8_t opcode = read(pc);

	std::cout << "Unknown/Illegal instruction: "<< "PREFIX CB: " << wasCB << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
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

	ime = 0;

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::EI()
{
	uint8_t opcode = read(pc++);

	ime = 1;
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

void Cpu::RRA()
{
	uint8_t opcode = read(pc++);

	bool carry = isFlagSet(Carry);
	bool new_carry = registers.a & 0x01;

	registers.a = (registers.a >> 1) | (carry << 7);

	setFlag(Zero, false);
	setFlag(Subtraction, false);
	setFlag(HalfCarry, false);
	setFlag(Carry, new_carry);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::CPL()
{
	uint8_t opcode = read(pc++);

	registers.a = ~registers.a;

	setFlag(Subtraction, true);
	setFlag(HalfCarry, true);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::SCF()
{
	uint8_t opcode = read(pc++);

	setFlag(Subtraction, false);
	setFlag(HalfCarry, false);
	setFlag(Carry, true);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::CCF()
{
	uint8_t opcode = read(pc++);

	uint8_t carry = !isFlagSet(Carry);

	setFlag(Subtraction, false);
	setFlag(HalfCarry, false);
	setFlag(Carry, carry);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::LDSPHL()
{
	uint8_t opcode = read(pc++);

	stkp = registers.hl;

	cycles += 8;
	cyclesRan += 8;
}

void Cpu::RRCA()
{
	uint8_t opcode = read(pc++);

	bool carry = registers.a & 0x01;

	registers.a >>= 1;
	registers.a |= (carry << 7);

	setFlag(Zero, false);
	setFlag(Subtraction, false);
	setFlag(HalfCarry, false);
	setFlag(Carry, carry);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::RLCA() {
	uint8_t opcode = read(pc++);

	bool carry = (registers.a & 0x80) != 0;

	registers.a <<= 1;
	registers.a |= carry;

	setFlag(Zero, false);
	setFlag(Subtraction, false);
	setFlag(HalfCarry, false);
	setFlag(Carry, carry);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::RLA() {
	uint8_t opcode = read(pc++);

	bool carry = isFlagSet(Carry);

	setFlag(Carry, (registers.a & 0x80) != 0);
	registers.a = (registers.a << 1) | carry;

	setFlag(Zero, false);
	setFlag(Subtraction, false);
	setFlag(HalfCarry, false);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::PREFIXCB()
{
	uint8_t opcode = read(pc++);

	wasCB = true;

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::RLCr8()
{
	uint8_t opcode = read(pc++);

	uint8_t* r8 = nullptr;

	switch (opcode)
	{
		case 0x00:
		{
			r8 = &registers.b;
			break;
		}
		case 0x01:
		{
			r8 = &registers.c;
			break;
		}
		case 0x02:
		{
			r8 = &registers.d;
			break;
		}
		case 0x03:
		{
			r8 = &registers.e;
			break;
		}
		case 0x04:
		{
			r8 = &registers.h;
			break;
		}
		case 0x05:
		{
			r8 = &registers.l;
			break;
		}
		case 0x06:
		{
			uint8_t value = read(registers.hl);
			
			bool msb = (value & 0x80) != 0;
			value = (value << 1) | msb;

			write(registers.hl, value);

			setFlag(Zero, value == 0);
			setFlag(Subtraction, false);
			setFlag(HalfCarry, false);
			setFlag(Carry, msb);

			cycles += 12;
			cyclesRan += 12;
			break;
		}
		case 0x07:
		{
			r8 = &registers.a;
			break;
		}
		default:
			std::cout << "Unknown RLCr8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	if (r8 == nullptr) 
	{
		return;
	}

	bool msb = (*r8 & 0x80) != 0;

	*r8 = (*r8 << 1) | msb;

	setFlag(Zero, *r8 == 0);
	setFlag(Subtraction, false);
	setFlag(HalfCarry, false);
	setFlag(Carry, msb);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::RRCr8()
{
	uint8_t opcode = read(pc++);

	uint8_t* r8 = nullptr;

	switch (opcode)
	{
		case 0x08:
		{
			r8 = &registers.b;
			break;
		}
		case 0x09:
		{
			r8 = &registers.c;
			break;
		}
		case 0x0A:
		{
			r8 = &registers.d;
			break;
		}
		case 0x0B:
		{
			r8 = &registers.e;
			break;
		}
		case 0x0C:
		{
			r8 = &registers.h;
			break;
		}
		case 0x0D:
		{
			r8 = &registers.l;
			break;
		}
		case 0x0E:
		{
			uint8_t value = read(registers.hl);

			bool carry = value & 0x01;
			value = (value >> 1) | (carry << 7);

			write(registers.hl, value);

			setFlag(Zero, value == 0);
			setFlag(Subtraction, false);
			setFlag(HalfCarry, false);
			setFlag(Carry, carry);

			cycles += 12;
			cyclesRan += 12;
			break;
		}
		case 0x0F:
		{
			r8 = &registers.a;
			break;
		}
		default:
			std::cout << "Unknown RRCr8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	if (r8 == nullptr)
	{
		return;
	}

	bool carry = *r8 & 0x01;

	*r8 = (*r8 >> 1) | (carry << 7);

	setFlag(Zero, *r8 == 0);
	setFlag(Subtraction, false);
	setFlag(HalfCarry, false);
	setFlag(Carry, carry);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::SRLr8()
{
	uint8_t opcode = read(pc++);

	uint8_t* r8 = nullptr;

	switch (opcode)
	{
		case 0x38:
		{
			r8 = &registers.b;
			break;
		}
		case 0x39:
		{
			r8 = &registers.c;
			break;
		}
		case 0x3A:
		{
			r8 = &registers.d;
			break;
		}
		case 0x3B:
		{
			r8 = &registers.e;
			break;
		}
		case 0x3C:
		{
			r8 = &registers.h;
			break;
		}
		case 0x3D:
		{
			r8 = &registers.l;
			break;
		}
		case 0x3E:
		{
			uint8_t value = read(registers.hl);

			bool carry = value & 0x01;
			value >>= 1;

			write(registers.hl, value);

			setFlag(Zero, value == 0);
			setFlag(Subtraction, false);
			setFlag(HalfCarry, false);
			setFlag(Carry, carry);

			cycles += 12;
			cyclesRan += 12;
			break;
		}
		case 0x3F:
		{
			r8 = &registers.a;
			break;
		}
		default:
			std::cout << "Unknown SRLr8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			break;
	}

	if (r8 == nullptr)
	{
		return;
	}

	bool carry = *r8 & 0x01;

	*r8 >>= 1;

	setFlag(Zero, *r8 == 0);
	setFlag(Subtraction, false);
	setFlag(HalfCarry, false);
	setFlag(Carry, carry);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::RRr8()
{
	uint8_t opcode = read(pc++);

	uint8_t* r8 = nullptr;

	switch (opcode)
	{
		case 0x18:
		{
			r8 = &registers.b;
			break;
		}
		case 0x19:
		{
			r8 = &registers.c;
			break;
		}
		case 0x1A:
		{
			r8 = &registers.d;
			break;
		}
		case 0x1B:
		{
			r8 = &registers.e;
			break;
		}
		case 0x1C:
		{
			r8 = &registers.h;
			break;
		}
		case 0x1D:
		{
			r8 = &registers.l;
			break;
		}
		case 0x1E:
		{
			uint8_t value = read(registers.hl);

			bool carry = isFlagSet(Carry);
			bool new_carry = value & 0x01;
			value = (value >> 1) | (carry << 7);

			write(registers.hl, value);

			setFlag(Zero, value == 0);
			setFlag(Subtraction, false);
			setFlag(HalfCarry, false);
			setFlag(Carry, new_carry);

			cycles += 16;
			cyclesRan += 16;
			break;
		}
		case 0x1F:
		{
			r8 = &registers.a;
			break;
		}
		default:
			std::cout << "Unknown RRr8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	bool carry = isFlagSet(Carry);
	bool new_carry = *r8 & 0x01;

	*r8 = (*r8 >> 1) | (carry << 7);

	setFlag(Zero, *r8 == 0);
	setFlag(Subtraction, false);
	setFlag(HalfCarry, false);
	setFlag(Carry, new_carry);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::SWAPr8()
{
	uint8_t opcode = read(pc++);
	uint8_t* r8 = nullptr;

	switch (opcode)
	{
		case 0x30:
		{
			r8 = &registers.b;
			break;
		}
		case 0x31:
		{
			r8 = &registers.c;
			break;
		}
		case 0x32:
		{
			r8 = &registers.d;
			break;
		}
		case 0x33:
		{
			r8 = &registers.e;
			break;
		}
		case 0x34:
		{
			r8 = &registers.h;
			break;
		}
		case 0x35:
		{
			r8 = &registers.l;
			break;
		}
		case 0x36:
		{
			uint8_t value = read(registers.hl);

			value = ((value & 0x0F) << 4) | ((value & 0xF0) >> 4);

			write(registers.hl, value);

			setFlag(Zero, value == 0);
			setFlag(Subtraction, false);
			setFlag(HalfCarry, false);
			setFlag(Carry, false);

			cycles += 12;
			cyclesRan += 12;
			break;
		}
		case 0x37:
		{
			r8 = &registers.a;
			break;
		}
		default:
			std::cout << "Unknown SWAPr8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			return;
	}

	if (r8 == nullptr) 
	{
		return;
	}

	*r8 = ((*r8 & 0x0F) << 4) | ((*r8 & 0xF0) >> 4);

	setFlag(Zero, *r8 == 0);
	setFlag(Subtraction, false);
	setFlag(HalfCarry, false);
	setFlag(Carry, false);

	cycles += 8;
	cyclesRan += 8;
}

void Cpu::RLr8()
{
	uint8_t opcode = read(pc++);
	uint8_t* r8 = nullptr;

	switch (opcode) {
		case 0x10:
		{
			r8 = &registers.b;
			break;
		}
		case 0x11:
		{
			r8 = &registers.c;
			break;
		}
		case 0x12:
		{
			r8 = &registers.d;
			break;
		}
		case 0x13:
		{
			r8 = &registers.e;
			break;
		}
		case 0x14:
		{
			r8 = &registers.h;
			break;
		}
		case 0x15:
		{
			r8 = &registers.l;
			break;
		}
		case 0x16: 
		{
			uint8_t value = read(registers.hl);
			bool carry = isFlagSet(Carry);
			bool msb = (value & 0x80) != 0;

			value = (value << 1) | carry;

			write(registers.hl, value);

			setFlag(Zero, value == 0);
			setFlag(Subtraction, false);
			setFlag(HalfCarry, false);
			setFlag(Carry, msb);

			cycles += 12;
			cyclesRan += 12;
			break;
		}
		case 0x17:
		{
			r8 = &registers.a;
			break;
		}
		default:
			std::cout << "Unknown RLr8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			return;
	}

	if (r8 == nullptr) {
		return;
	}

	bool carry = isFlagSet(Carry);
	bool msb = (*r8 & 0x80) != 0;

	*r8 <<= 1;
	*r8 |= carry;

	setFlag(Zero, *r8 == 0);
	setFlag(Subtraction, false);
	setFlag(HalfCarry, false);
	setFlag(Carry, msb);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::SLAr8()
{
	uint8_t opcode = read(pc++);
	uint8_t* r8 = nullptr;

	switch (opcode)
	{
		case 0x20:
		{
			r8 = &registers.b;
			break;
		}
		case 0x21:
		{
			r8 = &registers.c;
			break;
		}
		case 0x22:
		{
			r8 = &registers.d;
			break;
		}
		case 0x23:
		{
			r8 = &registers.e;
			break;
		}
		case 0x24:
		{
			r8 = &registers.h;
			break;
		}
		case 0x25:
		{
			r8 = &registers.l;
			break;
		}
		case 0x26:
		{
			uint8_t value = read(registers.hl);
			bool carry = (value & 0x80) != 0;
			value <<= 1;

			write(registers.hl, value);

			setFlag(Zero, value == 0x00);
			setFlag(Subtraction, false);
			setFlag(HalfCarry, false);
			setFlag(Carry, carry);

			cycles += 12;
			cyclesRan += 12;
			break;
		}
		case 0x27:
		{
			r8 = &registers.a;
			break;
		}
		default:
			std::cout << "Unknown SLAr8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	bool carry = (*r8 & 0x80) != 0;

	*r8 <<= 1;

	setFlag(Zero, *r8 == 0x00);
	setFlag(Subtraction, false);
	setFlag(HalfCarry, false);
	setFlag(Carry, carry);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::SRAr8()
{
	uint8_t opcode = read(pc++);
	uint8_t* r8 = nullptr;

	switch (opcode)
	{
		case 0x28:
		{
			r8 = &registers.b;
			break;
		}
		case 0x29:
		{
			r8 = &registers.c;
			break;
		}
		case 0x2A:
		{
			r8 = &registers.d;
			break;
		}
		case 0x2B:
		{
			r8 = &registers.e;
			break;
		}
		case 0x2C:
		{
			r8 = &registers.h;
			break;
		}
		case 0x2D:
		{
			r8 = &registers.l;
			break;
		}
		case 0x2E:
		{
			uint8_t value = read(registers.hl);
			bool carry = (value & 0x01) != 0;
			bool bit7 = (value & 0x80) != 0;

			value >>= 1;

			if (bit7)
			{
				value |= 0x80;
			}

			write(registers.hl, value);

			setFlag(Zero, value == 0x00);
			setFlag(Subtraction, false);
			setFlag(HalfCarry, false);
			setFlag(Carry, carry);

			cycles += 12;
			cyclesRan += 12;
			break;
		}
		case 0x2F:
		{
			r8 = &registers.a;
			break;
		}
		default:
			std::cout << "Unknown SRAr8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	bool carry = (*r8 & 0x01) != 0;
	bool bit7 = (*r8 & 0x80) != 0;

	*r8 >>= 1;

	if (bit7)
	{
		*r8 |= 0x80;
	}

	setFlag(Zero, *r8 == 0x00);
	setFlag(Subtraction, false);
	setFlag(HalfCarry, false);
	setFlag(Carry, carry);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::RESu3r8()
{
	uint8_t opcode = read(pc++);

	switch (opcode)
	{
		case 0x87:
		{
			registers.a &= ~(1 << 0);
			break;
		}
		case 0x88:
		{
			registers.b &= ~(1 << 1);
			break;
		}
		case 0x89:
		{
			registers.c &= ~(1 << 1);
			break;
		}
		case 0x8A:
		{
			registers.d &= ~(1 << 1);
			break;
		}
		case 0x8B:
		{
			registers.e &= ~(1 << 1);
			break;
		}
		case 0x8C:
		{
			registers.h &= ~(1 << 1);
			break;
		}
		case 0x8D:
		{
			registers.l &= ~(1 << 1);
			break;
		}
		case 0x8E:
		{
			uint8_t value = read(registers.hl);

			value &= ~(1 << 1);

			write(registers.hl, value);
			cycles += 12;
			cyclesRan += 12;

			break;
		}
		case 0x8F:
		{
			registers.a &= ~(1 << 1);
			break;
		}
		default:
			std::cout << "Unknown RESu3r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			return;
	}

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::BIT0r8()
{
	uint8_t opcode = read(pc++);
	uint8_t* r8 = nullptr;

	switch (opcode)
	{
		case 0x40:
		{
			r8 = &registers.b;
			break;
		}
		case 0x41:
		{
			r8 = &registers.c;
			break;
		}
		case 0x42:
		{
			r8 = &registers.d;
			break;
		}
		case 0x43:
		{
			r8 = &registers.e;
			break;
		}
		case 0x44:
		{
			r8 = &registers.h;
			break;
		}
		case 0x45:
		{
			r8 = &registers.l;
			break;
		}
		case 0x46:
		{
			uint8_t value = read(registers.hl);
			setFlag(Zero, !(value & (1 << 0)));
			setFlag(Subtraction, false);
			setFlag(HalfCarry, true);

			cycles += 12;
			cyclesRan += 12;
			break;
		}
		case 0x47:
		{
			r8 = &registers.a;
			break;
		}
		default:
			std::cout << "Unknown BIT0r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	setFlag(Zero, !(*r8 & (1 << 0)));
	setFlag(Subtraction, false);
	setFlag(HalfCarry, true);
	cycles += 4;
	cyclesRan += 4;
}

void Cpu::BIT1r8()
{
	uint8_t opcode = read(pc++);
	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0x48:
	{
		r8 = &registers.b;
		break;
	}
	case 0x49:
	{
		r8 = &registers.c;
		break;
	}
	case 0x4A:
	{
		r8 = &registers.d;
		break;
	}
	case 0x4B:
	{
		r8 = &registers.e;
		break;
	}
	case 0x4C:
	{
		r8 = &registers.h;
		break;
	}
	case 0x4D:
	{
		r8 = &registers.l;
		break;
	}
	case 0x4E:
	{
		uint8_t value = read(registers.hl);
		setFlag(Zero, !(value & (1 << 1)));
		setFlag(Subtraction, false);
		setFlag(HalfCarry, true);

		cycles += 12;
		cyclesRan += 12;
		break;
	}
	case 0x4F:
	{
		r8 = &registers.a;
		break;
	}
	default:
		std::cout << "Unknown BIT1r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	setFlag(Zero, !(*r8 & (1 << 1)));
	setFlag(Subtraction, false);
	setFlag(HalfCarry, true);
	cycles += 4;
	cyclesRan += 4;
}

void Cpu::BIT2r8()
{
	uint8_t opcode = read(pc++);
	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0x50:
	{
		r8 = &registers.b;
		break;
	}
	case 0x51:
	{
		r8 = &registers.c;
		break;
	}
	case 0x52:
	{
		r8 = &registers.d;
		break;
	}
	case 0x53:
	{
		r8 = &registers.e;
		break;
	}
	case 0x54:
	{
		r8 = &registers.h;
		break;
	}
	case 0x55:
	{
		r8 = &registers.l;
		break;
	}
	case 0x56:
	{
		uint8_t value = read(registers.hl);
		setFlag(Zero, !(value & (1 << 2)));
		setFlag(Subtraction, false);
		setFlag(HalfCarry, true);

		cycles += 12;
		cyclesRan += 12;
		break;
	}
	case 0x57:
	{
		r8 = &registers.a;
		break;
	}
	default:
		std::cout << "Unknown BIT2r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	setFlag(Zero, !(*r8 & (1 << 2)));
	setFlag(Subtraction, false);
	setFlag(HalfCarry, true);
	cycles += 4;
	cyclesRan += 4;
}

void Cpu::BIT3r8()
{
	uint8_t opcode = read(pc++);
	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0x58:
	{
		r8 = &registers.b;
		break;
	}
	case 0x59:
	{
		r8 = &registers.c;
		break;
	}
	case 0x5A:
	{
		r8 = &registers.d;
		break;
	}
	case 0x5B:
	{
		r8 = &registers.e;
		break;
	}
	case 0x5C:
	{
		r8 = &registers.h;
		break;
	}
	case 0x5D:
	{
		r8 = &registers.l;
		break;
	}
	case 0x5E:
	{
		uint8_t value = read(registers.hl);
		setFlag(Zero, !(value & (1 << 3)));
		setFlag(Subtraction, false);
		setFlag(HalfCarry, true);

		cycles += 12;
		cyclesRan += 12;
		break;
	}
	case 0x5F:
	{
		r8 = &registers.a;
		break;
	}
	default:
		std::cout << "Unknown BIT3r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	setFlag(Zero, !(*r8 & (1 << 3)));
	setFlag(Subtraction, false);
	setFlag(HalfCarry, true);
	cycles += 4;
	cyclesRan += 4;
}

void Cpu::BIT4r8()
{
	uint8_t opcode = read(pc++);
	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0x60:
	{
		r8 = &registers.b;
		break;
	}
	case 0x61:
	{
		r8 = &registers.c;
		break;
	}
	case 0x62:
	{
		r8 = &registers.d;
		break;
	}
	case 0x63:
	{
		r8 = &registers.e;
		break;
	}
	case 0x64:
	{
		r8 = &registers.h;
		break;
	}
	case 0x65:
	{
		r8 = &registers.l;
		break;
	}
	case 0x66:
	{
		uint8_t value = read(registers.hl);
		setFlag(Zero, !(value & (1 << 4)));
		setFlag(Subtraction, false);
		setFlag(HalfCarry, true);

		cycles += 12;
		cyclesRan += 12;
		break;
	}
	case 0x67:
	{
		r8 = &registers.a;
		break;
	}
	default:
		std::cout << "Unknown BIT4r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	setFlag(Zero, !(*r8 & (1 << 4)));
	setFlag(Subtraction, false);
	setFlag(HalfCarry, true);
	cycles += 4;
	cyclesRan += 4;
}

void Cpu::BIT5r8()
{
	uint8_t opcode = read(pc++);
	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0x68:
	{
		r8 = &registers.b;
		break;
	}
	case 0x69:
	{
		r8 = &registers.c;
		break;
	}
	case 0x6A:
	{
		r8 = &registers.d;
		break;
	}
	case 0x6B:
	{
		r8 = &registers.e;
		break;
	}
	case 0x6C:
	{
		r8 = &registers.h;
		break;
	}
	case 0x6D:
	{
		r8 = &registers.l;
		break;
	}
	case 0x6E:
	{
		uint8_t value = read(registers.hl);
		setFlag(Zero, !(value & (1 << 5)));
		setFlag(Subtraction, false);
		setFlag(HalfCarry, true);

		cycles += 12;
		cyclesRan += 12;
		break;
	}
	case 0x6F:
	{
		r8 = &registers.a;
		break;
	}
	default:
		std::cout << "Unknown BIT5r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	setFlag(Zero, !(*r8 & (1 << 5)));
	setFlag(Subtraction, false);
	setFlag(HalfCarry, true);
	cycles += 4;
	cyclesRan += 4;
}

void Cpu::BIT6r8()
{
	uint8_t opcode = read(pc++);
	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0x70:
	{
		r8 = &registers.b;
		break;
	}
	case 0x71:
	{
		r8 = &registers.c;
		break;
	}
	case 0x72:
	{
		r8 = &registers.d;
		break;
	}
	case 0x73:
	{
		r8 = &registers.e;
		break;
	}
	case 0x74:
	{
		r8 = &registers.h;
		break;
	}
	case 0x75:
	{
		r8 = &registers.l;
		break;
	}
	case 0x76:
	{
		uint8_t value = read(registers.hl);
		setFlag(Zero, !(value & (1 << 6)));
		setFlag(Subtraction, false);
		setFlag(HalfCarry, true);

		cycles += 12;
		cyclesRan += 12;
		break;
	}
	case 0x77:
	{
		r8 = &registers.a;
		break;
	}
	default:
		std::cout << "Unknown BIT6r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	setFlag(Zero, !(*r8 & (1 << 6)));
	setFlag(Subtraction, false);
	setFlag(HalfCarry, true);
	cycles += 4;
	cyclesRan += 4;
}

void Cpu::BIT7r8()
{
	uint8_t opcode = read(pc++);
	uint8_t* r8 = nullptr;

	switch (opcode)
	{
		case 0x78:
		{
			r8 = &registers.b;
			break;
		}
		case 0x79:
		{
			r8 = &registers.c;
			break;
		}
		case 0x7A:
		{
			r8 = &registers.d;
			break;
		}
		case 0x7B:
		{
			r8 = &registers.e;
			break;
		}
		case 0x7C:
		{
			r8 = &registers.h;
			break;
		}
		case 0x7D:
		{
			r8 = &registers.l;
			break;
		}
		case 0x7E:
		{
			uint8_t value = read(registers.hl);
			setFlag(Zero, !(value & (1 << 7)));
			setFlag(Subtraction, false);
			setFlag(HalfCarry, true);

			cycles += 12;
			cyclesRan += 12;
			break;
		}
		case 0x7F:
		{
			r8 = &registers.a;
			break;
		}
		default:
			std::cout << "Unknown BIT7r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	setFlag(Zero, !(*r8 & (1 << 7)));
	setFlag(Subtraction, false);
	setFlag(HalfCarry, true);
	cycles += 4;
	cyclesRan += 4;
}

void Cpu::RES0r8()
{
	uint8_t opcode = read(pc++);
	uint8_t* r8 = nullptr;

	switch (opcode) {
		case 0x80:
		{
			r8 = &registers.b;
			break;
		}
		case 0x81:
		{
			r8 = &registers.c;
			break;
		}
		case 0x82:
		{
			r8 = &registers.d;
			break;
		}
		case 0x83:
		{
			r8 = &registers.e;
			break;
		}
		case 0x84:
		{
			r8 = &registers.h;
			break;
		}
		case 0x85:
		{
			r8 = &registers.l;
			break;
		}
		case 0x86:
		{
			uint8_t value = read(registers.hl);
			value = value & ~(1 << 0);

			write(registers.hl, value);

			cycles += 12;
			cyclesRan += 12;
			break;
		}
		case 0x87:
		{
			r8 = &registers.a;
			break;
		}
		default:
			std::cout << "Unknown RES0r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			return;
	}

	if (r8 == nullptr) 
	{
		return;
	}

	*r8 &= ~(1 << 0);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::RES1r8()
{
	uint8_t opcode = read(pc++);
	uint8_t* r8 = nullptr;

	switch (opcode) {
		case 0x88:
		{
			r8 = &registers.b;
			break;
		}
		case 0x89:
		{
			r8 = &registers.c;
			break;
		}
		case 0x8A:
		{
			r8 = &registers.d;
			break;
		}
		case 0x8B:
		{
			r8 = &registers.e;
			break;
		}
		case 0x8C:
		{
			r8 = &registers.h;
			break;
		}
		case 0x8D:
		{
			r8 = &registers.l;
			break;
		}
		case 0x8E:
		{
			uint8_t value = read(registers.hl);
			value = value & ~(1 << 1);

			write(registers.hl, value);

			cycles += 12;
			cyclesRan += 12;
			break;
		}
		case 0x8F:
		{
			r8 = &registers.a;
			break;
		}
	default:
		std::cout << "Unknown RES1r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	*r8 &= ~(1 << 1);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::RES2r8()
{
	uint8_t opcode = read(pc++);
	uint8_t* r8 = nullptr;

	switch (opcode) {
	case 0x90:
	{
		r8 = &registers.b;
		break;
	}
	case 0x91:
	{
		r8 = &registers.c;
		break;
	}
	case 0x92:
	{
		r8 = &registers.d;
		break;
	}
	case 0x93:
	{
		r8 = &registers.e;
		break;
	}
	case 0x94:
	{
		r8 = &registers.h;
		break;
	}
	case 0x95:
	{
		r8 = &registers.l;
		break;
	}
	case 0x96:
	{
		uint8_t value = read(registers.hl);
		value = value & ~(1 << 2);

		write(registers.hl, value);

		cycles += 12;
		cyclesRan += 12;
		break;
	}
	case 0x97:
	{
		r8 = &registers.a;
		break;
	}
	default:
		std::cout << "Unknown RES2r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	*r8 &= ~(1 << 2);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::RES3r8()
{
	uint8_t opcode = read(pc++);
	uint8_t* r8 = nullptr;

	switch (opcode) {
	case 0x98:
	{
		r8 = &registers.b;
		break;
	}
	case 0x99:
	{
		r8 = &registers.c;
		break;
	}
	case 0x9A:
	{
		r8 = &registers.d;
		break;
	}
	case 0x9B:
	{
		r8 = &registers.e;
		break;
	}
	case 0x9C:
	{
		r8 = &registers.h;
		break;
	}
	case 0x9D:
	{
		r8 = &registers.l;
		break;
	}
	case 0x9E:
	{
		uint8_t value = read(registers.hl);
		value = value & ~(1 << 3);

		write(registers.hl, value);

		cycles += 12;
		cyclesRan += 12;
		break;
	}
	case 0x9F:
	{
		r8 = &registers.a;
		break;
	}
	default:
		std::cout << "Unknown RES3r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	*r8 &= ~(1 << 3);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::RES4r8()
{
	uint8_t opcode = read(pc++);
	uint8_t* r8 = nullptr;

	switch (opcode) {
	case 0xA0:
	{
		r8 = &registers.b;
		break;
	}
	case 0xA1:
	{
		r8 = &registers.c;
		break;
	}
	case 0xA2:
	{
		r8 = &registers.d;
		break;
	}
	case 0xA3:
	{
		r8 = &registers.e;
		break;
	}
	case 0xA4:
	{
		r8 = &registers.h;
		break;
	}
	case 0xA5:
	{
		r8 = &registers.l;
		break;
	}
	case 0xA6:
	{
		uint8_t value = read(registers.hl);
		value = value & ~(1 << 4);

		write(registers.hl, value);

		cycles += 12;
		cyclesRan += 12;
		break;
	}
	case 0xA7:
	{
		r8 = &registers.a;
		break;
	}
	default:
		std::cout << "Unknown RES4r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	*r8 &= ~(1 << 4);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::RES5r8()
{
	uint8_t opcode = read(pc++);
	uint8_t* r8 = nullptr;

	switch (opcode) {
	case 0xA8:
	{
		r8 = &registers.b;
		break;
	}
	case 0xA9:
	{
		r8 = &registers.c;
		break;
	}
	case 0xAA:
	{
		r8 = &registers.d;
		break;
	}
	case 0xAB:
	{
		r8 = &registers.e;
		break;
	}
	case 0xAC:
	{
		r8 = &registers.h;
		break;
	}
	case 0xAD:
	{
		r8 = &registers.l;
		break;
	}
	case 0xAE:
	{
		uint8_t value = read(registers.hl);
		value = value & ~(1 << 5);

		write(registers.hl, value);

		cycles += 12;
		cyclesRan += 12;
		break;
	}
	case 0xAF:
	{
		r8 = &registers.a;
		break;
	}
	default:
		std::cout << "Unknown RES5r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	*r8 &= ~(1 << 5);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::RES6r8()
{
	uint8_t opcode = read(pc++);
	uint8_t* r8 = nullptr;

	switch (opcode) {
	case 0xB0:
	{
		r8 = &registers.b;
		break;
	}
	case 0xB1:
	{
		r8 = &registers.c;
		break;
	}
	case 0xB2:
	{
		r8 = &registers.d;
		break;
	}
	case 0xB3:
	{
		r8 = &registers.e;
		break;
	}
	case 0xB4:
	{
		r8 = &registers.h;
		break;
	}
	case 0xB5:
	{
		r8 = &registers.l;
		break;
	}
	case 0xB6:
	{
		uint8_t value = read(registers.hl);
		value = value & ~(1 << 6);

		write(registers.hl, value);

		cycles += 12;
		cyclesRan += 12;
		break;
	}
	case 0xB7:
	{
		r8 = &registers.a;
		break;
	}
	default:
		std::cout << "Unknown RES6r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	*r8 &= ~(1 << 6);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::RES7r8()
{
	uint8_t opcode = read(pc++);
	uint8_t* r8 = nullptr;

	switch (opcode) {
	case 0xB8:
	{
		r8 = &registers.b;
		break;
	}
	case 0xB9:
	{
		r8 = &registers.c;
		break;
	}
	case 0xBA:
	{
		r8 = &registers.d;
		break;
	}
	case 0xBB:
	{
		r8 = &registers.e;
		break;
	}
	case 0xBC:
	{
		r8 = &registers.h;
		break;
	}
	case 0xBD:
	{
		r8 = &registers.l;
		break;
	}
	case 0xBE:
	{
		uint8_t value = read(registers.hl);
		value = value & ~(1 << 1);

		write(registers.hl, value);

		cycles += 12;
		cyclesRan += 12;
		break;
	}
	case 0xBF:
	{
		r8 = &registers.a;
		break;
	}
	default:
		std::cout << "Unknown RES7r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	*r8 &= ~(1 << 7);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::SET0r8()
{
	uint8_t opcode = read(pc++);

	uint8_t* r8 = nullptr;

	switch (opcode)
	{
		case 0xC0:
		{
			r8 = &registers.b;
			break;
		}
		case 0xC1:
		{
			r8 = &registers.c;
			break;
		}
		case 0xC2:
		{
			r8 = &registers.d;
			break;
		}
		case 0xC3:
		{
			r8 = &registers.e;
			break;
		}
		case 0xC4:
		{
			r8 = &registers.h;
			break;
		}
		case 0xC5:
		{
			r8 = &registers.l;
			break;
		}
		case 0xC6:
		{
			uint8_t value = read(registers.hl);
			value |= (1 << 0);

			write(registers.hl, value);

			cycles += 12;
			cyclesRan += 12;
			break;
		}
		case 0xC7:
		{
			r8 = &registers.a;
			break;
		}
		default:
			std::cout << "Unknown SET0r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			setHasNotBroken(false);
			return;
	}

	if (r8 == nullptr) {
		return;
	}

	*r8 |= (1 << 0);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::SET1r8()
{
	uint8_t opcode = read(pc++);

	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0xC8:
	{
		r8 = &registers.b;
		break;
	}
	case 0xC9:
	{
		r8 = &registers.c;
		break;
	}
	case 0xCA:
	{
		r8 = &registers.d;
		break;
	}
	case 0xCB:
	{
		r8 = &registers.e;
		break;
	}
	case 0xCC:
	{
		r8 = &registers.h;
		break;
	}
	case 0xCD:
	{
		r8 = &registers.l;
		break;
	}
	case 0xCE:
	{
		uint8_t value = read(registers.hl);
		value |= (1 << 1);

		write(registers.hl, value);

		cycles += 12;
		cyclesRan += 12;
		break;
	}
	case 0xCF:
	{
		r8 = &registers.a;
		break;
	}
	default:
		std::cout << "Unknown SET1r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr) {
		return;
	}

	*r8 |= (1 << 1);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::SET2r8()
{
	uint8_t opcode = read(pc++);

	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0xD0:
	{
		r8 = &registers.b;
		break;
	}
	case 0xD1:
	{
		r8 = &registers.c;
		break;
	}
	case 0xD2:
	{
		r8 = &registers.d;
		break;
	}
	case 0xD3:
	{
		r8 = &registers.e;
		break;
	}
	case 0xD4:
	{
		r8 = &registers.h;
		break;
	}
	case 0xD5:
	{
		r8 = &registers.l;
		break;
	}
	case 0xD6:
	{
		uint8_t value = read(registers.hl);
		value |= (1 << 2);

		write(registers.hl, value);

		cycles += 12;
		cyclesRan += 12;
		break;
	}
	case 0xD7:
	{
		r8 = &registers.a;
		break;
	}
	default:
		std::cout << "Unknown SET2r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr) {
		return;
	}

	*r8 |= (1 << 2);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::SET3r8()
{
	uint8_t opcode = read(pc++);

	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0xD8:
	{
		r8 = &registers.b;
		break;
	}
	case 0xD9:
	{
		r8 = &registers.c;
		break;
	}
	case 0xDA:
	{
		r8 = &registers.d;
		break;
	}
	case 0xDB:
	{
		r8 = &registers.e;
		break;
	}
	case 0xDC:
	{
		r8 = &registers.h;
		break;
	}
	case 0xDD:
	{
		r8 = &registers.l;
		break;
	}
	case 0xDE:
	{
		uint8_t value = read(registers.hl);
		value |= (1 << 3);

		write(registers.hl, value);

		cycles += 12;
		cyclesRan += 12;
		break;
	}
	case 0xDF:
	{
		r8 = &registers.a;
		break;
	}
	default:
		std::cout << "Unknown SET3r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr) {
		return;
	}

	*r8 |= (1 << 3);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::SET4r8()
{
	uint8_t opcode = read(pc++);

	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0xE0:
	{
		r8 = &registers.b;
		break;
	}
	case 0xE1:
	{
		r8 = &registers.c;
		break;
	}
	case 0xE2:
	{
		r8 = &registers.d;
		break;
	}
	case 0xE3:
	{
		r8 = &registers.e;
		break;
	}
	case 0xE4:
	{
		r8 = &registers.h;
		break;
	}
	case 0xE5:
	{
		r8 = &registers.l;
		break;
	}
	case 0xE6:
	{
		uint8_t value = read(registers.hl);
		value |= (1 << 4);

		write(registers.hl, value);

		cycles += 12;
		cyclesRan += 12;
		break;
	}
	case 0xE7:
	{
		r8 = &registers.a;
		break;
	}
	default:
		std::cout << "Unknown SET4r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr) {
		return;
	}

	*r8 |= (1 << 4);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::SET5r8()
{
	uint8_t opcode = read(pc++);

	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0xE8:
	{
		r8 = &registers.b;
		break;
	}
	case 0xE9:
	{
		r8 = &registers.c;
		break;
	}
	case 0xEA:
	{
		r8 = &registers.d;
		break;
	}
	case 0xEB:
	{
		r8 = &registers.e;
		break;
	}
	case 0xEC:
	{
		r8 = &registers.h;
		break;
	}
	case 0xED:
	{
		r8 = &registers.l;
		break;
	}
	case 0xEE:
	{
		uint8_t value = read(registers.hl);
		value |= (1 << 5);

		write(registers.hl, value);

		cycles += 12;
		cyclesRan += 12;
		break;
	}
	case 0xEF:
	{
		r8 = &registers.a;
		break;
	}
	default:
		std::cout << "Unknown SET5r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr) {
		return;
	}

	*r8 |= (1 << 5);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::SET6r8()
{
	uint8_t opcode = read(pc++);

	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0xF0:
	{
		r8 = &registers.b;
		break;
	}
	case 0xF1:
	{
		r8 = &registers.c;
		break;
	}
	case 0xF2:
	{
		r8 = &registers.d;
		break;
	}
	case 0xF3:
	{
		r8 = &registers.e;
		break;
	}
	case 0xF4:
	{
		r8 = &registers.h;
		break;
	}
	case 0xF5:
	{
		r8 = &registers.l;
		break;
	}
	case 0xF6:
	{
		uint8_t value = read(registers.hl);
		value |= (1 << 6);

		write(registers.hl, value);

		cycles += 12;
		cyclesRan += 12;
		break;
	}
	case 0xF7:
	{
		r8 = &registers.a;
		break;
	}
	default:
		std::cout << "Unknown SET6r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr) {
		return;
	}

	*r8 |= (1 << 6);

	cycles += 4;
	cyclesRan += 4;
}

void Cpu::SET7r8()
{
	uint8_t opcode = read(pc++);

	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0xF8:
	{
		r8 = &registers.b;
		break;
	}
	case 0xF9:
	{
		r8 = &registers.c;
		break;
	}
	case 0xFA:
	{
		r8 = &registers.d;
		break;
	}
	case 0xFB:
	{
		r8 = &registers.e;
		break;
	}
	case 0xFC:
	{
		r8 = &registers.h;
		break;
	}
	case 0xFD:
	{
		r8 = &registers.l;
		break;
	}
	case 0xFE:
	{
		uint8_t value = read(registers.hl);
		value |= (1 << 7);

		write(registers.hl, value);

		cycles += 12;
		cyclesRan += 12;
		break;
	}
	case 0xFF:
	{
		r8 = &registers.a;
		break;
	}
	default:
		std::cout << "Unknown SET7r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr) {
		return;
	}

	*r8 |= (1 << 7);

	cycles += 4;
	cyclesRan += 4;
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