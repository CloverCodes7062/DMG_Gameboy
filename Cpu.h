#pragma once

#include <cstdint>
#include "Bus.h"
#include <string>

class Cpu
{
public:
	Cpu(Bus& bus);
	~Cpu();

	// write and read to ram stored on bus
	void write(uint16_t addr, uint8_t data);
	uint8_t read(uint16_t addr);

    // Enum for flags
    enum Flags {
        Zero = 7,
        Subtraction = 6,
        HalfCarry = 5,
        Carry = 4
    };

    // Set and Get Flag
    void setFlag(Cpu::Flags flag, bool value);
    bool isFlagSet(Cpu::Flags flag) const;


	// Run an instruction
	void runInstruction();
    // Runs the opcode
    void runOpcode();

	// Load entire rom into rom vector for access
	void loadRom(std::vector<uint8_t> romData);

	// Set Title
	void setTitle();

	// Helper functions
	bool getHasNotBroken();
	uint16_t getPc();
	void setHasNotBroken(bool value);
	std::string getTitle();

    uint64_t cycles = 0;

private:
	// Bus that has the ram on it
	Bus& bus;

	// Set to false when an error happens
	bool hasNotBroken = true;

	// Vector that stores the entire rom
	std::vector<uint8_t> rom;

	// Stores the rom title
	std::string romTitle;

private:

    // Program counter and Stack Pointer
    uint16_t pc;
    uint16_t stkp;

    // Struct to hold CPU registers
    struct Registers {
        union {
            struct {
                uint8_t f;
                uint8_t a;
            };
            uint16_t af;
        };
        union {
            struct {
                uint8_t c;
                uint8_t b;
            };
            uint16_t bc;
        };
        union {
            struct {
                uint8_t e;
                uint8_t d;
            };
            uint16_t de;
        };
        union {
            struct {
                uint8_t l;
                uint8_t h;
            };
            uint16_t hl;
        };
    } registers;
};