#include "CpuInstructions.h"
#include <iostream>
#include <iomanip>



CpuInstructions::CpuInstructions()
{
}

CpuInstructions::~CpuInstructions()
{
}


void CpuInstructions::runInstruction(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	uint16_t* r16 = nullptr;
	uint8_t* r8 = nullptr;

	uint8_t hlValue;
	uint8_t D8;

	uint8_t carry = cpu.isFlagSet(cpu.Carry) ? 1 : 0;

	uint16_t ADDresult = 0;
	uint8_t SUBresult;

	uint8_t n;

	int instructionMode = INVALID;

	switch (opcode)
	{
		case 0x00: // NOP
		{
			cpu.cycles += 4;
			cpu.cyclesRan += 4;
			return;
			break;
		}
		case 0x01: // LD BC, d16
		{
			r16 = &cpu.registers.bc;
			instructionMode = LD_RR_D16;
			break;
		}
		case 0x11: // LD DE, d16
		{
			r16 = &cpu.registers.de;
			instructionMode = LD_RR_D16;
			break;
		}
		case 0x21: // LD HL, d16
		{
			r16 = &cpu.registers.hl;
			instructionMode = LD_RR_D16;
			break;
		}
		case 0x31: // LD SP, d16
		{
			r16 = &cpu.stkp;
			instructionMode = LD_RR_D16;
			break;
		}
		case 0x02: // LD (BC), A
		{
			cpu.write(cpu.registers.bc, cpu.registers.a);
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x12: // LD (DE), A
		{
			cpu.write(cpu.registers.de, cpu.registers.a);
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x22: // LD (HL+), A
		{
			cpu.write(cpu.registers.hl++, cpu.registers.a);
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x32: // LD (HL-), A
		{
			cpu.write(cpu.registers.hl--, cpu.registers.a);
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x70: // LD (HL), B
		{
			cpu.write(cpu.registers.hl, cpu.registers.b);
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x71: // LD (HL), C
		{
			cpu.write(cpu.registers.hl, cpu.registers.c);
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x72: // LD (HL), D
		{
			cpu.write(cpu.registers.hl, cpu.registers.d);
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x73: // LD (HL), E
		{
			cpu.write(cpu.registers.hl, cpu.registers.e);
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x74: // LD (HL), H
		{
			cpu.write(cpu.registers.hl, cpu.registers.h);
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x75: // LD (HL), L
		{
			cpu.write(cpu.registers.hl, cpu.registers.l);
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x77: // LD (HL), A
		{
			cpu.write(cpu.registers.hl, cpu.registers.a);
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x03: // INC BC
		{
			cpu.registers.bc++;
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x13: // INC DE
		{
			cpu.registers.de++;
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x23: // INC HL
		{
			cpu.registers.hl++;
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x33: // INC SP (Stack Pointer)
		{
			cpu.stkp++;
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x04: // INC B
		{
			r8 = &cpu.registers.b;
			instructionMode = INR_R;

			break;
		}
		case 0x14: // INC D
		{
			r8 = &cpu.registers.d;
			instructionMode = INR_R;

			break;
		}
		case 0x24: // INC H
		{
			r8 = &cpu.registers.h;
			instructionMode = INR_R;

			break;
		}
		case 0x0C: // INC C
		{
			r8 = &cpu.registers.c;
			instructionMode = INR_R;

			break;
		}
		case 0x1C: // INC E
		{
			r8 = &cpu.registers.e;
			instructionMode = INR_R;

			break;
		}
		case 0x2C: // INC L
		{
			r8 = &cpu.registers.l;
			instructionMode = INR_R;

			break;
		}
		case 0x3C: // INC A
		{
			r8 = &cpu.registers.a;
			instructionMode = INR_R;

			break;
		}
		case 0x05: // DEC B
		{
			uint8_t value = cpu.registers.b;
			value--;
			cpu.registers.b = value;

			cpu.setFlag(cpu.Zero, cpu.registers.b == 0x00);
			cpu.setFlag(cpu.Subtraction, true);
			cpu.setFlag(cpu.HalfCarry, (cpu.registers.b & 0x0F) == 0x0F);

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
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

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
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

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
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

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
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

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
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

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
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

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x06: // LD B, d8
		{
			r8 = &cpu.registers.b;
			instructionMode = LD_R_D8;

			break;
		}
		case 0x16: // LD D, d8
		{
			r8 = &cpu.registers.d;
			instructionMode = LD_R_D8;

			break;
		}
		case 0x26: // LD H, d8
		{
			r8 = &cpu.registers.h;
			instructionMode = LD_R_D8;

			break;
		}
		case 0x36: // LD (HL), d8
		{
			uint8_t value = cpu.read(cpu.pc++);
			cpu.write(cpu.registers.hl, value);
			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0x0E: // LD C, d8
		{
			r8 = &cpu.registers.c;
			instructionMode = LD_R_D8;

			break;
		}
		case 0x1E: // LD E, d8
		{
			r8 = &cpu.registers.e;
			instructionMode = LD_R_D8;

			break;
		}
		case 0x2E: // LD L, d8
		{
			r8 = &cpu.registers.l;
			instructionMode = LD_R_D8;

			break;
		}
		case 0x3E: // LD A, d8
		{
			r8 = &cpu.registers.a;
			instructionMode = LD_R_D8;

			break;
		}
		case 0x07: // RCLA
		{
			bool carry = (cpu.registers.a & 0x80) != 0;

			cpu.registers.a <<= 1;
			cpu.registers.a |= carry;

			cpu.setFlag(cpu.Zero, false);
			cpu.setFlag(cpu.Subtraction, false);
			cpu.setFlag(cpu.HalfCarry, false);
			cpu.setFlag(cpu.Carry, carry);

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x08: // LD (a16), SP
		{
			uint8_t lo = cpu.read(cpu.pc++);
			uint8_t hi = cpu.read(cpu.pc++);

			uint16_t a16 = (hi << 8) | lo;

			cpu.write(a16, cpu.stkp & 0xFF);
			cpu.write(a16 + 1, (cpu.stkp >> 8) & 0xFF);

			cpu.cycles += 20;
			cpu.cyclesRan += 20;

			return;
			break;
		}
		case 0x09: // ADD HL, BC
		{
			r16 = &cpu.registers.bc;
			instructionMode = ADD_RR_RR;

			break;
		}
		case 0x19: // ADD HL, DE
		{
			r16 = &cpu.registers.de;
			instructionMode = ADD_RR_RR;

			break;
		}
		case 0x29: // ADD HL, HL
		{
			r16 = &cpu.registers.hl;
			instructionMode = ADD_RR_RR;

			break;
		}
		case 0x39: // ADD HL, SP
		{
			r16 = &cpu.stkp;
			instructionMode = ADD_RR_RR;

			break;
		}
		case 0x0B: // DEC BC
		{
			cpu.registers.bc--;
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x1B: // DEC DE
		{
			cpu.registers.de--;
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x2B: // DEC HL
		{
			cpu.registers.hl--;
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x3B: // DEC SP (Stack Pointer)
		{
			cpu.stkp--;
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x0F:
		{
			bool carry = cpu.registers.a & 0x01;

			cpu.registers.a >>= 1;
			cpu.registers.a |= (carry << 7);

			cpu.setFlag(cpu.Zero, false);
			cpu.setFlag(cpu.Subtraction, false);
			cpu.setFlag(cpu.HalfCarry, false);
			cpu.setFlag(cpu.Carry, carry);

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x10:
		{
			std::cout << "UNIMPLEMENTED STOP INSTRUCTION" << std::endl;
			cpu.setHasNotBroken(false);

			return;
			break; 
		}
		case 0x17: // RLA
		{
			bool carry = cpu.isFlagSet(cpu.Carry);

			cpu.setFlag(cpu.Carry, (cpu.registers.a & 0x80) != 0);
			cpu.registers.a = (cpu.registers.a << 1) | carry;

			cpu.setFlag(cpu.Zero, false);
			cpu.setFlag(cpu.Subtraction, false);
			cpu.setFlag(cpu.HalfCarry, false);

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x1F: // RRA
		{
			bool carry = cpu.isFlagSet(cpu.Carry);
			bool new_carry = cpu.registers.a & 0x01;

			cpu.registers.a = (cpu.registers.a >> 1) | (carry << 7);

			cpu.setFlag(cpu.Zero, false);
			cpu.setFlag(cpu.Subtraction, false);
			cpu.setFlag(cpu.HalfCarry, false);
			cpu.setFlag(cpu.Carry, new_carry);

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x27: // DAA
		{
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

			return;
			break;
		}
		case 0x2F: // CPL
		{
			cpu.registers.a = ~cpu.registers.a;

			cpu.setFlag(cpu.Subtraction, true);
			cpu.setFlag(cpu.HalfCarry, true);

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x34: // INC (HL)
		{
			uint8_t value = cpu.read(cpu.registers.hl);
			value++;

			cpu.write(cpu.registers.hl, value);

			cpu.setFlag(cpu.Zero, value == 0);
			cpu.setFlag(cpu.Subtraction, false);
			cpu.setFlag(cpu.HalfCarry, (value & 0x0F) == 0x00);

			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0x35: // DEC (HL)
		{
			uint8_t value = cpu.read(cpu.registers.hl);
			uint8_t result = value - 1;

			cpu.setFlag(cpu.Zero, result == 0);
			cpu.setFlag(cpu.Subtraction, true);
			cpu.setFlag(cpu.HalfCarry, (result & 0x0F) == 0x0F);

			cpu.write(cpu.registers.hl, result);

			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0x37: // SCF
		{
			cpu.setFlag(cpu.Subtraction, false);
			cpu.setFlag(cpu.HalfCarry, false);
			cpu.setFlag(cpu.Carry, true);

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x3F: // CCF
		{
			uint8_t carry = !cpu.isFlagSet(cpu.Carry);

			cpu.setFlag(cpu.Subtraction, false);
			cpu.setFlag(cpu.HalfCarry, false);
			cpu.setFlag(cpu.Carry, carry);

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x40: // LD B, B
		{
			cpu.registers.b = cpu.registers.b;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x41: // LD B, C
		{
			cpu.registers.b = cpu.registers.c;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x42: // LD B, D
		{
			cpu.registers.b = cpu.registers.d;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x43: // LD B, E
		{
			cpu.registers.b = cpu.registers.e;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x44: // LD B, H
		{
			cpu.registers.b = cpu.registers.h;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x45: // LD B, L
		{
			cpu.registers.b = cpu.registers.l;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x46: // LD B, (HL)
		{
			cpu.registers.b = cpu.read(cpu.registers.hl);
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x47: // LD B, A
		{
			cpu.registers.b = cpu.registers.a;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x48: // LD C, B
		{
			cpu.registers.c = cpu.registers.b;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x49: // LD C, C
		{
			cpu.registers.c = cpu.registers.c;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x4A: // LD C, D
		{
			cpu.registers.c = cpu.registers.d;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x4B: // LD C, E
		{
			cpu.registers.c = cpu.registers.e;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x4C: // LD C, H
		{
			cpu.registers.c = cpu.registers.h;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x4D: // LD C, L
		{
			cpu.registers.c = cpu.registers.l;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x4E: // LD C, (HL)
		{
			cpu.registers.c = cpu.read(cpu.registers.hl);
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x4F: // LD C, A
		{
			cpu.registers.c = cpu.registers.a;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x50: // LD D, B
		{
			cpu.registers.d = cpu.registers.b;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x51: // LD D, C
		{
			cpu.registers.d = cpu.registers.c;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x52: // LD D, D
		{
			cpu.registers.d = cpu.registers.d;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x53: // LD D, E
		{
			cpu.registers.d = cpu.registers.e;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x54: // LD D, H
		{
			cpu.registers.d = cpu.registers.h;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x55: // LD D, L
		{
			cpu.registers.d = cpu.registers.l;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x56: // LD D, (HL)
		{
			cpu.registers.d = cpu.read(cpu.registers.hl);
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x57: // LD D, A
		{
			cpu.registers.d = cpu.registers.a;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x58: // LD E, B
		{
			cpu.registers.e = cpu.registers.b;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x59: // LD E, C
		{
			cpu.registers.e = cpu.registers.c;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x5A: // LD E, D
		{
			cpu.registers.e = cpu.registers.d;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x5B: // LD E, E
		{
			cpu.registers.e = cpu.registers.e;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x5C: // LD E, H
		{
			cpu.registers.e = cpu.registers.h;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x5D: // LD E, L
		{
			cpu.registers.e = cpu.registers.l;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x5E: // LD E, (HL)
		{
			cpu.registers.e = cpu.read(cpu.registers.hl);
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x5F: // LD E, A
		{
			cpu.registers.e = cpu.registers.a;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x60: // LD E, B
		{
			cpu.registers.h = cpu.registers.b;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x61: // LD H, C
		{
			cpu.registers.h = cpu.registers.c;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x62: // LD H, D
		{
			cpu.registers.h = cpu.registers.d;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x63: // LD H, E
		{
			cpu.registers.h = cpu.registers.e;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x64: // LD H, H
		{
			cpu.registers.h = cpu.registers.h;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x65: // LD H, L
		{
			cpu.registers.h = cpu.registers.l;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x66: // LD H, (HL)
		{
			cpu.registers.h = cpu.read(cpu.registers.hl);
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x67: // LD H, A
		{
			cpu.registers.h = cpu.registers.a;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x68: // LD L, B
		{
			cpu.registers.l = cpu.registers.b;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x69: // LD L, C
		{
			cpu.registers.l = cpu.registers.c;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x6A: // LD L, D
		{
			cpu.registers.l = cpu.registers.d;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x6B: // LD L, E
		{
			cpu.registers.l = cpu.registers.e;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x6C: // LD L, H
		{
			cpu.registers.l = cpu.registers.h;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x6D: // LD L, L
		{
			cpu.registers.l = cpu.registers.l;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x6E: // LD L, (HL)
		{
			cpu.registers.l = cpu.read(cpu.registers.hl);
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x6F: // LD L, A
		{
			cpu.registers.l = cpu.registers.a;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x78: // LD A, B
		{
			cpu.registers.a = cpu.registers.b;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x79: // LD A, C
		{
			cpu.registers.a = cpu.registers.c;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x7A: // LD A, D
		{
			cpu.registers.a = cpu.registers.d;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x7B: // LD A, E
		{
			cpu.registers.a = cpu.registers.e;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x7C: // LD A, H
		{
			cpu.registers.a = cpu.registers.h;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x7D: // LD A, L
		{
			cpu.registers.a = cpu.registers.l;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x7E: // LD A, (HL)
		{
			cpu.registers.a = cpu.read(cpu.registers.hl);
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0x7F: // LD A, A
		{
			cpu.registers.a = cpu.registers.a;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0x76: // HALT
		{
			cpu.halted = true;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			std::cout << "CPU HALTED!" << std::endl;

			return;
			break;
		}
		case 0x80:
		{
			ADDresult = cpu.registers.a + cpu.registers.b;
			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.b & 0x0F) > 0x0F);

			instructionMode = ADD_R_R;
			break;
		}
		case 0x81:
		{
			ADDresult = cpu.registers.a + cpu.registers.c;
			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.c & 0x0F) > 0x0F);

			instructionMode = ADD_R_R;
			break;
		}
		case 0x82:
		{
			ADDresult = cpu.registers.a + cpu.registers.d;
			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.d & 0x0F) > 0x0F);

			instructionMode = ADD_R_R;
			break;
		}
		case 0x83:
		{
			ADDresult = cpu.registers.a + cpu.registers.e;
			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.e & 0x0F) > 0x0F);

			instructionMode = ADD_R_R;
			break;
		}
		case 0x84:
		{
			ADDresult = cpu.registers.a + cpu.registers.h;
			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.h & 0x0F) > 0x0F);

			instructionMode = ADD_R_R;
			break;
		}
		case 0x85:
		{
			ADDresult = cpu.registers.a + cpu.registers.l;
			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.l & 0x0F) > 0x0F);

			instructionMode = ADD_R_R;
			break;
		}
		case 0x86:
		{
			ADDresult = cpu.registers.a + cpu.read(cpu.registers.hl);
			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.read(cpu.registers.hl) & 0x0F) > 0x0F);

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			instructionMode = ADD_R_R;
			break;
		}
		case 0x87:
		{
			ADDresult = cpu.registers.a + cpu.registers.a;
			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.a & 0x0F) > 0x0F);

			instructionMode = ADD_R_R;
			break;
		}
		case 0x88:
		{
			ADDresult = cpu.registers.a + cpu.registers.b + carry;

			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.b & 0x0F) + carry > 0x0F);

			instructionMode = ADD_R_R;
			break;
		}
		case 0x89:
		{
			ADDresult = cpu.registers.a + cpu.registers.c + carry;

			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.c & 0x0F) + carry > 0x0F);

			instructionMode = ADD_R_R;
			break;
		}
		case 0x8A:
		{
			ADDresult = cpu.registers.a + cpu.registers.d + carry;

			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.d & 0x0F) + carry > 0x0F);

			instructionMode = ADD_R_R;
			break;
		}
		case 0x8B:
		{
			ADDresult = cpu.registers.a + cpu.registers.e + carry;

			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.e & 0x0F) + carry > 0x0F);

			instructionMode = ADD_R_R;
			break;
		}
		case 0x8C:
		{
			ADDresult = cpu.registers.a + cpu.registers.h + carry;

			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.h & 0x0F) + carry > 0x0F);

			instructionMode = ADD_R_R;
			break;
		}
		case 0x8D:
		{
			ADDresult = cpu.registers.a + cpu.registers.l + carry;

			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.l & 0x0F) + carry > 0x0F);

			instructionMode = ADD_R_R;
			break;
		}
		case 0x8E:
		{
			ADDresult = cpu.registers.a + cpu.read(cpu.registers.hl) + carry;

			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.read(cpu.registers.hl) & 0x0F) + carry > 0x0F);

			cpu.cycles += 4;
			cpu.cycles += 4;

			instructionMode = ADD_R_R;
			break;
		}
		case 0x8F:
		{
			ADDresult = cpu.registers.a + cpu.registers.a + carry;

			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (cpu.registers.a & 0x0F) + carry > 0x0F);

			instructionMode = ADD_R_R;
			break;
		}
		case 0xC6:
		{
			uint8_t d8 = cpu.read(cpu.pc++);

			ADDresult = cpu.registers.a + d8;

			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (d8 & 0x0F) > 0x0F);

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			instructionMode = ADD_R_R;
			break;
		}
		case 0xCE:
		{
			uint8_t d8 = cpu.read(cpu.pc++);
			ADDresult = cpu.registers.a + d8 + carry;

			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) + (d8 & 0x0F) + carry > 0x0F);

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			instructionMode = ADD_R_R;

			break;
		}
		case 0x90:
		{
			SUBresult = cpu.registers.a - cpu.registers.b;
			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.b & 0x0F));
			cpu.setFlag(cpu.Carry, cpu.registers.b > cpu.registers.a);

			instructionMode = SUB_R_R;
			break;
		}
		case 0x91:
		{
			SUBresult = cpu.registers.a - cpu.registers.c;
			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.c & 0x0F));
			cpu.setFlag(cpu.Carry, cpu.registers.c > cpu.registers.a);

			instructionMode = SUB_R_R;
			break;
		}
		case 0x92:
		{
			SUBresult = cpu.registers.a - cpu.registers.d;
			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.d & 0x0F));
			cpu.setFlag(cpu.Carry, cpu.registers.d > cpu.registers.a);

			instructionMode = SUB_R_R;
			break;
		}
		case 0x93:
		{
			SUBresult = cpu.registers.a - cpu.registers.e;
			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.e & 0x0F));
			cpu.setFlag(cpu.Carry, cpu.registers.e > cpu.registers.a);

			instructionMode = SUB_R_R;
			break;
		}
		case 0x94:
		{
			SUBresult = cpu.registers.a - cpu.registers.h;
			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.h & 0x0F));
			cpu.setFlag(cpu.Carry, cpu.registers.h > cpu.registers.a);

			instructionMode = SUB_R_R;
			break;
		}
		case 0x95:
		{
			SUBresult = cpu.registers.a - cpu.registers.l;
			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.l & 0x0F));
			cpu.setFlag(cpu.Carry, cpu.registers.l > cpu.registers.a);

			instructionMode = SUB_R_R;
			break;
		}
		case 0x96:
		{
			SUBresult = cpu.registers.a - cpu.read(cpu.registers.hl);
			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.read(cpu.registers.hl) & 0x0F));
			cpu.setFlag(cpu.Carry, cpu.read(cpu.registers.hl) > cpu.registers.a);

			instructionMode = SUB_R_R;
			break;
		}
		case 0x97:
		{
			SUBresult = cpu.registers.a - cpu.registers.a;
			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.a & 0x0F));
			cpu.setFlag(cpu.Carry, cpu.registers.a > cpu.registers.a);

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			instructionMode = SUB_R_R;
			break;
		}
		case 0x98:
		{
			SUBresult = cpu.registers.a - cpu.registers.b - carry;

			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.b & 0x0F) + carry);
			cpu.setFlag(cpu.Carry, cpu.registers.b + carry > cpu.registers.a);

			instructionMode = SUB_R_R;
			break;
		}
		case 0x99:
		{
			SUBresult = cpu.registers.a - cpu.registers.c - carry;

			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.c & 0x0F) + carry);
			cpu.setFlag(cpu.Carry, cpu.registers.c + carry > cpu.registers.a);

			instructionMode = SUB_R_R;
			break;
		}
		case 0x9A:
		{
			SUBresult = cpu.registers.a - cpu.registers.d - carry;

			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.d & 0x0F) + carry);
			cpu.setFlag(cpu.Carry, cpu.registers.d + carry > cpu.registers.a);

			instructionMode = SUB_R_R;
			break;
		}
		case 0x9B:
		{
			SUBresult = cpu.registers.a - cpu.registers.e - carry;

			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.e & 0x0F) + carry);
			cpu.setFlag(cpu.Carry, cpu.registers.e + carry > cpu.registers.a);

			instructionMode = SUB_R_R;
			break;
		}
		case 0x9C:
		{
			SUBresult = cpu.registers.a - cpu.registers.h - carry;

			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.h & 0x0F) + carry);
			cpu.setFlag(cpu.Carry, cpu.registers.h + carry > cpu.registers.a);

			instructionMode = SUB_R_R;
			break;
		}
		case 0x9D:
		{
			SUBresult = cpu.registers.a - cpu.registers.l - carry;

			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.l & 0x0F) + carry);
			cpu.setFlag(cpu.Carry, cpu.registers.l + carry > cpu.registers.a);

			instructionMode = SUB_R_R;
			break;
		}
		case 0x9E:
		{
			SUBresult = cpu.registers.a - cpu.read(cpu.registers.hl) - carry;

			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.read(cpu.registers.hl) & 0x0F) + carry);
			cpu.setFlag(cpu.Carry, cpu.read(cpu.registers.hl) + carry > cpu.registers.a);

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			instructionMode = SUB_R_R;
			break;
		}
		case 0x9F:
		{
			SUBresult = cpu.registers.a - cpu.registers.a - carry;

			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (cpu.registers.a & 0x0F) + carry);
			cpu.setFlag(cpu.Carry, cpu.registers.a + carry > cpu.registers.a);

			instructionMode = SUB_R_R;
			break;
		}
		case 0xD6:
		{
			uint8_t d8 = cpu.read(cpu.pc++);

			SUBresult = cpu.registers.a - d8;

			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (d8 & 0x0F));
			cpu.setFlag(cpu.Carry, d8 > cpu.registers.a);

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			instructionMode = SUB_R_R;
			break;
		}
		case 0xDE:
		{
			uint8_t d8 = cpu.read(cpu.pc++);

			SUBresult = cpu.registers.a - d8 - carry;

			cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (d8 & 0x0F) + carry);
			cpu.setFlag(cpu.Carry, d8 + carry > cpu.registers.a);

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			instructionMode = SUB_R_R;
			break;
		}
		case 0xA0:
		{
			r8 = &cpu.registers.b;

			instructionMode = AND_R;
			break;
		}
		case 0xA1:
		{
			r8 = &cpu.registers.c;

			instructionMode = AND_R;
			break;
		}
		case 0xA2:
		{
			r8 = &cpu.registers.d;

			instructionMode = AND_R;
			break;
		}
		case 0xA3:
		{
			r8 = &cpu.registers.e;

			instructionMode = AND_R;
			break;
		}
		case 0xA4:
		{
			r8 = &cpu.registers.h;

			instructionMode = AND_R;
			break;
		}
		case 0xA5:
		{
			r8 = &cpu.registers.l;

			instructionMode = AND_R;
			break;
		}
		case 0xA6:
		{
			hlValue = cpu.read(cpu.registers.hl);
			r8 = &hlValue;

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			instructionMode = AND_R;
			break;
		}
		case 0xA7:
		{
			r8 = &cpu.registers.a;

			instructionMode = AND_R;
			break;
		}
		case 0xE6:
		{
			D8 = cpu.read(cpu.pc++);
			r8 = &D8;

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			instructionMode = AND_R;
			break;
		}
		case 0xA8:
		{
			r8 = &cpu.registers.b;

			instructionMode = XOR_R;
			break;
		}
		case 0xA9:
		{
			r8 = &cpu.registers.c;

			instructionMode = XOR_R;
			break;
		}
		case 0xAA:
		{
			r8 = &cpu.registers.d;

			instructionMode = XOR_R;
			break;
		}
		case 0xAB:
		{
			r8 = &cpu.registers.e;

			instructionMode = XOR_R;
			break;
		}
		case 0xAC:
		{
			r8 = &cpu.registers.h;

			instructionMode = XOR_R;
			break;
		}
		case 0xAD:
		{
			r8 = &cpu.registers.l;

			instructionMode = XOR_R;
			break;
		}
		case 0xAE:
		{
			hlValue = cpu.read(cpu.registers.hl);

			r8 = &hlValue;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			instructionMode = XOR_R;
			break;
		}
		case 0xAF:
		{
			r8 = &cpu.registers.a;

			instructionMode = XOR_R;
			break;
		}
		case 0xEE:
		{
			D8 = cpu.read(cpu.pc++);

			r8 = &D8;

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			instructionMode = XOR_R;
			break;
		}
		case 0xF1: // POP AF
		{
			uint8_t lo = cpu.read(cpu.stkp++);
			uint8_t hi = cpu.read(cpu.stkp++);

			cpu.registers.af = (hi << 8) | lo;
			cpu.registers.f &= 0xF0;

			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0xF5: // PUSH AF
		{
			cpu.write(--cpu.stkp, (cpu.registers.af >> 8) & 0xFF);
			cpu.write(--cpu.stkp, cpu.registers.af & 0xFF);

			cpu.cycles += 16;
			cpu.cyclesRan += 16;

			return;
			break;
		}
		case 0xB0:
		{
			r8 = &cpu.registers.b;

			instructionMode = OR_R;
			break;
		}
		case 0xB1:
		{
			r8 = &cpu.registers.c;

			instructionMode = OR_R;
			break;
		}
		case 0xB2:
		{
			r8 = &cpu.registers.d;

			instructionMode = OR_R;
			break;
		}
		case 0xB3:
		{
			r8 = &cpu.registers.e;

			instructionMode = OR_R;
			break;
		}
		case 0xB4:
		{
			r8 = &cpu.registers.h;

			instructionMode = OR_R;
			break;
		}
		case 0xB5:
		{
			r8 = &cpu.registers.l;

			instructionMode = OR_R;
			break;
		}
		case 0xB6:
		{
			hlValue = cpu.read(cpu.registers.hl);

			r8 = &hlValue;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			instructionMode = OR_R;
			break;
		}
		case 0xB7:
		{
			r8 = &cpu.registers.a;

			instructionMode = OR_R;
			break;
		}
		case 0xF6:
		{
			D8 = cpu.read(cpu.pc++);
			r8 = &D8;

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			instructionMode = OR_R;
			break;
		}
		case 0xB8:
		{
			r8 = &cpu.registers.b;

			instructionMode = CP_R;
			break;
		}
		case 0xB9:
		{
			r8 = &cpu.registers.c;

			instructionMode = CP_R;
			break;
		}
		case 0xBA:
		{
			r8 = &cpu.registers.d;

			instructionMode = CP_R;
			break;
		}
		case 0xBB:
		{
			r8 = &cpu.registers.e;

			instructionMode = CP_R;
			break;
		}
		case 0xBC:
		{
			r8 = &cpu.registers.h;

			instructionMode = CP_R;
			break;
		}
		case 0xBD:
		{
			r8 = &cpu.registers.l;

			instructionMode = CP_R;
			break;
		}
		case 0xBE:
		{
			hlValue = cpu.read(cpu.registers.hl);
			r8 = &hlValue;

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			instructionMode = CP_R;
			break;
		}
		case 0xBF:
		{
			r8 = &cpu.registers.a;

			instructionMode = CP_R;
			break;
		}
		case 0xFE:
		{
			D8 = cpu.read(cpu.pc++);
			r8 = &D8;

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			instructionMode = CP_R;
			break;
		}
		case 0x0A:
		{
			cpu.registers.a = cpu.read(cpu.registers.bc);
			cpu.cycles += 8;

			return;
			break;
		}
		case 0x1A:
		{
			cpu.registers.a = cpu.read(cpu.registers.de);
			cpu.cycles += 8;

			return;
			break;
		}
		case 0x2A:
		{
			cpu.registers.a = cpu.read(cpu.registers.hl++);
			cpu.cycles += 8;

			return;
			break;
		}
		case 0x3A:
		{
			cpu.registers.a = cpu.read(cpu.registers.hl--);
			cpu.cycles += 8;

			return;
			break;
		}
		case 0x20:
		{
			int8_t offset = static_cast<int8_t>(cpu.read(cpu.pc++));
			if (!cpu.isFlagSet(cpu.Zero))
			{
				uint16_t targetAddress = static_cast<uint16_t>(cpu.pc) + offset;

				cpu.pc = targetAddress;
				cpu.cycles += 12;
				cpu.cyclesRan += 12;
			}
			else
			{
				cpu.cycles += 8;
				cpu.cyclesRan += 8;
			}

			return;
			break;
		}
		case 0x30:
		{
			int8_t offset = static_cast<int8_t>(cpu.read(cpu.pc++));
			if (!cpu.isFlagSet(cpu.Carry))
			{
				uint16_t targetAddress = static_cast<uint16_t>(cpu.pc) + offset;

				cpu.pc = targetAddress;
				cpu.cycles += 12;
				cpu.cyclesRan += 12;
			}
			else
			{
				cpu.cycles += 8;
				cpu.cyclesRan += 8;
			}

			return;
			break;
		}
		case 0x18:
		{
			int8_t offset = static_cast<int8_t>(cpu.read(cpu.pc++));
			uint16_t targetAddress = static_cast<uint16_t>(cpu.pc) + offset;

			cpu.pc = targetAddress;
			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0x28:
		{
			int8_t offset = static_cast<int8_t>(cpu.read(cpu.pc++));
			if (cpu.isFlagSet(cpu.Zero))
			{
				uint16_t targetAddress = static_cast<uint16_t>(cpu.pc) + offset;

				cpu.pc = targetAddress;
				cpu.cycles += 12;
				cpu.cyclesRan += 12;
			}
			else
			{
				cpu.cycles += 8;
				cpu.cyclesRan += 8;
			}

			return;
			break;
		}
		case 0x38:
		{
			int8_t offset = static_cast<int8_t>(cpu.read(cpu.pc++));
			if (cpu.isFlagSet(cpu.Carry))
			{
				uint16_t targetAddress = static_cast<uint16_t>(cpu.pc) + offset;

				cpu.pc = targetAddress;
				cpu.cycles += 12;
				cpu.cyclesRan += 12;
			}
			else
			{
				cpu.cycles += 8;
				cpu.cyclesRan += 8;
			}

			return;
			break;
		}
		case 0xC0:
		{
			if (!cpu.isFlagSet(cpu.Zero))
			{
				cpu.inCALL = false;

				uint8_t lo = cpu.read(cpu.stkp++);
				uint8_t hi = cpu.read(cpu.stkp++);

				cpu.pc = (hi << 8) | lo;

				cpu.cycles += 20;
				cpu.cyclesRan += 20;
			}
			else
			{
				cpu.cycles += 8;
				cpu.cyclesRan += 8;
			}

			return;
			break;
		}
		case 0xD0:
		{
			if (!cpu.isFlagSet(cpu.Carry))
			{
				cpu.inCALL = false;

				uint8_t lo = cpu.read(cpu.stkp++);
				uint8_t hi = cpu.read(cpu.stkp++);

				cpu.pc = (hi << 8) | lo;

				cpu.cycles += 20;
				cpu.cyclesRan += 20;
			}
			else
			{
				cpu.cycles += 8;
				cpu.cyclesRan += 8;
			}

			return;
			break;
		}
		case 0xC8:
		{
			if (cpu.isFlagSet(cpu.Zero))
			{
				cpu.inCALL = false;
				uint8_t lo = cpu.read(cpu.stkp++);
				uint8_t hi = cpu.read(cpu.stkp++);

				cpu.pc = (hi << 8) | lo;

				cpu.cycles += 20;
				cpu.cyclesRan += 20;
			}
			else
			{
				cpu.cycles += 8;
				cpu.cyclesRan += 8;
			}

			return;
			break;
		}
		case 0xC9:
		{
			cpu.inCALL = false;

			uint8_t lo = cpu.read(cpu.stkp++);
			uint8_t hi = cpu.read(cpu.stkp++);

			cpu.pc = (hi << 8) | lo;

			cpu.cycles += 16;
			cpu.cyclesRan += 16;

			return;
			break;
		}
		case 0xD8:
		{
			if (cpu.isFlagSet(cpu.Carry))
			{
				cpu.inCALL = false;

				uint8_t lo = cpu.read(cpu.stkp++);
				uint8_t hi = cpu.read(cpu.stkp++);

				cpu.pc = (hi << 8) | lo;

				cpu.cycles += 20;
				cpu.cyclesRan += 20;
			}
			else
			{
				cpu.cycles += 8;
				cpu.cyclesRan += 8;
			}

			return;
			break;
		}
		case 0xD9:
		{
			//std::cout << "RETURNING FROM INTERRUPT" << std::endl;
			if (cpu.inVblank)
			{
				cpu.inVblank = false;
			}
			cpu.ime = 1;
			uint8_t lo = cpu.read(cpu.stkp++);
			uint8_t hi = cpu.read(cpu.stkp++);

			cpu.pc = (hi << 8) | lo;

			cpu.cycles += 16;
			cpu.cyclesRan += 16;

			return;
			break;
		}
		case 0xC1: // POP BC
		{
			uint8_t lo = cpu.read(cpu.stkp++);
			uint8_t hi = cpu.read(cpu.stkp++);

			cpu.registers.bc = (hi << 8) | lo;

			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0xD1: // POP DE
		{
			uint8_t lo = cpu.read(cpu.stkp++);
			uint8_t hi = cpu.read(cpu.stkp++);

			cpu.registers.de = (hi << 8) | lo;

			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0xE1: // POP HL
		{
			uint8_t lo = cpu.read(cpu.stkp++);
			uint8_t hi = cpu.read(cpu.stkp++);

			cpu.registers.hl = (hi << 8) | lo;

			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0xC2: // JP NZ,a16
		{
			uint8_t lo = cpu.read(cpu.pc);
			cpu.pc++;
			uint8_t hi = cpu.read(cpu.pc);
			cpu.pc++;
			if (!cpu.isFlagSet(cpu.Zero))
			{
				cpu.pc = (hi << 8) | lo;
				cpu.cycles += 16;
				cpu.cyclesRan += 16;
			}
			else
			{
				cpu.cycles += 12;
				cpu.cyclesRan += 12;
			}

			return;
			break;
		}
		case 0xCA: // JP Z,a16
		{
			uint8_t lo = cpu.read(cpu.pc);
			cpu.pc++;
			uint8_t hi = cpu.read(cpu.pc);
			cpu.pc++;
			if (cpu.isFlagSet(cpu.Zero))
			{
				cpu.pc = (hi << 8) | lo;
				cpu.cycles += 16;
				cpu.cyclesRan += 16;
			}
			else
			{
				cpu.cycles += 12;
				cpu.cyclesRan += 12;
			}

			return;
			break;
		}
		case 0xD2: // JP NC,a16
		{
			uint8_t lo = cpu.read(cpu.pc);
			cpu.pc++;
			uint8_t hi = cpu.read(cpu.pc);
			cpu.pc++;

			if (!cpu.isFlagSet(cpu.Carry))
			{
				cpu.pc = (hi << 8) | lo;
				cpu.cycles += 16;
				cpu.cyclesRan += 16;
			}
			else
			{
				cpu.cycles += 12;
				cpu.cyclesRan += 12;
			}

			return;
			break;
		}
		case 0xDA: // JP C,a16
		{
			uint8_t lo = cpu.read(cpu.pc);
			cpu.pc++;
			uint8_t hi = cpu.read(cpu.pc);
			cpu.pc++;

			if (cpu.isFlagSet(cpu.Carry))
			{
				cpu.pc = (hi << 8) | lo;
				cpu.cycles += 4;
				cpu.cyclesRan += 4;
			}
			else
			{
				cpu.cycles += 12;
				cpu.cyclesRan += 12;
			}

			return;
			break;
		}
		case 0xC5:
		{
			cpu.write(--cpu.stkp, (cpu.registers.bc >> 8) & 0xFF);
			cpu.write(--cpu.stkp, cpu.registers.bc & 0xFF);
			cpu.cycles += 16;
			cpu.cyclesRan += 16;

			return;
			break;
		}
		case 0xD5:
		{
			cpu.write(--cpu.stkp, (cpu.registers.de >> 8) & 0xFF);
			cpu.write(--cpu.stkp, cpu.registers.de & 0xFF);
			cpu.cycles += 16;
			cpu.cyclesRan += 16;

			return;
			break;
		}
		case 0xE5:
		{
			cpu.write(--cpu.stkp, (cpu.registers.hl >> 8) & 0xFF);
			cpu.write(--cpu.stkp, cpu.registers.hl & 0xFF);
			cpu.cycles += 16;
			cpu.cyclesRan += 16;

			return;
			break;
		}
		case 0xC7:
		{
			n = 0x00;

			instructionMode = RST_N;
			break;
		}
		case 0xCF:
		{
			n = 0x08;

			instructionMode = RST_N;
			break;
		}
		case 0xD7:
		{
			n = 0x10;

			instructionMode = RST_N;
			break;
		}
		case 0xDF:
		{
			n = 0x18;

			instructionMode = RST_N;
			break;
		}
		case 0xE7:
		{
			n = 0x20;

			instructionMode = RST_N;
			break;
		}
		case 0xEF:
		{
			n = 0x28;

			instructionMode = RST_N;
			break;
		}
		case 0xF7:
		{
			n = 0x30;

			instructionMode = RST_N;
			break;
		}
		case 0xFF:
		{
			n = 0x38;

			instructionMode = RST_N;
			break;
		}
		case 0xC4: // CALL NZ, a16
		{
			uint8_t lo = cpu.read(cpu.pc++);

			uint8_t hi = cpu.read(cpu.pc++);
			if (!cpu.isFlagSet(cpu.Zero))
			{
				cpu.inCALL = true;

				uint16_t nn = (hi << 8) | lo;

				cpu.write(--cpu.stkp, (cpu.pc >> 8) & 0xFF);
				cpu.write(--cpu.stkp, cpu.pc & 0xFF);

				cpu.pc = nn;

				cpu.cycles += 24;
				cpu.cyclesRan += 24;
			}
			else
			{
				cpu.cycles += 12;
				cpu.cyclesRan += 12;
			}

			return;
			break;
		}
		case 0xCC: // CALL Z, a16
		{
			uint8_t lo = cpu.read(cpu.pc++);

			uint8_t hi = cpu.read(cpu.pc++);
			if (cpu.isFlagSet(cpu.Zero))
			{
				cpu.inCALL = true;

				uint16_t nn = (hi << 8) | lo;

				cpu.write(--cpu.stkp, (cpu.pc >> 8) & 0xFF);
				cpu.write(--cpu.stkp, cpu.pc & 0xFF);

				cpu.pc = nn;

				cpu.cycles += 24;
				cpu.cyclesRan += 24;
			}
			else
			{
				cpu.cycles += 12;
				cpu.cyclesRan += 12;
			}

			return;
			break;
		}
		case 0xCD: // CALL a16
		{
			cpu.inCALL = true;
			uint8_t lo = cpu.read(cpu.pc++);

			uint8_t hi = cpu.read(cpu.pc++);

			uint16_t nn = (hi << 8) | lo;

			cpu.write(--cpu.stkp, (cpu.pc >> 8) & 0xFF);
			cpu.write(--cpu.stkp, cpu.pc & 0xFF);

			cpu.pc = nn;

			cpu.cycles += 24;
			cpu.cyclesRan += 24;

			return;
			break;
		}
		case 0xD4: // CALL NC, a16
		{
			uint8_t lo = cpu.read(cpu.pc++);

			uint8_t hi = cpu.read(cpu.pc++);
			if (!cpu.isFlagSet(cpu.Carry))
			{
				cpu.inCALL = true;

				uint16_t nn = (hi << 8) | lo;

				cpu.write(--cpu.stkp, (cpu.pc >> 8) & 0xFF);
				cpu.write(--cpu.stkp, cpu.pc & 0xFF);

				cpu.pc = nn;

				cpu.cycles += 24;
				cpu.cyclesRan += 24;
			}
			else
			{
				cpu.cycles += 12;
				cpu.cyclesRan += 12;
			}

			return;
			break;
		}
		case 0xDC: // CALL C, a16
		{
			uint8_t lo = cpu.read(cpu.pc++);

			uint8_t hi = cpu.read(cpu.pc++);
			if (cpu.isFlagSet(cpu.Carry))
			{
				cpu.inCALL = true;

				uint16_t nn = (hi << 8) | lo;

				cpu.write(--cpu.stkp, (cpu.pc >> 8) & 0xFF);
				cpu.write(--cpu.stkp, cpu.pc & 0xFF);

				cpu.pc = nn;

				cpu.cycles += 24;
				cpu.cyclesRan += 24;
			}
			else
			{
				cpu.cycles += 12;
				cpu.cyclesRan += 12;
			}

			return;
			break;
		}
		case 0xCB:
		{
			cpu.wasCB = true;

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0xE0: // LDH (a8), A
		{
			uint16_t a8 = 0xFF00 + cpu.read(cpu.pc++);
			cpu.write(a8, cpu.registers.a);
			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0xF0: // LDH A, (a8)
		{
			uint16_t a8 = 0xFF00 + cpu.read(cpu.pc++);
			cpu.registers.a = cpu.read(a8);
			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0xE2: // LD (C),A
		{
			cpu.write(0xFF00 + cpu.registers.c, cpu.registers.a);
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0xF2: // LD A,(C)
		{
			cpu.registers.a = cpu.read(0xFF00 + cpu.registers.c);
			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0xE8: // ADD SP, r8
		{
			int data = static_cast<int>(static_cast<int8_t>(cpu.read(cpu.pc++)));
			int result = cpu.stkp + data;

			cpu.setFlag(cpu.Zero, false);
			cpu.setFlag(cpu.Subtraction, false);
			cpu.setFlag(cpu.HalfCarry, ((cpu.stkp & 0x0F) + (data & 0x0F)) > 0x0F);
			cpu.setFlag(cpu.Carry, ((cpu.stkp & 0xFF) + (data & 0xFF)) > 0xFF);

			cpu.stkp = static_cast<uint16_t>(result & 0xFFFF);

			cpu.cycles += 16;
			cpu.cyclesRan += 16;

			return;
			break;
		}
		case 0xE9: // JP (HL)
		{
			cpu.pc = cpu.registers.hl;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0xEA: // LD (a16), A
		{
			uint8_t lo = cpu.read(cpu.pc++);
			uint8_t hi = cpu.read(cpu.pc++);

			cpu.write((hi << 8) | lo, cpu.registers.a);
			cpu.cycles += 16;
			cpu.cyclesRan += 16;

			return;
			break;
		}
		case 0xFA: // LD A, (a16)
		{
			uint8_t lo = cpu.read(cpu.pc++);
			uint8_t hi = cpu.read(cpu.pc++);

			cpu.registers.a = cpu.read((hi << 8) | lo);
			cpu.cycles += 16;
			cpu.cyclesRan += 16;

			return;
			break;
		}
		case 0xF3: // DI
		{
			cpu.ime = 0;

			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0xF8: // LD HL, SP+r8
		{
			int data = static_cast<int>(static_cast<int8_t>(cpu.read(cpu.pc++)));

			int result = cpu.stkp + data;

			cpu.setFlag(cpu.Zero, false);
			cpu.setFlag(cpu.Subtraction, false);
			cpu.setFlag(cpu.HalfCarry, ((cpu.stkp & 0x0F) + (data & 0x0F)) > 0x0F);
			cpu.setFlag(cpu.Carry, ((cpu.stkp & 0xFF) + (data & 0xFF)) > 0xFF);

			cpu.registers.hl = static_cast<uint16_t>(result & 0xFFFF);

			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0xF9: // LD SP, HL
		{
			cpu.stkp = cpu.registers.hl;

			cpu.cycles += 8;
			cpu.cyclesRan += 8;

			return;
			break;
		}
		case 0xFB: // EI
		{
			cpu.ime = 1;
			cpu.cycles += 4;
			cpu.cyclesRan += 4;

			return;
			break;
		}
		case 0xC3: // JP a16
		{
			uint8_t lo = cpu.read(cpu.pc++);
			uint8_t hi = cpu.read(cpu.pc++);

			uint16_t addr = (hi << 8) | lo;

			cpu.pc = addr;
			cpu.cycles += 16;
			cpu.cyclesRan += 16;

			return;
			break;
		}
		default:
		{
			std::cout << "Unknown/Illegal instruction: " << "PREFIX CB: " << cpu.wasCB << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			cpu.setHasNotBroken(false);
			return;
			break;
		}
	}


	if (instructionMode)
	{
		switch (instructionMode)
		{
			case LD_RR_D16:
			{
				uint8_t lo = cpu.read(cpu.pc);
				cpu.pc++;
				uint8_t hi = cpu.read(cpu.pc);
				cpu.pc++;

				uint16_t value = (hi << 8) | lo;

				cpu.cycles += 12;
				cpu.cyclesRan += 12;

				(*r16) = value;

				return;
				break;
			}
			case INR_R:
			{
				(*r8)++;

				cpu.setFlag(cpu.Zero, (*r8) == 0);
				cpu.setFlag(cpu.Subtraction, false);
				cpu.setFlag(cpu.HalfCarry, ((*r8) & 0x0F) == 0);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;

				return;
				break;
			}
			case LD_R_D8:
			{
				uint8_t value = cpu.read(cpu.pc++);

				(*r8) = value;

				cpu.cycles += 8;
				cpu.cyclesRan += 8;

				return;
				break;
			}
			case ADD_RR_RR:
			{
				uint32_t result = static_cast<uint32_t>(cpu.registers.hl) + static_cast<uint32_t>(*r16);

				cpu.setFlag(cpu.Subtraction, false);
				cpu.setFlag(cpu.HalfCarry, ((cpu.registers.hl & 0xFFF) + (*r16 & 0xFFF)) > 0xFFF);
				cpu.setFlag(cpu.Carry, result > 0xFFFF);

				cpu.registers.hl = static_cast<uint16_t>(result & 0xFFFF);

				cpu.cycles += 8;
				cpu.cyclesRan += 8;

				return;
				break;
			}
			case ADD_R_R:
			{
				cpu.setFlag(cpu.Zero, (ADDresult & 0xFF) == 0x00);
				cpu.setFlag(cpu.Subtraction, false);
				cpu.setFlag(cpu.Carry, ADDresult > 0xFF);

				cpu.registers.a = ADDresult & 0xFF;

				cpu.cycles += 4;
				cpu.cyclesRan += 4;

				return;
				break;
			}
			case SUB_R_R:
			{
				cpu.setFlag(cpu.Zero, SUBresult == 0x00);
				cpu.setFlag(cpu.Subtraction, true);

				cpu.registers.a = SUBresult;

				cpu.cycles += 4;
				cpu.cyclesRan += 4;

				return;
				break;
			}
			case AND_R:
			{
				cpu.registers.a &= (*r8);

				cpu.setFlag(cpu.Zero, cpu.registers.a == 0x00);
				cpu.setFlag(cpu.Subtraction, false);
				cpu.setFlag(cpu.HalfCarry, true);
				cpu.setFlag(cpu.Carry, false);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;

				return;
				break;
			}
			case XOR_R:
			{
				cpu.registers.a ^= (*r8);

				cpu.setFlag(cpu.Zero, cpu.registers.a == 0x00);
				cpu.setFlag(cpu.Subtraction, false);
				cpu.setFlag(cpu.HalfCarry, false);
				cpu.setFlag(cpu.Carry, false);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;

				return;
				break;
			}
			case OR_R:
			{
				cpu.registers.a |= (*r8);

				cpu.setFlag(cpu.Zero, cpu.registers.a == 0x00);
				cpu.setFlag(cpu.Subtraction, false);
				cpu.setFlag(cpu.HalfCarry, false);
				cpu.setFlag(cpu.Carry, false);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;

				return;
				break;
			}
			case CP_R:
			{
				uint8_t CPresult = cpu.registers.a - (*r8);

				cpu.setFlag(cpu.Zero, CPresult == 0x00);
				cpu.setFlag(cpu.Subtraction, true);
				cpu.setFlag(cpu.HalfCarry, (cpu.registers.a & 0x0F) < (*r8 & 0x0F));
				cpu.setFlag(cpu.Carry, (*r8) > cpu.registers.a);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;

				return;
				break;
			}
			case RST_N:
			{
				cpu.write(--cpu.stkp, (cpu.pc >> 8) & 0xFF);
				cpu.write(--cpu.stkp, cpu.pc & 0xFF);

				cpu.pc = (0x00 << 8) | n;

				cpu.cycles += 16;
				cpu.cyclesRan += 16;

				return;
				break;
			}
			default:
			{
				std::cout << "Unknown/Illegal instruction: " << "PREFIX CB: " << cpu.wasCB << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
				cpu.setHasNotBroken(false);
			}
		}
	}
}

void CpuInstructions::runCBInstruction(Cpu& cpu)
{
	uint8_t opcode = cpu.read(cpu.pc++);
	uint16_t* r16 = nullptr;
	uint8_t* r8 = nullptr;

	int CBinstructionMode = ILLEGAL;

	switch (opcode)
	{
		case 0x00:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = RLC_R8;
			break;
		}
		case 0x01:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = RLC_R8;
			break;
		}
		case 0x02:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = RLC_R8;
			break;
		}
		case 0x03:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = RLC_R8;
			break;
		}
		case 0x04:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = RLC_R8;
			break;
		}
		case 0x05:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = RLC_R8;
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

			return;
			break;
		}
		case 0x07:
		{
			r8 = &cpu.registers.a;
			
			CBinstructionMode = RLC_R8;
			break;
		}
		case 0x08:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = RRC_R8;
			break;
		}
		case 0x09:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = RRC_R8;
			break;
		}
		case 0x0A:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = RRC_R8;
			break;
		}
		case 0x0B:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = RRC_R8;
			break;
		}
		case 0x0C:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = RRC_R8;
			break;
		}
		case 0x0D:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = RRC_R8;
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

			return;
			break;
		}
		case 0x0F:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = RRC_R8;
			break;
		}
		case 0x10:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = RL_R8;
			break;
		}
		case 0x11:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = RL_R8;
			break;
		}
		case 0x12:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = RL_R8;
			break;
		}
		case 0x13:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = RL_R8;
			break;
		}
		case 0x14:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = RL_R8;
			break;
		}
		case 0x15:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = RL_R8;
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

			return;
			break;
		}
		case 0x17:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = RL_R8;
			break;
		}
		case 0x18:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = RR_R8;
			break;
		}
		case 0x19:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = RR_R8;
			break;
		}
		case 0x1A:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = RR_R8;
			break;
		}
		case 0x1B:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = RR_R8;
			break;
		}
		case 0x1C:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = RR_R8;
			break;
		}
		case 0x1D:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = RR_R8;
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

			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0x1F:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = RR_R8;
			break;
		}
		case 0x20:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = SLA_R8;
			break;
		}
		case 0x21:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = SLA_R8;
			break;
		}
		case 0x22:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = SLA_R8;
			break;
		}
		case 0x23:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = SLA_R8;
			break;
		}
		case 0x24:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = SLA_R8;
			break;
		}
		case 0x25:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = SLA_R8;
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

			return;
			break;
		}
		case 0x27:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = SLA_R8;
			break;
		}
		case 0x28:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = SRA_R8;
			break;
		}
		case 0x29:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = SRA_R8;
			break;
		}
		case 0x2A:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = SRA_R8;
			break;
		}
		case 0x2B:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = SRA_R8;
			break;
		}
		case 0x2C:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = SRA_R8;
			break;
		}
		case 0x2D:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = SRA_R8;
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

			return;
			break;
		}
		case 0x2F:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = SRA_R8;
			break;
		}
		case 0x30:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = SWAP_R8;
			break;
		}
		case 0x31:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = SWAP_R8;
			break;
		}
		case 0x32:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = SWAP_R8;
			break;
		}
		case 0x33:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = SWAP_R8;
			break;
		}
		case 0x34:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = SWAP_R8;
			break;
		}
		case 0x35:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = SWAP_R8;
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

			return;
			break;
		}
		case 0x37:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = SWAP_R8;
			break;
		}
		case 0x38:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = SRL_R8;
			break;
		}
		case 0x39:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = SRL_R8;
			break;
		}
		case 0x3A:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = SRL_R8;
			break;
		}
		case 0x3B:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = SRL_R8;
			break;
		}
		case 0x3C:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = SRL_R8;
			break;
		}
		case 0x3D:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = SRL_R8;
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

			return;
			break;
		}
		case 0x3F:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = SRL_R8;
			break;
		}
		case 0x40:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = BIT_0_R8;
			break;
		}
		case 0x41:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = BIT_0_R8;
			break;
		}
		case 0x42:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = BIT_0_R8;
			break;
		}
		case 0x43:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = BIT_0_R8;
			break;
		}
		case 0x44:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = BIT_0_R8;
			break;
		}
		case 0x45:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = BIT_0_R8;
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

			return;
			break;
		}
		case 0x47:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = BIT_0_R8;
			break;
		}
		case 0x48:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = BIT_1_R8;
			break;
		}
		case 0x49:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = BIT_1_R8;
			break;
		}
		case 0x4A:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = BIT_1_R8;
			break;
		}
		case 0x4B:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = BIT_1_R8;
			break;
		}
		case 0x4C:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = BIT_1_R8;
			break;
		}
		case 0x4D:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = BIT_1_R8;
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

			return;
			break;
		}
		case 0x4F:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = BIT_1_R8;
			break;
		}
		case 0x50:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = BIT_2_R8;
			break;
		}
		case 0x51:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = BIT_2_R8;
			break;
		}
		case 0x52:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = BIT_2_R8;
			break;
		}
		case 0x53:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = BIT_2_R8;
			break;
		}
		case 0x54:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = BIT_2_R8;
			break;
		}
		case 0x55:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = BIT_2_R8;
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

			return;
			break;
		}
		case 0x57:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = BIT_2_R8;
			break;
		}
		case 0x58:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = BIT_3_R8;
			break;
		}
		case 0x59:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = BIT_3_R8;
			break;
		}
		case 0x5A:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = BIT_3_R8;
			break;
		}
		case 0x5B:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = BIT_3_R8;
			break;
		}
		case 0x5C:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = BIT_3_R8;
			break;
		}
		case 0x5D:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = BIT_3_R8;
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

			return;
			break;
		}
		case 0x5F:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = BIT_3_R8;
			break;
		}
		case 0x60:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = BIT_4_R8;
			break;
		}
		case 0x61:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = BIT_4_R8;
			break;
		}
		case 0x62:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = BIT_4_R8;
			break;
		}
		case 0x63:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = BIT_4_R8;
			break;
		}
		case 0x64:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = BIT_4_R8;
			break;
		}
		case 0x65:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = BIT_4_R8;
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

			return;
			break;
		}
		case 0x67:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = BIT_4_R8;
			break;
		}
		case 0x68:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = BIT_5_R8;
			break;
		}
		case 0x69:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = BIT_5_R8;
			break;
		}
		case 0x6A:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = BIT_5_R8;
			break;
		}
		case 0x6B:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = BIT_5_R8;
			break;
		}
		case 0x6C:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = BIT_5_R8;
			break;
		}
		case 0x6D:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = BIT_5_R8;
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

			return;
			break;
		}
		case 0x6F:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = BIT_5_R8;
			break;
		}
		case 0x70:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = BIT_6_R8;
			break;
		}
		case 0x71:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = BIT_6_R8;
			break;
		}
		case 0x72:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = BIT_6_R8;
			break;
		}
		case 0x73:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = BIT_6_R8;
			break;
		}
		case 0x74:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = BIT_6_R8;
			break;
		}
		case 0x75:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = BIT_6_R8;
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

			return;
			break;
		}
		case 0x77:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = BIT_6_R8;
			break;
		}
		case 0x78:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = BIT_7_R8;
			break;
		}
		case 0x79:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = BIT_7_R8;
			break;
		}
		case 0x7A:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = BIT_7_R8;
			break;
		}
		case 0x7B:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = BIT_7_R8;
			break;
		}
		case 0x7C:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = BIT_7_R8;
			break;
		}
		case 0x7D:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = BIT_7_R8;
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

			return;
			break;
		}
		case 0x7F:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = BIT_7_R8;
			break;
		}
		case 0x80:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = RES_0_R8;
			break;
		}
		case 0x81:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = RES_0_R8;
			break;
		}
		case 0x82:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = RES_0_R8;
			break;
		}
		case 0x83:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = RES_0_R8;
			break;
		}
		case 0x84:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = RES_0_R8;
			break;
		}
		case 0x85:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = RES_0_R8;
			break;
		}
		case 0x86:
		{
			uint8_t value = cpu.read(cpu.registers.hl);
			value = value & ~(1 << 0);

			cpu.write(cpu.registers.hl, value);

			cpu.cycles += 12;
			cpu.cyclesRan += 12;
			
			return;
			break;
		}
		case 0x87:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = RES_0_R8;
			break;
		}
		case 0x88:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = RES_1_R8;
			break;
		}
		case 0x89:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = RES_1_R8;
			break;
		}
		case 0x8A:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = RES_1_R8;
			break;
		}
		case 0x8B:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = RES_1_R8;
			break;
		}
		case 0x8C:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = RES_1_R8;
			break;
		}
		case 0x8D:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = RES_1_R8;
			break;
		}
		case 0x8E:
		{
			uint8_t value = cpu.read(cpu.registers.hl);
			value = value & ~(1 << 1);

			cpu.write(cpu.registers.hl, value);

			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0x8F:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = RES_1_R8;
			break;
		}
		case 0x90:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = RES_2_R8;
			break;
		}
		case 0x91:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = RES_2_R8;
			break;
		}
		case 0x92:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = RES_2_R8;
			break;
		}
		case 0x93:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = RES_2_R8;
			break;
		}
		case 0x94:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = RES_2_R8;
			break;
		}
		case 0x95:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = RES_2_R8;
			break;
		}
		case 0x96:
		{
			uint8_t value = cpu.read(cpu.registers.hl);
			value = value & ~(1 << 2);

			cpu.write(cpu.registers.hl, value);

			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0x97:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = RES_2_R8;
			break;
		}
		case 0x98:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = RES_3_R8;
			break;
		}
		case 0x99:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = RES_3_R8;
			break;
		}
		case 0x9A:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = RES_3_R8;
			break;
		}
		case 0x9B:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = RES_3_R8;
			break;
		}
		case 0x9C:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = RES_3_R8;
			break;
		}
		case 0x9D:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = RES_3_R8;
			break;
		}
		case 0x9E:
		{
			uint8_t value = cpu.read(cpu.registers.hl);
			value = value & ~(1 << 3);

			cpu.write(cpu.registers.hl, value);

			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0x9F:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = RES_3_R8;
			break;
		}
		case 0xA0:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = RES_4_R8;
			break;
		}
		case 0xA1:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = RES_4_R8;
			break;
		}
		case 0xA2:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = RES_4_R8;
			break;
		}
		case 0xA3:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = RES_4_R8;
			break;
		}
		case 0xA4:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = RES_4_R8;
			break;
		}
		case 0xA5:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = RES_4_R8;
			break;
		}
		case 0xA6:
		{
			uint8_t value = cpu.read(cpu.registers.hl);
			value = value & ~(1 << 4);

			cpu.write(cpu.registers.hl, value);

			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0xA7:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = RES_4_R8;
			break;
		}
		case 0xA8:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = RES_5_R8;
			break;
		}
		case 0xA9:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = RES_5_R8;
			break;
		}
		case 0xAA:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = RES_5_R8;
			break;
		}
		case 0xAB:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = RES_5_R8;
			break;
		}
		case 0xAC:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = RES_5_R8;
			break;
		}
		case 0xAD:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = RES_5_R8;
			break;
		}
		case 0xAE:
		{
			uint8_t value = cpu.read(cpu.registers.hl);
			value = value & ~(1 << 5);

			cpu.write(cpu.registers.hl, value);

			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0xAF:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = RES_5_R8;
			break;
		}
		case 0xB0:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = RES_6_R8;
			break;
		}
		case 0xB1:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = RES_6_R8;
			break;
		}
		case 0xB2:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = RES_6_R8;
			break;
		}
		case 0xB3:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = RES_6_R8;
			break;
		}
		case 0xB4:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = RES_6_R8;
			break;
		}
		case 0xB5:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = RES_6_R8;
			break;
		}
		case 0xB6:
		{
			uint8_t value = cpu.read(cpu.registers.hl);
			value = value & ~(1 << 6);

			cpu.write(cpu.registers.hl, value);

			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0xB7:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = RES_6_R8;
			break;
		}
		case 0xB8:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = RES_7_R8;
			break;
		}
		case 0xB9:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = RES_7_R8;
			break;
		}
		case 0xBA:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = RES_7_R8;
			break;
		}
		case 0xBB:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = RES_7_R8;
			break;
		}
		case 0xBC:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = RES_7_R8;
			break;
		}
		case 0xBD:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = RES_7_R8;
			break;
		}
		case 0xBE:
		{
			uint8_t value = cpu.read(cpu.registers.hl);
			value = value & ~(1 << 7);

			cpu.write(cpu.registers.hl, value);

			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0xBF:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = RES_7_R8;
			break;
		}
		case 0xC0:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = SET_0_R8;
			break;
		}
		case 0xC1:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = SET_0_R8;
			break;
		}
		case 0xC2:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = SET_0_R8;
			break;
		}
		case 0xC3:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = SET_0_R8;
			break;
		}
		case 0xC4:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = SET_0_R8;
			break;
		}
		case 0xC5:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = SET_0_R8;
			break;
		}
		case 0xC6:
		{
			uint8_t value = cpu.read(cpu.registers.hl);
			value |= (1 << 0);

			cpu.write(cpu.registers.hl, value);

			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0xC7:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = SET_0_R8;
			break;
		}
		case 0xC8:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = SET_1_R8;
			break;
		}
		case 0xC9:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = SET_1_R8;
			break;
		}
		case 0xCA:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = SET_1_R8;
			break;
		}
		case 0xCB:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = SET_1_R8;
			break;
		}
		case 0xCC:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = SET_1_R8;
			break;
		}
		case 0xCD:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = SET_1_R8;
			break;
		}
		case 0xCE:
		{
			uint8_t value = cpu.read(cpu.registers.hl);
			value |= (1 << 1);

			cpu.write(cpu.registers.hl, value);

			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0xCF:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = SET_1_R8;
			break;
		}
		case 0xD0:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = SET_2_R8;
			break;
		}
		case 0xD1:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = SET_2_R8;
			break;
		}
		case 0xD2:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = SET_2_R8;
			break;
		}
		case 0xD3:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = SET_2_R8;
			break;
		}
		case 0xD4:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = SET_2_R8;
			break;
		}
		case 0xD5:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = SET_2_R8;
			break;
		}
		case 0xD6:
		{
			uint8_t value = cpu.read(cpu.registers.hl);
			value |= (1 << 2);

			cpu.write(cpu.registers.hl, value);

			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0xD7:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = SET_2_R8;
			break;
		}
		case 0xD8:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = SET_3_R8;
			break;
		}
		case 0xD9:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = SET_3_R8;
			break;
		}
		case 0xDA:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = SET_3_R8;
			break;
		}
		case 0xDB:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = SET_3_R8;
			break;
		}
		case 0xDC:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = SET_3_R8;
			break;
		}
		case 0xDD:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = SET_3_R8;
			break;
		}
		case 0xDE:
		{
			uint8_t value = cpu.read(cpu.registers.hl);
			value |= (1 << 3);

			cpu.write(cpu.registers.hl, value);

			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0xDF:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = SET_3_R8;
			break;
		}
		case 0xE0:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = SET_4_R8;
			break;
		}
		case 0xE1:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = SET_4_R8;
			break;
		}
		case 0xE2:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = SET_4_R8;
			break;
		}
		case 0xE3:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = SET_4_R8;
			break;
		}
		case 0xE4:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = SET_4_R8;
			break;
		}
		case 0xE5:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = SET_4_R8;
			break;
		}
		case 0xE6:
		{
			uint8_t value = cpu.read(cpu.registers.hl);
			value |= (1 << 4);

			cpu.write(cpu.registers.hl, value);

			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0xE7:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = SET_4_R8;
			break;
		}
		case 0xE8:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = SET_5_R8;
			break;
		}
		case 0xE9:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = SET_5_R8;
			break;
		}
		case 0xEA:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = SET_5_R8;
			break;
		}
		case 0xEB:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = SET_5_R8;
			break;
		}
		case 0xEC:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = SET_5_R8;
			break;
		}
		case 0xED:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = SET_5_R8;
			break;
		}
		case 0xEE:
		{
			uint8_t value = cpu.read(cpu.registers.hl);
			value |= (1 << 5);

			cpu.write(cpu.registers.hl, value);

			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0xEF:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = SET_5_R8;
			break;
		}
		case 0xF0:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = SET_6_R8;
			break;
		}
		case 0xF1:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = SET_6_R8;
			break;
		}
		case 0xF2:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = SET_6_R8;
			break;
		}
		case 0xF3:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = SET_6_R8;
			break;
		}
		case 0xF4:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = SET_6_R8;
			break;
		}
		case 0xF5:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = SET_6_R8;
			break;
		}
		case 0xF6:
		{
			uint8_t value = cpu.read(cpu.registers.hl);
			value |= (1 << 6);

			cpu.write(cpu.registers.hl, value);

			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0xF7:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = SET_6_R8;
			break;
		}
		case 0xF8:
		{
			r8 = &cpu.registers.b;

			CBinstructionMode = SET_7_R8;
			break;
		}
		case 0xF9:
		{
			r8 = &cpu.registers.c;

			CBinstructionMode = SET_7_R8;
			break;
		}
		case 0xFA:
		{
			r8 = &cpu.registers.d;

			CBinstructionMode = SET_7_R8;
			break;
		}
		case 0xFB:
		{
			r8 = &cpu.registers.e;

			CBinstructionMode = SET_7_R8;
			break;
		}
		case 0xFC:
		{
			r8 = &cpu.registers.h;

			CBinstructionMode = SET_7_R8;
			break;
		}
		case 0xFD:
		{
			r8 = &cpu.registers.l;

			CBinstructionMode = SET_7_R8;
			break;
		}
		case 0xFE:
		{
			uint8_t value = cpu.read(cpu.registers.hl);
			value |= (1 << 7);

			cpu.write(cpu.registers.hl, value);

			cpu.cycles += 12;
			cpu.cyclesRan += 12;

			return;
			break;
		}
		case 0xFF:
		{
			r8 = &cpu.registers.a;

			CBinstructionMode = SET_7_R8;
			break;
		}
		default:
		{
			std::cout << "Unknown/Illegal instruction: " << "PREFIX CB: " << cpu.wasCB << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
			cpu.setHasNotBroken(false);
			return;
			break;
		}
	}

	if (CBinstructionMode)
	{
		switch (CBinstructionMode)
		{
			case RLC_R8:
			{
				bool msb = (*r8 & 0x80) != 0;

				*r8 = (*r8 << 1) | msb;

				cpu.setFlag(cpu.Zero, *r8 == 0);
				cpu.setFlag(cpu.Subtraction, false);
				cpu.setFlag(cpu.HalfCarry, false);
				cpu.setFlag(cpu.Carry, msb);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;

				return;
				break;
			}
			case RRC_R8:
			{
				bool carry = *r8 & 0x01;

				*r8 = (*r8 >> 1) | (carry << 7);

				cpu.setFlag(cpu.Zero, *r8 == 0);
				cpu.setFlag(cpu.Subtraction, false);
				cpu.setFlag(cpu.HalfCarry, false);
				cpu.setFlag(cpu.Carry, carry);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;

				return;
				break;
			}
			case RL_R8:
			{
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

				return;
				break;
			}
			case RR_R8:
			{
				bool carry = cpu.isFlagSet(cpu.Carry);
				bool new_carry = *r8 & 0x01;

				*r8 = (*r8 >> 1) | (carry << 7);

				cpu.setFlag(cpu.Zero, *r8 == 0);
				cpu.setFlag(cpu.Subtraction, false);
				cpu.setFlag(cpu.HalfCarry, false);
				cpu.setFlag(cpu.Carry, new_carry);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;

				return;
				break;
			}
			case SLA_R8:
			{
				bool carry = (*r8 & 0x80) != 0;

				*r8 <<= 1;

				cpu.setFlag(cpu.Zero, *r8 == 0x00);
				cpu.setFlag(cpu.Subtraction, false);
				cpu.setFlag(cpu.HalfCarry, false);
				cpu.setFlag(cpu.Carry, carry);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;

				return;
				break;
			}
			case SRA_R8:
			{
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

				return;
				break;
			}
			case SWAP_R8:
			{
				*r8 = ((*r8 & 0x0F) << 4) | ((*r8 & 0xF0) >> 4);

				cpu.setFlag(cpu.Zero, *r8 == 0);
				cpu.setFlag(cpu.Subtraction, false);
				cpu.setFlag(cpu.HalfCarry, false);
				cpu.setFlag(cpu.Carry, false);

				cpu.cycles += 8;
				cpu.cyclesRan += 8;

				return;
				break;
			}
			case SRL_R8:
			{
				bool carry = *r8 & 0x01;

				*r8 >>= 1;

				cpu.setFlag(cpu.Zero, *r8 == 0);
				cpu.setFlag(cpu.Subtraction, false);
				cpu.setFlag(cpu.HalfCarry, false);
				cpu.setFlag(cpu.Carry, carry);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;

				return;
				break;
			}
			case BIT_0_R8:
			{
				cpu.setFlag(cpu.Zero, !(*r8& (1 << 0)));
				cpu.setFlag(cpu.Subtraction, false);
				cpu.setFlag(cpu.HalfCarry, true);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;

				return;
				break;
			}
			case BIT_1_R8:
			{
				cpu.setFlag(cpu.Zero, !(*r8& (1 << 1)));
				cpu.setFlag(cpu.Subtraction, false);
				cpu.setFlag(cpu.HalfCarry, true);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;

				return;
				break;
			}
			case BIT_2_R8:
			{
				cpu.setFlag(cpu.Zero, !(*r8& (1 << 2)));
				cpu.setFlag(cpu.Subtraction, false);
				cpu.setFlag(cpu.HalfCarry, true);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;

				return;
				break;
			}
			case BIT_3_R8:
			{
				cpu.setFlag(cpu.Zero, !(*r8& (1 << 3)));
				cpu.setFlag(cpu.Subtraction, false);
				cpu.setFlag(cpu.HalfCarry, true);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;

				return;
				break;
			}
			case BIT_4_R8:
			{
				cpu.setFlag(cpu.Zero, !(*r8& (1 << 4)));
				cpu.setFlag(cpu.Subtraction, false);
				cpu.setFlag(cpu.HalfCarry, true);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;

				return;
				break;
			}
			case BIT_5_R8:
			{
				cpu.setFlag(cpu.Zero, !(*r8& (1 << 5)));
				cpu.setFlag(cpu.Subtraction, false);
				cpu.setFlag(cpu.HalfCarry, true);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;

				return;
				break;
			}
			case BIT_6_R8:
			{
				cpu.setFlag(cpu.Zero, !(*r8& (1 << 6)));
				cpu.setFlag(cpu.Subtraction, false);
				cpu.setFlag(cpu.HalfCarry, true);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;

				return;
				break;
			}
			case BIT_7_R8:
			{
				cpu.setFlag(cpu.Zero, !(*r8& (1 << 7)));
				cpu.setFlag(cpu.Subtraction, false);
				cpu.setFlag(cpu.HalfCarry, true);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;

				return;
				break;
			}
			case RES_0_R8:
			{
				*r8 &= ~(1 << 0);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;

				return;
				break;
			}
			case RES_1_R8:
			{
				*r8 &= ~(1 << 1);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;

				return;
				break;
			}
			case RES_2_R8:
			{
				*r8 &= ~(1 << 2);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;
				return;
				break;
			}
			case RES_3_R8:
			{
				*r8 &= ~(1 << 3);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;

				return;
				break;
			}
			case RES_4_R8:
			{
				*r8 &= ~(1 << 4);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;
				return;
				break;
			}
			case RES_5_R8:
			{
				*r8 &= ~(1 << 5);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;

				return;
				break;
			}
			case RES_6_R8:
			{
				*r8 &= ~(1 << 6);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;
				return;
				break;
			}
			case RES_7_R8:
			{
				*r8 &= ~(1 << 7);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;
				return;
				break;
			}
			case SET_0_R8:
			{
				*r8 |= (1 << 0);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;
				return;
				break;
			}
			case SET_1_R8:
			{
				*r8 |= (1 << 1);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;
				return;
				break;
			}
			case SET_2_R8:
			{
				*r8 |= (1 << 2);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;
				return;
				break;
			}
			case SET_3_R8:
			{
				*r8 |= (1 << 3);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;
				return;
				break;
			}
			case SET_4_R8:
			{
				*r8 |= (1 << 4);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;
				return;
				break;
			}
			case SET_5_R8:
			{
				*r8 |= (1 << 5);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;
				return;
				break;
			}
			case SET_6_R8:
			{
				*r8 |= (1 << 6);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;
				return;
				break;
			}
			case SET_7_R8:
			{
				*r8 |= (1 << 7);

				cpu.cycles += 4;
				cpu.cyclesRan += 4;
				return;
				break;
			}
			default:
			{
				std::cout << "Unknown/Illegal MODE: " << "PREFIX CB: " << cpu.wasCB << " 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(opcode) << " found" << std::endl;
				cpu.setHasNotBroken(false);
				break;
			}
		}
	}
}

void CpuInstructions::RST40(Cpu& cpu)
{
	//std::cout << "ENTERING A VBLANK" << std::endl;
	cpu.halted = false;
	cpu.inVblank = true;
	cpu.ime = 0;

	cpu.write(--cpu.stkp, (cpu.pc >> 8) & 0xFF);
	cpu.write(--cpu.stkp, cpu.pc & 0xFF);

	cpu.pc = 0x0040;

	cpu.cycles += 12;
	cpu.cyclesRan += 12;

	return;
}

void CpuInstructions::RST60(Cpu& cpu)
{
	std::cout << "JOYPAD INPUT DETECTED" << std::endl;

	cpu.halted = false;
	cpu.ime = 0;

	cpu.write(--cpu.stkp, (cpu.pc >> 8) & 0xFF);
	cpu.write(--cpu.stkp, cpu.pc & 0xFF);

	cpu.pc = 0x0060;

	cpu.cycles += 12;
	cpu.cyclesRan += 12;

	return;
}
