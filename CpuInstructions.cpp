#include "CpuInstructions.h"
#include <iostream>
#include <iomanip>



CpuInstructions::CpuInstructions()
{
	using a = CpuInstructions;

	lookup = {
		INSTRUCTION{ "NOP", &a::NOP}, INSTRUCTION{"LD BC, d16", &a::LDrrd16}, INSTRUCTION{"LD (BC), A", &a::LDarrR}, INSTRUCTION{"INC BC", &a::INCrr}, INSTRUCTION{"INC B", &a::INCr}, INSTRUCTION{"DEC B", &a::DECr}, INSTRUCTION{"LD B, d8", &a::LDrd8}, INSTRUCTION{"RLCA", &a::RLCA}, INSTRUCTION{"LD (a16), SP", &a::LDa16SP}, INSTRUCTION{"ADD HL, BC", &a::ADDrrRR}, INSTRUCTION{"LD A, (BC)", &a::LDraRR}, INSTRUCTION{"DEC BC", &a::DECrr}, INSTRUCTION{"INC C", &a::INCr}, INSTRUCTION{"DEC C", &a::DECr}, INSTRUCTION{"LD C, d8", &a::LDrd8}, INSTRUCTION{"RRCA", &a::RRCA},
		INSTRUCTION{ "STOP 0; UNIMPLEMENTED", &a::XXX }, INSTRUCTION{ "LD DE, d16", &a::LDrrd16 }, INSTRUCTION{ "LD (DE), d16", &a::LDarrR }, INSTRUCTION{ "INC DE", &a::INCrr }, INSTRUCTION{ "INC D", &a::INCr }, INSTRUCTION{ "DEC D", &a::DECr }, INSTRUCTION{ "LD D, d8", &a::LDrd8 }, INSTRUCTION{ "RLA", &a::RLA }, INSTRUCTION{ "JR r8", &a::JRcr8 }, INSTRUCTION{ "ADD HL, BC", &a::ADDrrRR }, INSTRUCTION{ "LD A, (DE)", &a::LDraRR }, INSTRUCTION{ "DEC DE", &a::DECrr }, INSTRUCTION{ "INC E", &a::INCr }, INSTRUCTION{ "DEC E", &a::DECr }, INSTRUCTION{ "LD E, d8", &a::LDrd8 }, INSTRUCTION{ "RRA", &a::RRA },
		INSTRUCTION{ "JR NZ, r8", &a::JRcr8 }, INSTRUCTION{ "LD HL, d16", &a::LDrrd16 }, INSTRUCTION{ "LD (HL+), d16", &a::LDarrR }, INSTRUCTION{ "INC HL", &a::INCrr }, INSTRUCTION{ "INC H", &a::INCr }, INSTRUCTION{ "DEC H", &a::DECr }, INSTRUCTION{ "LD H, d8", &a::LDrd8 }, INSTRUCTION{ "DAA", &a::DAA }, INSTRUCTION{ "JR Z r8", &a::JRcr8 }, INSTRUCTION{ "ADD HL, HL", &a::ADDrrRR }, INSTRUCTION{ "LD A, (HL+)", &a::LDraRR }, INSTRUCTION{ "DEC HL", &a::DECrr }, INSTRUCTION{ "INC L", &a::INCr }, INSTRUCTION{ "DEC L", &a::DECr }, INSTRUCTION{ "LD L, d8", &a::LDrd8 }, INSTRUCTION{ "CPL", &a::CPL },
		INSTRUCTION{ "JR NC, r8", &a::JRcr8 }, INSTRUCTION{ "LD SP, d16", &a::LDrrd16 }, INSTRUCTION{ "LD (HL-), d16", &a::LDarrR }, INSTRUCTION{ "INC SP", &a::INCrr }, INSTRUCTION{ "INC (HL)", &a::INCHL }, INSTRUCTION{ "DEC (HL)", &a::DECHL }, INSTRUCTION{ "LD (HL), d8", &a::LDrd8 }, INSTRUCTION{ "SCF", &a::SCF }, INSTRUCTION{ "JR C r8", &a::JRcr8 }, INSTRUCTION{ "ADD HL, SP", &a::ADDrrRR }, INSTRUCTION{ "LD A, (HL-)", &a::LDraRR }, INSTRUCTION{ "DEC SP", &a::DECrr }, INSTRUCTION{ "INC A", &a::INCr }, INSTRUCTION{ "DEC A", &a::DECr }, INSTRUCTION{ "LD A, d8", &a::LDrd8 }, INSTRUCTION{ "CCF", &a::CCF },
		INSTRUCTION{ "LD B, B", &a::LDrR }, INSTRUCTION{ "LD B, C", &a::LDrR }, INSTRUCTION{ "LD B, D", &a::LDrR }, INSTRUCTION{ "LD B, E", &a::LDrR }, INSTRUCTION{ "LD B, H", &a::LDrR }, INSTRUCTION{ "LD B, L", &a::LDrR }, INSTRUCTION{ "LD B, (HL)", &a::LDrR }, INSTRUCTION{ "LD B, A", &a::LDrR }, INSTRUCTION{ "LD C, B", &a::LDrR }, INSTRUCTION{ "LD C, C", &a::LDrR }, INSTRUCTION{ "LD C, D", &a::LDrR }, INSTRUCTION{ "LD C, E", &a::LDrR }, INSTRUCTION{ "LD C, H", &a::LDrR }, INSTRUCTION{ "LD C, L", &a::LDrR }, INSTRUCTION{ "LD C, (HL)", &a::LDrR }, INSTRUCTION{ "LD C, A", &a::LDrR },
		INSTRUCTION{ "LD D, B", &a::LDrR }, INSTRUCTION{ "LD D, C", &a::LDrR }, INSTRUCTION{ "LD D, D", &a::LDrR }, INSTRUCTION{ "LD D, E", &a::LDrR }, INSTRUCTION{ "LD D, H", &a::LDrR }, INSTRUCTION{ "LD D, L", &a::LDrR }, INSTRUCTION{ "LD D, (HL)", &a::LDrR }, INSTRUCTION{ "LD D, A", &a::LDrR }, INSTRUCTION{ "LD E, B", &a::LDrR }, INSTRUCTION{ "LD E, C", &a::LDrR }, INSTRUCTION{ "LD E, D", &a::LDrR }, INSTRUCTION{ "LD E, E", &a::LDrR }, INSTRUCTION{ "LD E, H", &a::LDrR }, INSTRUCTION{ "LD E, L", &a::LDrR }, INSTRUCTION{ "LD E, (HL)", &a::LDrR }, INSTRUCTION{ "LD E, A", &a::LDrR },
		INSTRUCTION{ "LD H, B", &a::LDrR }, INSTRUCTION{ "LD H, C", &a::LDrR }, INSTRUCTION{ "LD H, D", &a::LDrR }, INSTRUCTION{ "LD H, E", &a::LDrR }, INSTRUCTION{ "LD H, H", &a::LDrR }, INSTRUCTION{ "LD H, L", &a::LDrR }, INSTRUCTION{ "LD H, (HL)", &a::LDrR }, INSTRUCTION{ "LD H, A", &a::LDrR }, INSTRUCTION{ "LD L, B", &a::LDrR }, INSTRUCTION{ "LD L, C", &a::LDrR }, INSTRUCTION{ "LD L, D", &a::LDrR }, INSTRUCTION{ "LD L, E", &a::LDrR }, INSTRUCTION{ "LD L, H", &a::LDrR }, INSTRUCTION{ "LD L, L", &a::LDrR }, INSTRUCTION{ "LD L, (HL)", &a::LDrR }, INSTRUCTION{ "LD L, A", &a::LDrR },
		INSTRUCTION{ "LD (HL), B", &a::LDarrR }, INSTRUCTION{ "LD (HL), C", &a::LDarrR }, INSTRUCTION{ "LD (HL), D", &a::LDarrR }, INSTRUCTION{ "LD (HL), E", &a::LDarrR }, INSTRUCTION{ "LD (HL), H", &a::LDarrR }, INSTRUCTION{ "LD (HL), L", &a::LDarrR }, INSTRUCTION{ "HALT", &a::HALT }, INSTRUCTION{ "LD (HL), A", &a::LDarrR }, INSTRUCTION{ "LD A, B", &a::LDrR }, INSTRUCTION{ "LD A, C", &a::LDrR }, INSTRUCTION{ "LD A, D", &a::LDrR }, INSTRUCTION{ "LD A, E", &a::LDrR }, INSTRUCTION{ "LD A, H", &a::LDrR }, INSTRUCTION{ "LD A, L", &a::LDrR }, INSTRUCTION{ "LD A, (HL)", &a::LDrR }, INSTRUCTION{ "LD A, A", &a::LDrR },
		INSTRUCTION{ "ADD A, B", &a::ADDrR }, INSTRUCTION{ "ADD A, C", &a::ADDrR }, INSTRUCTION{ "ADD A, D", &a::ADDrR }, INSTRUCTION{ "ADD A, E", &a::ADDrR }, INSTRUCTION{ "ADD A, H", &a::ADDrR }, INSTRUCTION{ "ADD A, L", &a::ADDrR }, INSTRUCTION{ "ADD A, (HL)", &a::ADDrR }, INSTRUCTION{ "ADD A, A", &a::ADDrR }, INSTRUCTION{ "ADC A, B", &a::ADDrR }, INSTRUCTION{ "ADC A, C", &a::ADDrR }, INSTRUCTION{ "ADC A, D", &a::ADDrR }, INSTRUCTION{ "ADC A, E", &a::ADDrR }, INSTRUCTION{ "ADC A, H", &a::ADDrR }, INSTRUCTION{ "ADC A, L", &a::ADDrR }, INSTRUCTION{ "ADC A, (HL)", &a::ADDrR }, INSTRUCTION{ "ADC A, A", &a::ADDrR },
		INSTRUCTION{ "SUB B" , &a::SUBr }, INSTRUCTION{ "SUB C", &a::SUBr }, INSTRUCTION{ "SUB D", &a::SUBr }, INSTRUCTION{ "SUB E", &a::SUBr }, INSTRUCTION{ "SUB H", &a::SUBr }, INSTRUCTION{ "SUB L", &a::SUBr }, INSTRUCTION{ "SUB (HL)", &a::SUBr }, INSTRUCTION{ "SUB A", &a::SUBr }, INSTRUCTION{ "SBC A, B", &a::SUBr }, INSTRUCTION{ "SBC A, C", &a::SUBr }, INSTRUCTION{ "SBC A, D", &a::SUBr }, INSTRUCTION{ "SBC A, E", &a::SUBr }, INSTRUCTION{ "SBC A, H", &a::SUBr }, INSTRUCTION{ "SBC A, L", &a::SUBr }, INSTRUCTION{ "SBC A, (HL)", &a::SUBr }, INSTRUCTION{ "SBC A, A", &a::SUBr },
		INSTRUCTION{ "AND B", &a::ANDr }, INSTRUCTION{ "AND C", &a::ANDr }, INSTRUCTION{ "AND D", &a::ANDr }, INSTRUCTION{ "AND E", &a::ANDr }, INSTRUCTION{ "AND H", &a::ANDr }, INSTRUCTION{ "AND L", &a::ANDr }, INSTRUCTION{ "AND (HL)", &a::ANDr }, INSTRUCTION{ "AND A", &a::ANDr }, INSTRUCTION{ "XOR B", &a::XORr }, INSTRUCTION{ "XOR C", &a::XORr }, INSTRUCTION{ "XOR D", &a::XORr }, INSTRUCTION{ "XOR E", &a::XORr }, INSTRUCTION{ "XOR H", &a::XORr }, INSTRUCTION{ "XOR L", &a::XORr }, INSTRUCTION{ "XOR (HL)", &a::XORr }, INSTRUCTION{ "XOR A", &a::XORr },
		INSTRUCTION{ "OR B", &a::ORr }, INSTRUCTION{ "OR C", &a::ORr }, INSTRUCTION{ "OR D", &a::ORr }, INSTRUCTION{ "OR E", &a::ORr }, INSTRUCTION{ "OR H", &a::ORr }, INSTRUCTION{ "OR L", &a::ORr }, INSTRUCTION{ "OR (HL)", &a::ORr }, INSTRUCTION{ "OR A", &a::ORr }, INSTRUCTION{ "CP B", &a::CPr }, INSTRUCTION{ "CP C", &a::CPr }, INSTRUCTION{ "CP D", &a::CPr }, INSTRUCTION{ "CP E", &a::CPr }, INSTRUCTION{ "CP H", &a::CPr }, INSTRUCTION{ "CP L", &a::CPr }, INSTRUCTION{ "CP (HL)", &a::CPr }, INSTRUCTION{ "CP A", &a::CPr },
		INSTRUCTION{ "RET NZ", &a::RETc }, INSTRUCTION{ "POP BC", &a::POPrr }, INSTRUCTION{ "JP NZ, a16", &a::JPca16 }, INSTRUCTION{ "JP a16", &a::JP16 }, INSTRUCTION{ "CALL NZ, a16", &a::CALLca16 }, INSTRUCTION{ "PUSH BC", &a::PUSHrr }, INSTRUCTION{ "ADD A, d8", &a::ADDrR }, INSTRUCTION{ "RST 00H", &a::RSTn }, INSTRUCTION{ "RET Z", &a::RETc }, INSTRUCTION{ "RET", &a::RETc }, INSTRUCTION{ "JP Z, a16", &a::JPca16 }, INSTRUCTION{ "PREFIX CB", &a::PREFIXCB }, INSTRUCTION{ "CALL Z, a16", &a::CALLca16 }, INSTRUCTION{ "CALL a16", &a::CALLca16 }, INSTRUCTION{ "ADC A, d8", &a::ADDrR }, INSTRUCTION{ "RST 08H", &a::RSTn },
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
}

CpuInstructions::~CpuInstructions()
{
}


void CpuInstructions::runInstruction(Cpu& cpu)
{
	(this->*lookup[cpu.read(cpu.pc)].operate)(cpu);
}

void CpuInstructions::runCBInstruction(Cpu& cpu)
{
	(this->*lookupCB[cpu.read(cpu.pc)].operate)(cpu);
}

std::string CpuInstructions::getInstructionName(Cpu& cpu, uint16_t pc)
{
	return lookup[cpu.read(pc)].name;
}

std::string CpuInstructions::getCBInstructionName(Cpu& cpu, uint16_t pc)
{
	return lookupCB[cpu.read(pc)].name;
}

void CpuInstructions::NOP(Cpu& cpu)
{
	cpu.pc++;
	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::LDrrd16(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc);
	cpu.pc++;
	uint8_t lo = cpu.read(cpu.pc);
	cpu.pc++;
	uint8_t hi = cpu.read(cpu.pc);
	cpu.pc++;

	uint16_t value = (hi << 8) | lo;

	switch (opcode)
	{
	case 0x01: // LD BC, d16
	{
		cpu.registers.bc = value;
		break;
	}
	case 0x11: // LD DE, d16
	{
		cpu.registers.de = value;
		break;
	}
	case 0x21: // LD HL, d16
	{
		cpu.registers.hl = value;
		break;
	}
	case 0x31: // LD SP, d16
	{
		cpu.stkp = value;
		break;
	}
	default:
		std::cout << "Unknown LDrrd16 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	cpu.cycles += 12;
	cpu.cyclesRan += 12;
}

void CpuInstructions::LDarrR(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc);
	cpu.pc++;

	switch (opcode)
	{
	case 0x02: // LD (BC), A
	{
		cpu.write(cpu.registers.bc, cpu.registers.a);
		break;
	}
	case 0x12: // LD (DE), A
	{
		cpu.write(cpu.registers.de, cpu.registers.a);
		break;
	}
	case 0x22: // LD (HL+), A
	{
		cpu.write(cpu.registers.hl++, cpu.registers.a);
		break;
	}
	case 0x32: // LD (HL-), A
	{
		cpu.write(cpu.registers.hl--, cpu.registers.a);
		break;
	}
	case 0x70: // LD (HL), B
	{
		cpu.write(cpu.registers.hl, cpu.registers.b);
		break;
	}
	case 0x71: // LD (HL), C
	{
		cpu.write(cpu.registers.hl, cpu.registers.c);
		break;
	}
	case 0x72: // LD (HL), D
	{
		cpu.write(cpu.registers.hl, cpu.registers.d);
		break;
	}
	case 0x73: // LD (HL), E
	{
		cpu.write(cpu.registers.hl, cpu.registers.e);
		break;
	}
	case 0x74: // LD (HL), H
	{
		cpu.write(cpu.registers.hl, cpu.registers.h);
		break;
	}
	case 0x75: // LD (HL), L
	{
		cpu.write(cpu.registers.hl, cpu.registers.l);
		break;
	}
	case 0x77: // LD (HL), A
	{
		cpu.write(cpu.registers.hl, cpu.registers.a);
		break;
	}
	default:
		std::cout << "Unknown LDarrR instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	cpu.cycles += 8;
	cpu.cyclesRan += 8;
}

void CpuInstructions::INCrr(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc);
	cpu.pc++;

	switch (opcode)
	{
	case 0x03: // INC BC
	{
		cpu.registers.bc++;
		break;
	}
	case 0x13: // INC DE
	{
		cpu.registers.de++;
		break;
	}
	case 0x23: // INC HL
	{
		cpu.registers.hl++;
		break;
	}
	case 0x33: // INC SP (Stack Pointer)
	{
		cpu.stkp++;
		break;
	}
	default:
		std::cout << "Unknown INCrr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	cpu.cycles += 8;
	cpu.cyclesRan += 8;
}

void CpuInstructions::INCr(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc);
	cpu.pc++;

	uint8_t* regPtr = nullptr;

	switch (opcode)
	{
	case 0x04: // INC B
	{
		regPtr = &cpu.registers.b;
		break;
	}
	case 0x14: // INC D
	{
		regPtr = &cpu.registers.d;
		break;
	}
	case 0x24: // INC H
	{
		regPtr = &cpu.registers.h;
		break;
	}
	case 0x0C: // INC C
	{
		regPtr = &cpu.registers.c;
		break;
	}
	case 0x1C: // INC E
	{
		regPtr = &cpu.registers.e;
		break;
	}
	case 0x2C: // INC L
	{
		regPtr = &cpu.registers.l;
		break;
	}
	case 0x3C: // INC A
	{
		regPtr = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown INCr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	if (regPtr != nullptr)
	{
		(*regPtr)++;

		cpu.setFlag(cpu.Zero, (*regPtr) == 0);
		cpu.setFlag(cpu.Subtraction, false);
		cpu.setFlag(cpu.HalfCarry, ((*regPtr) & 0x0F) == 0);
	}

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::DECr(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	switch (opcode)
	{
	case 0x05: // DEC B
	{
		uint8_t value = cpu.registers.b;
		value--;
		cpu.registers.b = value;

		cpu.setFlag(cpu.Zero, cpu.registers.b == 0x00);
		cpu.setFlag(cpu.Subtraction, true);
		cpu.setFlag(cpu.HalfCarry, (cpu.registers.b & 0x0F) == 0x0F);

		break;
	}
	case 0x15: // DEC D
	{
		uint8_t value = cpu.registers.d;
		value--;
		cpu.registers.d = value;

		cpu.setFlag(cpu.Zero, cpu.registers.d == 0x00);
		cpu.setFlag(cpu.Subtraction, true);
		cpu.setFlag(cpu.HalfCarry, (cpu.registers.d & 0x0F) == 0x0F);

		break;
	}
	case 0x25: // DEC H
	{
		uint8_t value = cpu.registers.h;
		value--;
		cpu.registers.h = value;

		cpu.setFlag(cpu.Zero, cpu.registers.h == 0x00);
		cpu.setFlag(cpu.Subtraction, true);
		cpu.setFlag(cpu.HalfCarry, (cpu.registers.h & 0x0F) == 0x0F);

		break;
	}
	case 0x0D: // DEC C
	{
		uint8_t value = cpu.registers.c;
		value--;
		cpu.registers.c = value;

		cpu.setFlag(cpu.Zero, cpu.registers.c == 0x00);
		cpu.setFlag(cpu.Subtraction, true);
		cpu.setFlag(cpu.HalfCarry, (cpu.registers.c & 0x0F) == 0x0F);

		break;
	}
	case 0x1D: // DEC E
	{
		uint8_t value = cpu.registers.e;
		value--;
		cpu.registers.e = value;

		cpu.setFlag(cpu.Zero, cpu.registers.e == 0x00);
		cpu.setFlag(cpu.Subtraction, true);
		cpu.setFlag(cpu.HalfCarry, (cpu.registers.e & 0x0F) == 0x0F);

		break;
	}
	case 0x2D: // DEC L
	{
		uint8_t value = cpu.registers.l;
		value--;
		cpu.registers.l = value;

		cpu.setFlag(cpu.Zero, cpu.registers.l == 0x00);
		cpu.setFlag(cpu.Subtraction, true);
		cpu.setFlag(cpu.HalfCarry, (cpu.registers.l & 0x0F) == 0x0F);

		break;
	}
	case 0x3D: // DEC A
	{
		uint8_t value = cpu.registers.a;
		value--;
		cpu.registers.a = value;

		cpu.setFlag(cpu.Zero, cpu.registers.a == 0x00);
		cpu.setFlag(cpu.Subtraction, true);
		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) == 0x0F);

		break;
	}
	default:
		std::cout << "Unknown DECr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::LDrd8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc);
	cpu.pc++;

	uint8_t value = cpu.read(cpu.pc);
	cpu.pc++;

	switch (opcode)
	{
	case 0x06:
	{
		cpu.registers.b = value;
		break;
	}
	case 0x16:
	{
		cpu.registers.d = value;
		break;
	}
	case 0x26:
	{
		cpu.registers.h = value;
		break;
	}
	case 0x36:
	{
		cpu.write(cpu.registers.hl, value);
		cpu.cycles += 4;
		cpu.cyclesRan += 4;
		break;
	}
	case 0x0E:
	{
		cpu.registers.c = value;
		break;
	}
	case 0x1E:
	{
		cpu.registers.e = value;
		break;
	}
	case 0x2E:
	{
		cpu.registers.l = value;
		break;
	}
	case 0x3E:
	{
		cpu.registers.a = value;
		break;
	}
	default:
		std::cout << "Unknown LDrd8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	cpu.cycles += 8;
	cpu.cyclesRan += 8;
}

void CpuInstructions::RLCA(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	bool carry = (cpu.registers.a & 0x80) != 0;

	cpu.registers.a <<= 1;
	cpu.registers.a |= carry;

	cpu.setFlag(cpu.Zero, false);
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, false);
	cpu.setFlag(cpu.Carry, carry);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::LDa16SP(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	uint8_t lo = cpu.read(cpu.pc++);
	uint8_t hi = cpu.read(cpu.pc++);

	uint16_t a16 = (hi << 8) | lo;

	cpu.write(a16, cpu.stkp & 0xFF);
	cpu.write(a16 + 1, (cpu.stkp >> 8) & 0xFF);

	cpu.cycles += 20;
	cpu.cyclesRan += 20;
}

void CpuInstructions::ADDrrRR(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	uint16_t* r16 = nullptr;

	switch (opcode)
	{
	case 0x09: // ADD HL, BC
	{
		r16 = &cpu.registers.bc;
		break;
	}
	case 0x19: // ADD HL, DE
	{
		r16 = &cpu.registers.de;
		break;
	}
	case 0x29: // ADD HL, HL
	{
		r16 = &cpu.registers.hl;
		break;
	}
	case 0x39: // ADD HL, SP
	{
		r16 = &cpu.stkp;
		break;
	}
	default:
		std::cout << "Unknown ADDrrRR instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	if (r16 == nullptr)
	{
		cpu.setHasNotBroken(false);
		return;
	}

	uint32_t result = static_cast<uint32_t>(cpu.registers.hl) + static_cast<uint32_t>(*r16);

	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, ((cpu.registers.hl & 0xFFF) + (*r16 & 0xFFF)) > 0xFFF);
	cpu.setFlag(cpu.Carry, result > 0xFFFF);

	cpu.registers.hl = static_cast<uint16_t>(result & 0xFFFF);

	cpu.cycles += 8;
	cpu.cyclesRan += 8;
}

void CpuInstructions::DECrr(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc);
	cpu.pc++;

	switch (opcode)
	{
	case 0x0B: // DEC BC
	{
		cpu.registers.bc--;
		break;
	}
	case 0x1B: // DEC DE
	{
		cpu.registers.de--;
		break;
	}
	case 0x2B: // DEC HL
	{
		cpu.registers.hl--;
		break;
	}
	case 0x3B: // DEC SP (Stack Pointer)
	{
		cpu.stkp--;
		break;
	}
	default:
		std::cout << "Unknown DECrr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	cpu.cycles += 8;
	cpu.cyclesRan += 8;
}

void CpuInstructions::RRCA(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	bool carry = cpu.registers.a & 0x01;

	cpu.registers.a >>= 1;
	cpu.registers.a |= (carry << 7);

	cpu.setFlag(cpu.Zero, false);
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, false);
	cpu.setFlag(cpu.Carry, carry);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::STOP0(Cpu& cpu)
{
}

void CpuInstructions::RLA(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	bool carry = cpu.isFlagSet(cpu.Carry);

	cpu.setFlag(cpu.Carry, (cpu.registers.a & 0x80) != 0);
	cpu.registers.a = (cpu.registers.a << 1) | carry;

	cpu.setFlag(cpu.Zero, false);
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, false);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::JRr8(Cpu& cpu)
{
}

void CpuInstructions::RRA(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	bool carry = cpu.isFlagSet(cpu.Carry);
	bool new_carry = cpu.registers.a & 0x01;

	cpu.registers.a = (cpu.registers.a >> 1) | (carry << 7);

	cpu.setFlag(cpu.Zero, false);
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, false);
	cpu.setFlag(cpu.Carry, new_carry);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::DAA(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	uint8_t value = cpu.registers.a;

	bool carry = cpu.isFlagSet(cpu.Carry);
	bool halfCarry = cpu.isFlagSet(cpu.HalfCarry);
	bool subtract = cpu.isFlagSet(cpu.Subtraction);

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
			cpu.setFlag(cpu.Carry, true);
		}
		if (halfCarry || (value & 0x0F) > 0x09)
		{
			value += 0x06;
		}
	}

	cpu.registers.a = value;
	cpu.setFlag(cpu.Zero, value == 0);
	cpu.setFlag(cpu.HalfCarry, false);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::CPL(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	cpu.registers.a = ~cpu.registers.a;

	cpu.setFlag(cpu.Subtraction, true);
	cpu.setFlag(cpu.HalfCarry, true);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::INCaRR(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc);
	cpu.pc++;

	switch (opcode)
	{
	case 0x34: // INC (HL)
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		value++;
		cpu.write(cpu.registers.hl, value);

		cpu.setFlag(cpu.Zero, value == 0);
		cpu.setFlag(cpu.Subtraction, false);
		cpu.setFlag(cpu.HalfCarry, (value & 0x0F) == 0);

		break;
	}
	default:
		std::cout << "Unknown INCaRR instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	cpu.cycles += 12;
	cpu.cyclesRan += 12;
}

void CpuInstructions::DECaRR(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc);
	cpu.pc++;

	switch (opcode)
	{
	case 0x35: // DEC (HL)
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		value--;
		cpu.write(cpu.registers.hl, value);

		cpu.setFlag(cpu.Zero, value == 0);
		cpu.setFlag(cpu.Subtraction, true);
		cpu.setFlag(cpu.HalfCarry, (value & 0x0F) == 0);

		break;
	}
	default:
		std::cout << "Unknown DECaRR instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	cpu.cycles += 12;
	cpu.cyclesRan += 12;
}

void CpuInstructions::SCF(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, false);
	cpu.setFlag(cpu.Carry, true);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::CCF(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	uint8_t carry = !cpu.isFlagSet(cpu.Carry);

	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, false);
	cpu.setFlag(cpu.Carry, carry);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::LDrR(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc);
	cpu.pc++;

	switch (opcode)
	{
	case 0x40:
	{
		cpu.registers.b = cpu.registers.b;
		break;
	}
	case 0x41:
	{
		cpu.registers.b = cpu.registers.c;
		break;
	}
	case 0x42:
	{
		cpu.registers.b = cpu.registers.d;
		break;
	}
	case 0x43:
	{
		cpu.registers.b = cpu.registers.e;
		break;
	}
	case 0x44:
	{
		cpu.registers.b = cpu.registers.h;
		break;
	}
	case 0x45:
	{
		cpu.registers.b = cpu.registers.l;
		break;
	}
	case 0x46:
	{
		cpu.registers.b = cpu.read(cpu.registers.hl);
		cpu.cycles += 4;
		cpu.cyclesRan += 4;
		break;
	}
	case 0x47:
	{
		cpu.registers.b = cpu.registers.a;
		break;
	}
	case 0x48:
	{
		cpu.registers.c = cpu.registers.b;
		break;
	}
	case 0x49:
	{
		cpu.registers.c = cpu.registers.c;
		break;
	}
	case 0x4A:
	{
		cpu.registers.c = cpu.registers.d;
		break;
	}
	case 0x4B:
	{
		cpu.registers.c = cpu.registers.e;
		break;
	}
	case 0x4C:
	{
		cpu.registers.c = cpu.registers.h;
		break;
	}
	case 0x4D:
	{
		cpu.registers.c = cpu.registers.l;
		break;
	}
	case 0x4E:
	{
		cpu.registers.c = cpu.read(cpu.registers.hl);
		cpu.cycles += 4;
		cpu.cyclesRan += 4;
		break;
	}
	case 0x4F:
	{
		cpu.registers.c = cpu.registers.a;
		break;
	}
	case 0x50:
	{
		cpu.registers.d = cpu.registers.b;
		break;
	}
	case 0x51:
	{
		cpu.registers.d = cpu.registers.c;
		break;
	}
	case 0x52:
	{
		cpu.registers.d = cpu.registers.d;
		break;
	}
	case 0x53:
	{
		cpu.registers.d = cpu.registers.e;
		break;
	}
	case 0x54:
	{
		cpu.registers.d = cpu.registers.h;
		break;
	}
	case 0x55:
	{
		cpu.registers.d = cpu.registers.l;
		break;
	}
	case 0x56:
	{
		cpu.registers.d = cpu.read(cpu.registers.hl);
		cpu.cycles += 4;
		cpu.cyclesRan += 4;
		break;
	}
	case 0x57:
	{
		cpu.registers.d = cpu.registers.a;
		break;
	}
	case 0x58:
	{
		cpu.registers.e = cpu.registers.b;
		break;
	}
	case 0x59:
	{
		cpu.registers.e = cpu.registers.c;
		break;
	}
	case 0x5A:
	{
		cpu.registers.e = cpu.registers.d;
		break;
	}
	case 0x5B:
	{
		cpu.registers.e = cpu.registers.e;
		break;
	}
	case 0x5C:
	{
		cpu.registers.e = cpu.registers.h;
		break;
	}
	case 0x5D:
	{
		cpu.registers.e = cpu.registers.l;
		break;
	}
	case 0x5E:
	{
		cpu.registers.e = cpu.read(cpu.registers.hl);
		cpu.cycles += 4;
		cpu.cyclesRan += 4;
		break;
	}
	case 0x5F:
	{
		cpu.registers.e = cpu.registers.a;
		break;
	}
	case 0x60:
	{
		cpu.registers.h = cpu.registers.b;
		break;
	}
	case 0x61:
	{
		cpu.registers.h = cpu.registers.c;
		break;
	}
	case 0x62:
	{
		cpu.registers.h = cpu.registers.d;
		break;
	}
	case 0x63:
	{
		cpu.registers.h = cpu.registers.e;
		break;
	}
	case 0x64:
	{
		cpu.registers.h = cpu.registers.h;
		break;
	}
	case 0x65:
	{
		cpu.registers.h = cpu.registers.l;
		break;
	}
	case 0x66:
	{
		cpu.registers.h = cpu.read(cpu.registers.hl);
		cpu.cycles += 4;
		cpu.cyclesRan += 4;
		break;
	}
	case 0x67:
	{
		cpu.registers.h = cpu.registers.a;
		break;
	}
	case 0x68:
	{
		cpu.registers.l = cpu.registers.b;
		break;
	}
	case 0x69:
	{
		cpu.registers.l = cpu.registers.c;
		break;
	}
	case 0x6A:
	{
		cpu.registers.l = cpu.registers.d;
		break;
	}
	case 0x6B:
	{
		cpu.registers.l = cpu.registers.e;
		break;
	}
	case 0x6C:
	{
		cpu.registers.l = cpu.registers.h;
		break;
	}
	case 0x6D:
	{
		cpu.registers.l = cpu.registers.l;
		break;
	}
	case 0x6E:
	{
		cpu.registers.l = cpu.read(cpu.registers.hl);
		cpu.cycles += 4;
		cpu.cyclesRan += 4;
		break;
	}
	case 0x6F:
	{
		cpu.registers.l = cpu.registers.a;
		break;
	}
	case 0x78:
	{
		cpu.registers.a = cpu.registers.b;
		break;
	}
	case 0x79:
	{
		cpu.registers.a = cpu.registers.c;
		break;
	}
	case 0x7A:
	{
		cpu.registers.a = cpu.registers.d;
		break;
	}
	case 0x7B:
	{
		cpu.registers.a = cpu.registers.e;
		break;
	}
	case 0x7C:
	{
		cpu.registers.a = cpu.registers.h;
		break;
	}
	case 0x7D:
	{
		cpu.registers.a = cpu.registers.l;
		break;
	}
	case 0x7E:
	{
		cpu.registers.a = cpu.read(cpu.registers.hl);
		break;
	}
	case 0x7F:
	{
		cpu.registers.a = cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown LDrR instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::HALT(Cpu& cpu)
{
	cpu.pc++;
	cpu.halted = true;
	cpu.cycles += 4;
	cpu.cyclesRan += 4;

	std::cout << "CPU HALTED" << std::endl;
}

void CpuInstructions::DECB(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	uint8_t value = cpu.registers.b;
	value--;
	cpu.registers.b = value;

	cpu.setFlag(cpu.Zero, cpu.registers.b == 0x00);
	cpu.setFlag(cpu.Subtraction, true);
	cpu.setFlag(cpu.HalfCarry, (cpu.registers.b & 0x0F) == 0x0F);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::DECHL(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	uint8_t value = cpu.read(cpu.registers.hl);
	value--;
	cpu.write(cpu.registers.hl, value);

	cpu.setFlag(cpu.Zero, value == 0);
	cpu.setFlag(cpu.Subtraction, true);
	cpu.setFlag(cpu.HalfCarry, (value & 0x0F) == 0);

	cpu.cycles += 12;
	cpu.cyclesRan += 12;
}

void CpuInstructions::ADDrR(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc);
	cpu.pc++;

	uint8_t carry = cpu.isFlagSet(cpu.Carry) ? 1 : 0;
	uint16_t result = 0;

	switch (opcode)
	{
	case 0x80:
	{
		result = cpu.registers.a + cpu.registers.b;
		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.b & 0x0F) > 0x0F);
		break;
	}
	case 0x81:
	{
		result = cpu.registers.a + cpu.registers.c;
		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.c & 0x0F) > 0x0F);
		break;
	}
	case 0x82:
	{
		result = cpu.registers.a + cpu.registers.d;
		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.d & 0x0F) > 0x0F);
		break;
	}
	case 0x83:
	{
		result = cpu.registers.a + cpu.registers.e;
		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.e & 0x0F) > 0x0F);
		break;
	}
	case 0x84:
	{
		result = cpu.registers.a + cpu.registers.h;
		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.h & 0x0F) > 0x0F);
		break;
	}
	case 0x85:
	{
		result = cpu.registers.a + cpu.registers.l;
		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.l & 0x0F) > 0x0F);
		break;
	}
	case 0x86:
	{
		result = cpu.registers.a + cpu.read(cpu.registers.hl);
		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.read(cpu.registers.hl) & 0x0F) > 0x0F);

		cpu.cycles += 4;
		cpu.cyclesRan += 4;
		break;
	}
	case 0x87:
	{
		result = cpu.registers.a + cpu.registers.a;
		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.a & 0x0F) > 0x0F);
		break;
	}
	case 0x88:
	{
		result = cpu.registers.a + cpu.registers.b + carry;

		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.b & 0x0F) + carry > 0x0F);
		break;
	}
	case 0x89:
	{
		result = cpu.registers.a + cpu.registers.c + carry;

		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.c & 0x0F) + carry > 0x0F);
		break;
	}
	case 0x8A:
	{
		result = cpu.registers.a + cpu.registers.d + carry;

		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.d & 0x0F) + carry > 0x0F);
		break;
	}
	case 0x8B:
	{
		result = cpu.registers.a + cpu.registers.e + carry;

		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.e & 0x0F) + carry > 0x0F);
		break;
	}
	case 0x8C:
	{
		result = cpu.registers.a + cpu.registers.h + carry;

		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.h & 0x0F) + carry > 0x0F);
		break;
	}
	case 0x8D:
	{
		result = cpu.registers.a + cpu.registers.l + carry;

		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.l & 0x0F) + carry > 0x0F);
		break;
	}
	case 0x8E:
	{
		result = cpu.registers.a + cpu.read(cpu.registers.hl) + carry;

		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.read(cpu.registers.hl) & 0x0F) + carry > 0x0F);

		cpu.cycles += 4;
		cpu.cycles += 4;
		break;
	}
	case 0x8F:
	{
		result = cpu.registers.a + cpu.registers.a + carry;

		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.a & 0x0F) + carry > 0x0F);
		break;
	}
	case 0xC6:
	{
		uint8_t d8 = cpu.read(cpu.pc++);

		result = cpu.registers.a + d8;

		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (d8 & 0x0F) > 0x0F);

		cpu.cycles += 4;
		cpu.cyclesRan += 4;
		break;
	}
	case 0xCE:
	{
		uint8_t d8 = cpu.read(cpu.pc++);
		result = cpu.registers.a + d8 + carry;

		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (d8 & 0x0F) + carry > 0x0F);

		cpu.cycles += 4;
		cpu.cyclesRan += 4;
		break;
	}
	default:
		std::cout << "Unknown ADDrR instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	cpu.setFlag(cpu.Zero, (result & 0xFF) == 0x00);
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.Carry, result > 0xFF);

	cpu.registers.a = result & 0xFF;

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::SUBr(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc);
	cpu.pc++;

	uint8_t result;

	switch (opcode)
	{
	case 0x90:
	{
		result = cpu.registers.a - cpu.registers.b;
		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.b & 0x0F));
		cpu.setFlag(cpu.Carry, cpu.registers.b > cpu.registers.a);

		break;
	}
	case 0x91:
	{
		result = cpu.registers.a - cpu.registers.c;
		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.c & 0x0F));
		cpu.setFlag(cpu.Carry, cpu.registers.c > cpu.registers.a);

		break;
	}
	case 0x92:
	{
		result = cpu.registers.a - cpu.registers.d;
		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.d & 0x0F));
		cpu.setFlag(cpu.Carry, cpu.registers.d > cpu.registers.a);

		break;
	}
	case 0x93:
	{
		result = cpu.registers.a - cpu.registers.e;
		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.e & 0x0F));
		cpu.setFlag(cpu.Carry, cpu.registers.e > cpu.registers.a);

		break;
	}
	case 0x94:
	{
		result = cpu.registers.a - cpu.registers.h;
		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.h & 0x0F));
		cpu.setFlag(cpu.Carry, cpu.registers.h > cpu.registers.a);

		break;
	}
	case 0x95:
	{
		result = cpu.registers.a - cpu.registers.l;
		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.l & 0x0F));
		cpu.setFlag(cpu.Carry, cpu.registers.l > cpu.registers.a);

		break;
	}
	case 0x96:
	{
		result = cpu.registers.a - cpu.read(cpu.registers.hl);
		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.read(cpu.registers.hl) & 0x0F));
		cpu.setFlag(cpu.Carry, cpu.read(cpu.registers.hl) > cpu.registers.a);

		break;
	}
	case 0x97:
	{
		result = cpu.registers.a - cpu.registers.a;
		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.a & 0x0F));
		cpu.setFlag(cpu.Carry, cpu.registers.a > cpu.registers.a);

		cpu.cycles += 4;
		cpu.cyclesRan += 4;

		break;
	}
	case 0x98:
	{
		uint8_t carry = cpu.isFlagSet(cpu.Carry) ? 1 : 0;
		result = cpu.registers.a - cpu.registers.b - carry;

		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.b & 0x0F) + carry);
		cpu.setFlag(cpu.Carry, cpu.registers.b + carry > cpu.registers.a);

		break;
	}
	case 0x99:
	{
		uint8_t carry = cpu.isFlagSet(cpu.Carry) ? 1 : 0;
		result = cpu.registers.a - cpu.registers.c - carry;

		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.c & 0x0F) + carry);
		cpu.setFlag(cpu.Carry, cpu.registers.c + carry > cpu.registers.a);

		break;
	}
	case 0x9A:
	{
		uint8_t carry = cpu.isFlagSet(cpu.Carry) ? 1 : 0;
		result = cpu.registers.a - cpu.registers.d - carry;

		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.d & 0x0F) + carry);
		cpu.setFlag(cpu.Carry, cpu.registers.d + carry > cpu.registers.a);

		break;
	}
	case 0x9B:
	{
		uint8_t carry = cpu.isFlagSet(cpu.Carry) ? 1 : 0;
		result = cpu.registers.a - cpu.registers.e - carry;

		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.e & 0x0F) + carry);
		cpu.setFlag(cpu.Carry, cpu.registers.e + carry > cpu.registers.a);

		break;
	}
	case 0x9C:
	{
		uint8_t carry = cpu.isFlagSet(cpu.Carry) ? 1 : 0;
		result = cpu.registers.a - cpu.registers.h - carry;

		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.h & 0x0F) + carry);
		cpu.setFlag(cpu.Carry, cpu.registers.h + carry > cpu.registers.a);

		break;
	}
	case 0x9D:
	{
		uint8_t carry = cpu.isFlagSet(cpu.Carry) ? 1 : 0;
		result = cpu.registers.a - cpu.registers.l - carry;

		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.l & 0x0F) + carry);
		cpu.setFlag(cpu.Carry, cpu.registers.l + carry > cpu.registers.a);

		break;
	}
	case 0x9E:
	{
		uint8_t carry = cpu.isFlagSet(cpu.Carry) ? 1 : 0;
		result = cpu.registers.a - cpu.read(cpu.registers.hl) - carry;

		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.read(cpu.registers.hl) & 0x0F) + carry);
		cpu.setFlag(cpu.Carry, cpu.read(cpu.registers.hl) + carry > cpu.registers.a);

		cpu.cycles += 4;
		cpu.cyclesRan += 4;

		break;
	}
	case 0x9F:
	{
		uint8_t carry = cpu.isFlagSet(cpu.Carry) ? 1 : 0;
		result = cpu.registers.a - cpu.registers.a - carry;

		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.a & 0x0F) + carry);
		cpu.setFlag(cpu.Carry, cpu.registers.a + carry > cpu.registers.a);

		break;
	}
	case 0xD6:
	{
		uint8_t d8 = cpu.read(cpu.pc++);

		result = cpu.registers.a - d8;

		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (d8 & 0x0F));
		cpu.setFlag(cpu.Carry, d8 > cpu.registers.a);

		cpu.cycles += 4;
		cpu.cyclesRan += 4;
		break;
	}
	case 0xDE:
	{
		uint8_t d8 = cpu.read(cpu.pc++);

		uint8_t carry = cpu.isFlagSet(cpu.Carry) ? 1 : 0;
		result = cpu.registers.a - d8 - carry;

		cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (d8 & 0x0F) + carry);
		cpu.setFlag(cpu.Carry, d8 + carry > cpu.registers.a);

		cpu.cycles += 4;
		cpu.cyclesRan += 4;
		break;
	}
	default:
		std::cout << "Unknown SUBr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	cpu.setFlag(cpu.Zero, result == 0x00);
	cpu.setFlag(cpu.Subtraction, true);

	cpu.registers.a = result;

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::ANDr(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc);
	cpu.pc++;

	uint8_t hlValue;
	uint8_t d8;

	uint8_t* regPtr = nullptr;

	switch (opcode)
	{
	case 0xA0:
	{
		regPtr = &cpu.registers.b;
		break;
	}
	case 0xA1:
	{
		regPtr = &cpu.registers.c;
		break;
	}
	case 0xA2:
	{
		regPtr = &cpu.registers.d;
		break;
	}
	case 0xA3:
	{
		regPtr = &cpu.registers.e;
		break;
	}
	case 0xA4:
	{
		regPtr = &cpu.registers.h;
		break;
	}
	case 0xA5:
	{
		regPtr = &cpu.registers.l;
		break;
	}
	case 0xA6:
	{
		hlValue = cpu.read(cpu.registers.hl);
		regPtr = &hlValue;

		cpu.cycles += 4;
		cpu.cyclesRan += 4;

		break;
	}
	case 0xA7:
	{
		regPtr = &cpu.registers.a;
		break;
	}
	case 0xE6:
	{
		d8 = cpu.read(cpu.pc++);
		regPtr = &d8;

		cpu.cycles += 4;
		cpu.cyclesRan += 4;
		break;
	}
	default:
		std::cout << "Unknown ANDr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	if (regPtr == nullptr)
	{
		return;
	}

	cpu.registers.a &= (*regPtr);

	cpu.setFlag(cpu.Zero, cpu.registers.a == 0x00);
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, true);
	cpu.setFlag(cpu.Carry, false);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::XORr(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc);
	cpu.pc++;

	uint8_t d8;
	uint8_t hlValue;

	uint8_t* regPtr = nullptr;

	switch (opcode)
	{
	case 0xA8:
	{
		regPtr = &cpu.registers.b;
		break;
	}
	case 0xA9:
	{
		regPtr = &cpu.registers.c;
		break;
	}
	case 0xAA:
	{
		regPtr = &cpu.registers.d;
		break;
	}
	case 0xAB:
	{
		regPtr = &cpu.registers.e;
		break;
	}
	case 0xAC:
	{
		regPtr = &cpu.registers.h;
		break;
	}
	case 0xAD:
	{
		regPtr = &cpu.registers.l;
		break;
	}
	case 0xAE:
	{
		hlValue = cpu.read(cpu.registers.hl);

		regPtr = &hlValue;
		cpu.cycles += 4;
		cpu.cyclesRan += 4;
		break;
	}
	case 0xAF:
	{
		regPtr = &cpu.registers.a;
		break;
	}
	case 0xEE:
	{
		d8 = cpu.read(cpu.pc++);

		regPtr = &d8;

		cpu.cycles += 4;
		cpu.cyclesRan += 4;
		break;
	}
	default:
		std::cout << "Unknown XORr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	if (regPtr == nullptr)
	{
		return;
	}

	cpu.registers.a ^= (*regPtr);

	cpu.setFlag(cpu.Zero, cpu.registers.a == 0x00);
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, false);
	cpu.setFlag(cpu.Carry, false);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::POPAF(Cpu& cpu)
{
	cpu.pc++;

	uint8_t lo = cpu.read(cpu.stkp++);
	uint8_t hi = cpu.read(cpu.stkp++);

	cpu.registers.af = (hi << 8) | lo;
	cpu.registers.f &= 0xF0;

	cpu.cycles += 12;
	cpu.cyclesRan += 12;
}

void CpuInstructions::PUSHAF(Cpu& cpu)
{
	cpu.pc++;

	cpu.write(--cpu.stkp, (cpu.registers.af >> 8) & 0xFF);
	cpu.write(--cpu.stkp, cpu.registers.af & 0xFF);

	cpu.cycles += 16;
	cpu.cyclesRan += 16;
}

void CpuInstructions::ORr(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc);
	cpu.pc++;

	uint8_t hlValue;
	uint8_t d8;

	uint8_t* regPtr = nullptr;

	switch (opcode)
	{
	case 0xB0:
	{
		regPtr = &cpu.registers.b;
		break;
	}
	case 0xB1:
	{
		regPtr = &cpu.registers.c;
		break;
	}
	case 0xB2:
	{
		regPtr = &cpu.registers.d;
		break;
	}
	case 0xB3:
	{
		regPtr = &cpu.registers.e;
		break;
	}
	case 0xB4:
	{
		regPtr = &cpu.registers.h;
		break;
	}
	case 0xB5:
	{
		regPtr = &cpu.registers.l;
		break;
	}
	case 0xB6:
	{
		hlValue = cpu.read(cpu.registers.hl);

		regPtr = &hlValue;
		cpu.cycles += 4;
		cpu.cyclesRan += 4;
		break;
	}
	case 0xB7:
	{
		regPtr = &cpu.registers.a;
		break;
	}
	case 0xF6:
	{
		d8 = cpu.read(cpu.pc++);
		regPtr = &d8;

		cpu.cycles += 4;
		cpu.cyclesRan += 4;
		break;
	}
	default:
		std::cout << "Unknown ORr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	if (regPtr == nullptr)
	{
		return;
	}

	cpu.registers.a |= (*regPtr);

	cpu.setFlag(cpu.Zero, cpu.registers.a == 0x00);
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, false);
	cpu.setFlag(cpu.Carry, false);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::CPr(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	uint8_t d8;
	uint8_t hlValue;

	uint8_t* regPtr = nullptr;

	switch (opcode)
	{
	case 0xB8:
	{
		regPtr = &cpu.registers.b;
		break;
	}
	case 0xB9:
	{
		regPtr = &cpu.registers.c;
		break;
	}
	case 0xBA:
	{
		regPtr = &cpu.registers.d;
		break;
	}
	case 0xBB:
	{
		regPtr = &cpu.registers.e;
		break;
	}
	case 0xBC:
	{
		regPtr = &cpu.registers.h;
		break;
	}
	case 0xBD:
	{
		regPtr = &cpu.registers.l;
		break;
	}
	case 0xBE:
	{
		hlValue = cpu.read(cpu.registers.hl);
		regPtr = &hlValue;

		cpu.cycles += 4;
		cpu.cyclesRan += 4;
		break;
	}
	case 0xBF:
	{
		regPtr = &cpu.registers.a;
		break;
	}
	case 0xFE:
	{
		d8 = cpu.read(cpu.pc++);
		regPtr = &d8;

		cpu.cycles += 4;
		cpu.cyclesRan += 4;
		break;
	}
	default:
		std::cout << "Unknown CPr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	if (regPtr == nullptr)
	{
		return;
	}

	uint8_t result = cpu.registers.a - (*regPtr);

	cpu.setFlag(cpu.Zero, result == 0x00);
	cpu.setFlag(cpu.Subtraction, true);
	cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (*regPtr & 0x0F));
	cpu.setFlag(cpu.Carry, (*regPtr) > cpu.registers.a);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::LDraRR(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	switch (opcode)
	{
	case 0x0A:
	{
		cpu.registers.a = cpu.read(cpu.registers.bc);
		break;
	}
	case 0x1A:
	{
		cpu.registers.a = cpu.read(cpu.registers.de);
		break;
	}
	case 0x2A:
	{
		cpu.registers.a = cpu.read(cpu.registers.hl++);
		break;
	}
	case 0x3A:
	{
		cpu.registers.a = cpu.read(cpu.registers.hl--);
		break;
	}
	default:
		std::cout << "Unknown LDraRR instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	cpu.cycles += 8;
}

void CpuInstructions::JRcr8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	int8_t offset = static_cast<int8_t>(cpu.read(cpu.pc++));

	switch (opcode)
	{
	case 0x20:
	{
		if (!cpu.isFlagSet(cpu.Zero))
		{
			uint16_t targetAddress = static_cast<uint16_t>(cpu.pc) + offset;

			cpu.pc = targetAddress;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;
		}

		break;
	}
	case 0x30:
	{
		if (!cpu.isFlagSet(cpu.Carry))
		{
			uint16_t targetAddress = static_cast<uint16_t>(cpu.pc) + offset;

			cpu.pc = targetAddress;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;
		}

		break;
	}
	case 0x18:
	{
		uint16_t targetAddress = static_cast<uint16_t>(cpu.pc) + offset;

		cpu.pc = targetAddress;
		cpu.cycles += 4;
		cpu.cyclesRan += 4;

		break;
	}
	case 0x28:
	{
		if (cpu.isFlagSet(cpu.Zero))
		{
			uint16_t targetAddress = static_cast<uint16_t>(cpu.pc) + offset;

			cpu.pc = targetAddress;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;
		}

		break;
	}
	case 0x38:
	{
		if (cpu.isFlagSet(cpu.Carry))
		{
			uint16_t targetAddress = static_cast<uint16_t>(cpu.pc) + offset;

			cpu.pc = targetAddress;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;
		}

		break;
	}
	default:
		std::cout << "Unknown JRcr8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	cpu.cycles += 8;
	cpu.cyclesRan += 8;
}

void CpuInstructions::INCHL(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	uint8_t value = cpu.read(cpu.registers.hl);
	value++;

	cpu.write(cpu.registers.hl, value);

	cpu.setFlag(cpu.Zero, value == 0);
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, (value & 0x0F) == 0);

	cpu.cycles += 12;
	cpu.cyclesRan += 12;
}

void CpuInstructions::RETc(Cpu& cpu)
{
	cpu.inCALL = false;
	uint8_t opcode = cpu.read(cpu.pc);
	cpu.pc++;

	switch (opcode)
	{
	case 0xC0:
	{
		if (!cpu.isFlagSet(cpu.Zero))
		{
			uint8_t lo = cpu.read(cpu.stkp++);
			uint8_t hi = cpu.read(cpu.stkp++);

			cpu.pc = (hi << 8) | lo;

			cpu.cycles += 12;
			cpu.cyclesRan += 12;
		}
	}
	case 0xD0:
	{
		if (!cpu.isFlagSet(cpu.Carry))
		{
			uint8_t lo = cpu.read(cpu.stkp++);
			uint8_t hi = cpu.read(cpu.stkp++);

			cpu.pc = (hi << 8) | lo;

			cpu.cycles += 12;
			cpu.cyclesRan += 12;
		}

		break;
	}
	case 0xC8:
	{
		if (cpu.isFlagSet(cpu.Zero))
		{
			uint8_t lo = cpu.read(cpu.stkp++);
			uint8_t hi = cpu.read(cpu.stkp++);

			cpu.pc = (hi << 8) | lo;

			cpu.cycles += 12;
			cpu.cyclesRan += 12;
		}

		break;
	}
	case 0xC9:
	{
		uint8_t lo = cpu.read(cpu.stkp++);
		uint8_t hi = cpu.read(cpu.stkp++);

		cpu.pc = (hi << 8) | lo;

		cpu.cycles += 8;
		cpu.cyclesRan += 8;

		break;
	}
	case 0xD8:
	{
		if (cpu.isFlagSet(cpu.Zero))
		{
			uint8_t lo = cpu.read(cpu.stkp++);
			uint8_t hi = cpu.read(cpu.stkp++);

			cpu.pc = (hi << 8) | lo;

			cpu.cycles += 12;
			cpu.cyclesRan += 12;
		}

		break;
	}
	case 0xD9:
	{
		uint8_t lo = cpu.read(cpu.stkp++);
		uint8_t hi = cpu.read(cpu.stkp++);

		cpu.pc = (hi << 8) | lo;
		cpu.ime = 1;
		cpu.cycles += 8;
		cpu.cyclesRan += 8;

		break;
	}
	default:
		std::cout << "Unknown ORr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	cpu.cycles += 8;
	cpu.cyclesRan += 8;
}

void CpuInstructions::POPrr(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	uint8_t lo = cpu.read(cpu.stkp++);
	uint8_t hi = cpu.read(cpu.stkp++);

	switch (opcode)
	{
	case 0xC1: // POP BC
	{
		cpu.registers.bc = (hi << 8) | lo;
		break;
	}
	case 0xD1: // POP DE
	{
		cpu.registers.de = (hi << 8) | lo;
		break;
	}
	case 0xE1: // POP HL
	{
		cpu.registers.hl = (hi << 8) | lo;
		break;
	}
	default:
		std::cout << "Unknown POPrr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	cpu.cycles += 12;
	cpu.cyclesRan += 12;
}

void CpuInstructions::JPca16(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc);
	cpu.pc++;

	uint8_t lo = cpu.read(cpu.pc);
	cpu.pc++;
	uint8_t hi = cpu.read(cpu.pc);
	cpu.pc++;

	switch (opcode)
	{
	case 0xC2: // JP NZ,a16
	{
		if (!cpu.isFlagSet(cpu.Zero))
		{
			cpu.pc = (hi << 8) | lo;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;
		}

		break;
	}
	case 0xCA: // JP Z,a16
	{
		if (cpu.isFlagSet(cpu.Zero))
		{
			cpu.pc = (hi << 8) | lo;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;
		}

		break;
	}
	case 0xD2: // JP NC,a16
	{
		if (!cpu.isFlagSet(cpu.Carry))
		{
			cpu.pc = (hi << 8) | lo;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;
		}

		break;
	}
	case 0xDA: // JP C,a16
	{
		if (cpu.isFlagSet(cpu.Carry))
		{
			cpu.pc = (hi << 8) | lo;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;
		}

		break;
	}
	default:
		std::cout << "Unknown JPca16 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	cpu.cycles += 12;
	cpu.cyclesRan += 12;
}

void CpuInstructions::PUSHrr(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	switch (opcode)
	{
	case 0xC5:
	{
		cpu.write(--cpu.stkp, (cpu.registers.bc >> 8) & 0xFF);
		cpu.write(--cpu.stkp, cpu.registers.bc & 0xFF);

		break;
	}
	case 0xD5:
	{
		cpu.write(--cpu.stkp, (cpu.registers.de >> 8) & 0xFF);
		cpu.write(--cpu.stkp, cpu.registers.de & 0xFF);

		break;
	}
	case 0xE5:
	{
		cpu.write(--cpu.stkp, (cpu.registers.hl >> 8) & 0xFF);
		cpu.write(--cpu.stkp, cpu.registers.hl & 0xFF);

		break;
	}
	case 0xF5:
	{
		cpu.write(--cpu.stkp, (cpu.registers.af >> 8) & 0xFF);
		cpu.write(--cpu.stkp, cpu.registers.af & 0xFF);

		break;
	}
	default:
		std::cout << "Unknown PUSHrr instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	cpu.cycles += 16;
	cpu.cyclesRan += 16;
}

void CpuInstructions::RSTn(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
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
		cpu.setHasNotBroken(false);
		break;
	}

	cpu.write(--cpu.stkp, (cpu.pc >> 8) & 0xFF);
	cpu.write(--cpu.stkp, cpu.pc & 0xFF);

	cpu.pc = (0x00 << 8) | n;

	cpu.cycles += 16;
	cpu.cyclesRan += 16;
}

void CpuInstructions::PREFIXCB(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	cpu.wasCB = true;

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::CALLca16(Cpu& cpu)
{
	cpu.inCALL = true;
	uint8_t opcode = cpu.read(cpu.pc++);

	uint8_t lo = cpu.read(cpu.pc++);

	uint8_t hi = cpu.read(cpu.pc++);

	uint16_t nn = (hi << 8) | lo;

	switch (opcode)
	{
	case 0xC4: // CALL NZ, a16
	{
		if (!cpu.isFlagSet(cpu.Zero))
		{
			cpu.write(--cpu.stkp, (cpu.pc >> 8) & 0xFF);
			cpu.write(--cpu.stkp, cpu.pc & 0xFF);

			cpu.pc = nn;

			cpu.cycles += 12;
			cpu.cyclesRan += 12;
		}

		break;
	}
	case 0xCC: // CALL Z, a16
	{
		if (cpu.isFlagSet(cpu.Zero))
		{
			cpu.write(--cpu.stkp, (cpu.pc >> 8) & 0xFF);
			cpu.write(--cpu.stkp, cpu.pc & 0xFF);

			cpu.pc = nn;

			cpu.cycles += 12;
			cpu.cyclesRan += 12;
		}

		break;
	}
	case 0xCD: // CALL a16
	{
		cpu.write(--cpu.stkp, (cpu.pc >> 8) & 0xFF);
		cpu.write(--cpu.stkp, cpu.pc & 0xFF);

		cpu.pc = nn;

		cpu.cycles += 12;
		cpu.cyclesRan += 12;

		break;
	}
	case 0xD4: // CALL NC, a16
	{
		if (!cpu.isFlagSet(cpu.Carry))
		{
			cpu.write(--cpu.stkp, (cpu.pc >> 8) & 0xFF);
			cpu.write(--cpu.stkp, cpu.pc & 0xFF);

			cpu.pc = nn;

			cpu.cycles += 12;
			cpu.cyclesRan += 12;
		}

		break;
	}
	case 0xDC: // CALL C, a16
	{
		if (cpu.isFlagSet(cpu.Carry))
		{
			cpu.write(--cpu.stkp, (cpu.pc >> 8) & 0xFF);
			cpu.write(--cpu.stkp, cpu.pc & 0xFF);

			cpu.pc = nn;

			cpu.cycles += 12;
			cpu.cyclesRan += 12;
		}

		break;
	}
	default:
		std::cout << "Unknown CALLca16 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	cpu.cycles += 12;
	cpu.cyclesRan += 12;
}

void CpuInstructions::RETI(Cpu& cpu)
{
	
	if (cpu.inVblank)
	{
		cpu.inVblank = false;
	}
	cpu.ime = 1;
	uint8_t lo = cpu.read(cpu.stkp++);
	uint8_t hi = cpu.read(cpu.stkp++);

	cpu.pc = (hi << 8) | lo;

	cpu.cycles += 12;
	cpu.cyclesRan += 12;
}

void CpuInstructions::LDH(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	uint16_t a8 = 0xFF00 + cpu.read(cpu.pc++);

	switch (opcode)
	{
	case 0xE0: // LDH (a8), A
	{
		cpu.write(a8, cpu.registers.a);
		break;
	}
	case 0xF0: // LDH A, (a8)
	{
		cpu.registers.a = cpu.read(a8);
		break;
	}
	default:
		std::cout << "Unknown LDH instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	cpu.cycles += 12;
	cpu.cyclesRan += 12;
}

void CpuInstructions::LD_C_A(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	switch (opcode)
	{
	case 0xE2: // LD (C),A
	{
		cpu.write(0xFF00 + cpu.registers.c, cpu.registers.a);
		break;
	}
	case 0xF2: // LD A,(C)
	{
		cpu.registers.a = cpu.read(0xFF00 + cpu.registers.c);
		break;
	}
	default:
		std::cout << "Unknown LD (C),A instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	cpu.cycles += 8;
	cpu.cyclesRan += 8;
}

void CpuInstructions::ADDSPr8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	int data = static_cast<int>(static_cast<int8_t>(cpu.read(cpu.pc++)));
	int result = cpu.stkp + data;

	cpu.setFlag(cpu.Zero, false);
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, ((cpu.stkp & 0x0F) + (data & 0x0F)) > 0x0F);
	cpu.setFlag(cpu.Carry, ((cpu.stkp & 0xFF) + (data & 0xFF)) > 0xFF);

	cpu.stkp = static_cast<uint16_t>(result & 0xFFFF);

	cpu.cycles += 16;
	cpu.cyclesRan += 16;
}

void CpuInstructions::JPaHL(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	switch (opcode)
	{
	case 0xE9:
	{
		cpu.pc = cpu.registers.hl;
		break;
	}
	default:
		std::cout << "Unknown JPaHL instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::LDa16A(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	uint8_t lo = cpu.read(cpu.pc++);
	uint8_t hi = cpu.read(cpu.pc++);

	switch (opcode)
	{
	case 0xEA: // LD (a16), A
	{
		cpu.write((hi << 8) | lo, cpu.registers.a);
		break;
	}
	case 0xFA: // LD A, (a16)
	{
		cpu.registers.a = cpu.read((hi << 8) | lo);
		break;
	}
	default:
		std::cout << "Unknown LDa16A instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	cpu.cycles += 16;
	cpu.cyclesRan += 16;
}

void CpuInstructions::DI(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	cpu.ime = 0;

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::LDHLSPr8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	int data = static_cast<int>(static_cast<int8_t>(cpu.read(cpu.pc++)));

	int result = cpu.stkp + data;

	cpu.setFlag(cpu.Zero, false);
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, ((cpu.stkp & 0x0F) + (data & 0x0F)) > 0x0F);
	cpu.setFlag(cpu.Carry, ((cpu.stkp & 0xFF) + (data & 0xFF)) > 0xFF);

	cpu.registers.hl = static_cast<uint16_t>(result & 0xFFFF);

	cpu.cycles += 12;
	cpu.cyclesRan += 12;
}

void CpuInstructions::LDSPHL(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	cpu.stkp = cpu.registers.hl;

	cpu.cycles += 8;
	cpu.cyclesRan += 8;
}

void CpuInstructions::EI(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	cpu.ime = 1;
	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::JP16(Cpu& cpu)
{
	cpu.pc++;
	uint8_t lo = cpu.read(cpu.pc++);
	uint8_t hi = cpu.read(cpu.pc++);

	uint16_t addr = (hi << 8) | lo;

	cpu.pc = addr;
	cpu.cycles += 16;
	cpu.cyclesRan += 16;
}

void CpuInstructions::RST40(Cpu& cpu)
{
	std::cout << "ENTERING A VBLANK" << std::endl;
	cpu.halted = false;
	cpu.inVblank = true;
	cpu.ime = 0;

	cpu.write(--cpu.stkp, (cpu.pc >> 8) & 0xFF);
	cpu.write(--cpu.stkp, cpu.pc & 0xFF);

	cpu.pc = 0x0040;

	cpu.cycles += 12;
	cpu.cyclesRan += 12;
}

void CpuInstructions::RLCr8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0x00:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0x01:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0x02:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0x03:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0x04:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0x05:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0x06:
	{
		uint8_t value = cpu.read(cpu.registers.hl);

		bool msb = (value & 0x80) != 0;
		value = (value << 1) | msb;

		cpu.write(cpu.registers.hl, value);

		cpu.setFlag(cpu.Zero, value == 0);
		cpu.setFlag(cpu.Subtraction, false);
		cpu.setFlag(cpu.HalfCarry, false);
		cpu.setFlag(cpu.Carry, msb);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0x07:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown RLCr8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	if (r8 == nullptr)
	{
		return;
	}

	bool msb = (*r8 & 0x80) != 0;

	*r8 = (*r8 << 1) | msb;

	cpu.setFlag(cpu.Zero, *r8 == 0);
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, false);
	cpu.setFlag(cpu.Carry, msb);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::RRCr8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0x08:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0x09:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0x0A:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0x0B:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0x0C:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0x0D:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0x0E:
	{
		uint8_t value = cpu.read(cpu.registers.hl);

		bool carry = value & 0x01;
		value = (value >> 1) | (carry << 7);

		cpu.write(cpu.registers.hl, value);

		cpu.setFlag(cpu.Zero, value == 0);
		cpu.setFlag(cpu.Subtraction, false);
		cpu.setFlag(cpu.HalfCarry, false);
		cpu.setFlag(cpu.Carry, carry);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0x0F:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown RRCr8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	if (r8 == nullptr)
	{
		return;
	}

	bool carry = *r8 & 0x01;

	*r8 = (*r8 >> 1) | (carry << 7);

	cpu.setFlag(cpu.Zero, *r8 == 0);
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, false);
	cpu.setFlag(cpu.Carry, carry);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::RLr8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	uint8_t* r8 = nullptr;

	switch (opcode) {
	case 0x10:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0x11:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0x12:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0x13:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0x14:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0x15:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0x16:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		bool carry = cpu.isFlagSet(cpu.Carry);
		bool msb = (value & 0x80) != 0;

		value = (value << 1) | carry;

		cpu.write(cpu.registers.hl, value);

		cpu.setFlag(cpu.Zero, value == 0);
		cpu.setFlag(cpu.Subtraction, false);
		cpu.setFlag(cpu.HalfCarry, false);
		cpu.setFlag(cpu.Carry, msb);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0x17:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown RLr8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr) {
		return;
	}

	bool carry = cpu.isFlagSet(cpu.Carry);
	bool msb = (*r8 & 0x80) != 0;

	*r8 <<= 1;
	*r8 |= carry;

	cpu.setFlag(cpu.Zero, *r8 == 0);
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, false);
	cpu.setFlag(cpu.Carry, msb);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::SLAr8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0x20:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0x21:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0x22:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0x23:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0x24:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0x25:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0x26:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		bool carry = (value & 0x80) != 0;
		value <<= 1;

		cpu.write(cpu.registers.hl, value);

		cpu.setFlag(cpu.Zero, value == 0x00);
		cpu.setFlag(cpu.Subtraction, false);
		cpu.setFlag(cpu.HalfCarry, false);
		cpu.setFlag(cpu.Carry, carry);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0x27:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown SLAr8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	bool carry = (*r8 & 0x80) != 0;

	*r8 <<= 1;

	cpu.setFlag(cpu.Zero, *r8 == 0x00);
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, false);
	cpu.setFlag(cpu.Carry, carry);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::SRAr8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0x28:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0x29:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0x2A:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0x2B:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0x2C:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0x2D:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0x2E:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		bool carry = (value & 0x01) != 0;
		bool bit7 = (value & 0x80) != 0;

		value >>= 1;

		if (bit7)
		{
			value |= 0x80;
		}

		cpu.write(cpu.registers.hl, value);

		cpu.setFlag(cpu.Zero, value == 0x00);
		cpu.setFlag(cpu.Subtraction, false);
		cpu.setFlag(cpu.HalfCarry, false);
		cpu.setFlag(cpu.Carry, carry);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0x2F:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown SRAr8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
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

	cpu.setFlag(cpu.Zero, *r8 == 0x00);
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, false);
	cpu.setFlag(cpu.Carry, carry);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::SRLr8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0x38:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0x39:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0x3A:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0x3B:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0x3C:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0x3D:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0x3E:
	{
		uint8_t value = cpu.read(cpu.registers.hl);

		bool carry = value & 0x01;
		value >>= 1;

		cpu.write(cpu.registers.hl, value);

		cpu.setFlag(cpu.Zero, value == 0);
		cpu.setFlag(cpu.Subtraction, false);
		cpu.setFlag(cpu.HalfCarry, false);
		cpu.setFlag(cpu.Carry, carry);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0x3F:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown SRLr8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		break;
	}

	if (r8 == nullptr)
	{
		return;
	}

	bool carry = *r8 & 0x01;

	*r8 >>= 1;

	cpu.setFlag(cpu.Zero, *r8 == 0);
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, false);
	cpu.setFlag(cpu.Carry, carry);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::RRr8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0x18:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0x19:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0x1A:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0x1B:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0x1C:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0x1D:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0x1E:
	{
		uint8_t value = cpu.read(cpu.registers.hl);

		bool carry = cpu.isFlagSet(cpu.Carry);
		bool new_carry = value & 0x01;
		value = (value >> 1) | (carry << 7);

		cpu.write(cpu.registers.hl, value);

		cpu.setFlag(cpu.Zero, value == 0);
		cpu.setFlag(cpu.Subtraction, false);
		cpu.setFlag(cpu.HalfCarry, false);
		cpu.setFlag(cpu.Carry, new_carry);

		cpu.cycles += 16;
		cpu.cyclesRan += 16;
		break;
	}
	case 0x1F:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown RRr8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	bool carry = cpu.isFlagSet(cpu.Carry);
	bool new_carry = *r8 & 0x01;

	*r8 = (*r8 >> 1) | (carry << 7);

	cpu.setFlag(cpu.Zero, *r8 == 0);
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, false);
	cpu.setFlag(cpu.Carry, new_carry);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::SWAPr8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0x30:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0x31:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0x32:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0x33:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0x34:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0x35:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0x36:
	{
		uint8_t value = cpu.read(cpu.registers.hl);

		value = ((value & 0x0F) << 4) | ((value & 0xF0) >> 4);

		cpu.write(cpu.registers.hl, value);

		cpu.setFlag(cpu.Zero, value == 0);
		cpu.setFlag(cpu.Subtraction, false);
		cpu.setFlag(cpu.HalfCarry, false);
		cpu.setFlag(cpu.Carry, false);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0x37:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown SWAPr8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	*r8 = ((*r8 & 0x0F) << 4) | ((*r8 & 0xF0) >> 4);

	cpu.setFlag(cpu.Zero, *r8 == 0);
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, false);
	cpu.setFlag(cpu.Carry, false);

	cpu.cycles += 8;
	cpu.cyclesRan += 8;
}

void CpuInstructions::RESu3r8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	switch (opcode)
	{
	case 0x87:
	{
		cpu.registers.a &= ~(1 << 0);
		break;
	}
	case 0x88:
	{
		cpu.registers.b &= ~(1 << 1);
		break;
	}
	case 0x89:
	{
		cpu.registers.c &= ~(1 << 1);
		break;
	}
	case 0x8A:
	{
		cpu.registers.d &= ~(1 << 1);
		break;
	}
	case 0x8B:
	{
		cpu.registers.e &= ~(1 << 1);
		break;
	}
	case 0x8C:
	{
		cpu.registers.h &= ~(1 << 1);
		break;
	}
	case 0x8D:
	{
		cpu.registers.l &= ~(1 << 1);
		break;
	}
	case 0x8E:
	{
		uint8_t value = cpu.read(cpu.registers.hl);

		value &= ~(1 << 1);

		cpu.write(cpu.registers.hl, value);
		cpu.cycles += 12;
		cpu.cyclesRan += 12;

		break;
	}
	case 0x8F:
	{
		cpu.registers.a &= ~(1 << 1);
		break;
	}
	default:
		std::cout << "Unknown RESu3r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::BIT0r8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0x40:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0x41:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0x42:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0x43:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0x44:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0x45:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0x46:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		cpu.setFlag(cpu.Zero, !(value & (1 << 0)));
		cpu.setFlag(cpu.Subtraction, false);
		cpu.setFlag(cpu.HalfCarry, true);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0x47:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown BIT0r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	cpu.setFlag(cpu.Zero, !(*r8 & (1 << 0)));
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, true);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::BIT1r8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0x48:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0x49:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0x4A:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0x4B:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0x4C:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0x4D:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0x4E:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		cpu.setFlag(cpu.Zero, !(value & (1 << 1)));
		cpu.setFlag(cpu.Subtraction, false);
		cpu.setFlag(cpu.HalfCarry, true);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0x4F:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown BIT1r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	cpu.setFlag(cpu.Zero, !(*r8 & (1 << 1)));
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, true);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::BIT2r8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0x50:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0x51:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0x52:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0x53:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0x54:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0x55:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0x56:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		cpu.setFlag(cpu.Zero, !(value & (1 << 2)));
		cpu.setFlag(cpu.Subtraction, false);
		cpu.setFlag(cpu.HalfCarry, true);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0x57:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown BIT2r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	cpu.setFlag(cpu.Zero, !(*r8 & (1 << 2)));
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, true);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::BIT3r8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0x58:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0x59:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0x5A:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0x5B:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0x5C:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0x5D:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0x5E:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		cpu.setFlag(cpu.Zero, !(value & (1 << 3)));
		cpu.setFlag(cpu.Subtraction, false);
		cpu.setFlag(cpu.HalfCarry, true);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0x5F:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown BIT3r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	cpu.setFlag(cpu.Zero, !(*r8 & (1 << 3)));
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, true);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::BIT4r8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0x60:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0x61:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0x62:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0x63:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0x64:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0x65:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0x66:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		cpu.setFlag(cpu.Zero, !(value & (1 << 4)));
		cpu.setFlag(cpu.Subtraction, false);
		cpu.setFlag(cpu.HalfCarry, true);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0x67:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown BIT4r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	cpu.setFlag(cpu.Zero, !(*r8 & (1 << 4)));
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, true);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::BIT5r8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0x68:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0x69:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0x6A:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0x6B:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0x6C:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0x6D:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0x6E:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		cpu.setFlag(cpu.Zero, !(value & (1 << 5)));
		cpu.setFlag(cpu.Subtraction, false);
		cpu.setFlag(cpu.HalfCarry, true);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0x6F:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown BIT5r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	cpu.setFlag(cpu.Zero, !(*r8 & (1 << 5)));
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, true);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::BIT6r8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0x70:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0x71:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0x72:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0x73:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0x74:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0x75:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0x76:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		cpu.setFlag(cpu.Zero, !(value & (1 << 6)));
		cpu.setFlag(cpu.Subtraction, false);
		cpu.setFlag(cpu.HalfCarry, true);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0x77:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown BIT6r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	cpu.setFlag(cpu.Zero, !(*r8 & (1 << 6)));
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, true);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::BIT7r8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0x78:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0x79:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0x7A:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0x7B:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0x7C:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0x7D:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0x7E:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		cpu.setFlag(cpu.Zero, !(value & (1 << 7)));
		cpu.setFlag(cpu.Subtraction, false);
		cpu.setFlag(cpu.HalfCarry, true);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0x7F:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown BIT7r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	cpu.setFlag(cpu.Zero, !(*r8 & (1 << 7)));
	cpu.setFlag(cpu.Subtraction, false);
	cpu.setFlag(cpu.HalfCarry, true);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::RES0r8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	uint8_t* r8 = nullptr;

	switch (opcode) {
	case 0x80:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0x81:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0x82:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0x83:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0x84:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0x85:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0x86:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		value = value & ~(1 << 0);

		cpu.write(cpu.registers.hl, value);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0x87:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown RES0r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	*r8 &= ~(1 << 0);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::RES1r8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	uint8_t* r8 = nullptr;

	switch (opcode) {
	case 0x88:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0x89:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0x8A:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0x8B:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0x8C:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0x8D:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0x8E:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		value = value & ~(1 << 1);

		cpu.write(cpu.registers.hl, value);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0x8F:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown RES1r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	*r8 &= ~(1 << 1);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::RES2r8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	uint8_t* r8 = nullptr;

	switch (opcode) {
	case 0x90:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0x91:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0x92:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0x93:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0x94:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0x95:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0x96:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		value = value & ~(1 << 2);

		cpu.write(cpu.registers.hl, value);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0x97:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown RES2r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	*r8 &= ~(1 << 2);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::RES3r8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	uint8_t* r8 = nullptr;

	switch (opcode) {
	case 0x98:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0x99:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0x9A:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0x9B:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0x9C:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0x9D:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0x9E:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		value = value & ~(1 << 3);

		cpu.write(cpu.registers.hl, value);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0x9F:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown RES3r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	*r8 &= ~(1 << 3);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::RES4r8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	uint8_t* r8 = nullptr;

	switch (opcode) {
	case 0xA0:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0xA1:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0xA2:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0xA3:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0xA4:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0xA5:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0xA6:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		value = value & ~(1 << 4);

		cpu.write(cpu.registers.hl, value);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0xA7:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown RES4r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	*r8 &= ~(1 << 4);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::RES5r8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	uint8_t* r8 = nullptr;

	switch (opcode) {
	case 0xA8:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0xA9:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0xAA:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0xAB:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0xAC:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0xAD:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0xAE:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		value = value & ~(1 << 5);

		cpu.write(cpu.registers.hl, value);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0xAF:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown RES5r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	*r8 &= ~(1 << 5);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::RES6r8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	uint8_t* r8 = nullptr;

	switch (opcode) {
	case 0xB0:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0xB1:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0xB2:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0xB3:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0xB4:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0xB5:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0xB6:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		value = value & ~(1 << 6);

		cpu.write(cpu.registers.hl, value);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0xB7:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown RES6r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	*r8 &= ~(1 << 6);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::RES7r8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	uint8_t* r8 = nullptr;

	switch (opcode) {
	case 0xB8:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0xB9:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0xBA:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0xBB:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0xBC:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0xBD:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0xBE:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		value = value & ~(1 << 1);

		cpu.write(cpu.registers.hl, value);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0xBF:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown RES7r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr)
	{
		return;
	}

	*r8 &= ~(1 << 7);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::SET0r8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0xC0:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0xC1:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0xC2:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0xC3:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0xC4:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0xC5:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0xC6:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		value |= (1 << 0);

		cpu.write(cpu.registers.hl, value);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0xC7:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown SET0r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr) {
		return;
	}

	*r8 |= (1 << 0);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::SET1r8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0xC8:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0xC9:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0xCA:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0xCB:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0xCC:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0xCD:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0xCE:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		value |= (1 << 1);

		cpu.write(cpu.registers.hl, value);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0xCF:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown SET1r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr) {
		return;
	}

	*r8 |= (1 << 1);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::SET2r8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0xD0:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0xD1:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0xD2:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0xD3:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0xD4:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0xD5:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0xD6:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		value |= (1 << 2);

		cpu.write(cpu.registers.hl, value);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0xD7:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown SET2r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr) {
		return;
	}

	*r8 |= (1 << 2);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::SET3r8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0xD8:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0xD9:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0xDA:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0xDB:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0xDC:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0xDD:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0xDE:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		value |= (1 << 3);

		cpu.write(cpu.registers.hl, value);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0xDF:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown SET3r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr) {
		return;
	}

	*r8 |= (1 << 3);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::SET4r8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0xE0:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0xE1:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0xE2:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0xE3:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0xE4:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0xE5:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0xE6:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		value |= (1 << 4);

		cpu.write(cpu.registers.hl, value);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0xE7:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown SET4r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr) {
		return;
	}

	*r8 |= (1 << 4);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::SET5r8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0xE8:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0xE9:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0xEA:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0xEB:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0xEC:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0xED:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0xEE:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		value |= (1 << 5);

		cpu.write(cpu.registers.hl, value);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0xEF:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown SET5r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr) {
		return;
	}

	*r8 |= (1 << 5);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::SET6r8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0xF0:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0xF1:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0xF2:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0xF3:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0xF4:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0xF5:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0xF6:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		value |= (1 << 6);

		cpu.write(cpu.registers.hl, value);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0xF7:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown SET6r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr) {
		return;
	}

	*r8 |= (1 << 6);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::SET7r8(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);

	uint8_t* r8 = nullptr;

	switch (opcode)
	{
	case 0xF8:
	{
		r8 = &cpu.registers.b;
		break;
	}
	case 0xF9:
	{
		r8 = &cpu.registers.c;
		break;
	}
	case 0xFA:
	{
		r8 = &cpu.registers.d;
		break;
	}
	case 0xFB:
	{
		r8 = &cpu.registers.e;
		break;
	}
	case 0xFC:
	{
		r8 = &cpu.registers.h;
		break;
	}
	case 0xFD:
	{
		r8 = &cpu.registers.l;
		break;
	}
	case 0xFE:
	{
		uint8_t value = cpu.read(cpu.registers.hl);
		value |= (1 << 7);

		cpu.write(cpu.registers.hl, value);

		cpu.cycles += 12;
		cpu.cyclesRan += 12;
		break;
	}
	case 0xFF:
	{
		r8 = &cpu.registers.a;
		break;
	}
	default:
		std::cout << "Unknown SET7r8 instruction: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
		cpu.setHasNotBroken(false);
		return;
	}

	if (r8 == nullptr) {
		return;
	}

	*r8 |= (1 << 7);

	cpu.cycles += 4;
	cpu.cyclesRan += 4;
}

void CpuInstructions::XXX(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc);

	std::cout << "Unknown/Illegal instruction: " << "PREFIX CB: " << cpu.wasCB << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
	cpu.setHasNotBroken(false);
}
