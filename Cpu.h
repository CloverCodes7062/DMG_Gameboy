#pragma once

#include <cstdint>
#include "Bus.h"
#include <string>
#include "Engine.h"

class Cpu
{
public:
	Cpu(Bus& bus);
	~Cpu();

	// Write and read to ram stored on bus
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

    // Loads Nintendo Logo into VRAM
    void loadNintendoLogo();

	// Helper functions
	bool getHasNotBroken();
	uint16_t getPc();
	void setHasNotBroken(bool value);
	std::string getTitle();
    std::vector<uint8_t> createTileRow(uint8_t lsb, uint8_t msb);

    // Cycles
    uint64_t cycles = 0;
    // Cycles Ran
    int cyclesRan = 0;

    // SDL Engine
    Engine engine = Engine(256, 256);

private:
	// Bus that has the ram on it
	Bus& bus;

	// Set to false when an error happens
	bool hasNotBroken = true;

	// Vector that stores the entire rom
	std::vector<uint8_t> rom;

	// Stores the rom title
	std::string romTitle;

    // stores whether or not the last instruction was 0xCB, used to index the CB Table instead
    bool wasCB = false;

    // Tiles in Vram
    std::vector<std::vector<std::vector<uint8_t>>> vramTiles;

    // Background Tiles
    std::vector<std::vector<std::vector<uint8_t>>> backgroundTiles;

    // True if Nintendo Logo has been rendered else False
    bool hasNintendoLogoLoaded = false;

private:

    // Program counter, Stack Pointer, and Interrupt
    uint16_t pc;
    uint16_t stkp;
    uint8_t ie;

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

    // Tile Map Address
    uint16_t tileMapAddress;

    // Debug Helper Variable
    int scanlinesGenerated = 0;

    // Debug totalFramesGenerated
    int totalFramesGenerated = 0;

private:
    // PPU

    uint16_t lcdc; // Address of LCDC FF40
    uint16_t stat; // Address of LCD Status Register FF41
    uint16_t ly; // Address of the LY register FF44
};