#pragma once

#include <cstdint>
#include "Bus.h"
#include "CpuInstructions.h"
#include <string>
#include <deque>
#include "Engine.h"
#include <string>
#include "Gpu.h"
#include "MMU.h"
#include "JoyPadEnums.h"
#include <chrono>
#include "MiscRegs.h"

class CpuInstructions;

class Cpu
{
public:
	Cpu(CpuInstructions& CpuInstructions, MMU& mmu, Mbc& mbc, std::vector<uint8_t>& rom, Gpu& gpu);
	~Cpu();

    MMU& mmu;
    Mbc& mbc;
    Gpu& gpu;

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

	void loadRom();

	// Helper functions
	bool getHasNotBroken();
	uint16_t getPc();
    void printStatus();
	void setHasNotBroken(bool value);

    // Trace Deque
    std::deque<std::string> pcDeque;
    bool hasC505ran = false;
    //
    // Turn Step Mode On/Off
    bool stepMode = false;
    //
    // Checks if pc is in a CALL
    bool inCALL = false;
    //
    // Prints Out PC Trace
    void printTrace();

    // Cycles
    uint64_t cycles = 0;
    // Cycles Ran
    int cyclesRan = 0;

    // Program counter, Stack Pointer and Interrupts
    uint16_t pc;
    uint16_t stkp;
    bool halt;
    bool stopped;
    bool ime = false;

    // Stores whether or not the last instruction was 0xCB, used to index the CB Table instead
    bool wasCB = false;

    // Loads the Boot Rom
    void loadBootRom();

    // Handles the DMA Transfer into OAM
    void handleDMATransfer();

    uint8_t joypad_state;
    int joypad_cycles = 0;
    void update_joypad_memory();

    void key_press(JoyPadEnums key);
    void key_release(JoyPadEnums key);
    void check(int last_instr_cycles);


    void incrementDivReg();

    std::vector<std::vector<uint16_t>> getBackgroundTiles();
    std::vector<Sprite> getSprites();
    std::vector<std::vector<uint16_t>> getTileSet();
    uint8_t getSCY();
    uint8_t getSCX();
    bool getIs8x16Mode();

    bool frameReady = false;
    void setFrameReady(bool value);
    void clearGpuBackgroundTiles();

    std::vector<uint32_t> getFrameBuffer();
private:

    CpuInstructions& cpuInstructions;

	// Set to false when an error happens
	bool hasNotBroken = true;

	// Vector that stores the entire rom
	std::vector<uint8_t> rom;
    //
	// Stores the rom title
	std::string romTitle;
    //
    // True if we've loaded the rom into ram
    bool hasLoadedRom = false;

    // Tiles in Vram
    std::vector<std::vector<std::vector<uint8_t>>> vramTiles;

    // Background Tiles
    std::vector<std::vector<std::vector<uint8_t>>> backgroundTiles;

    // True if Nintendo Logo has been rendered else False
    bool hasNintendoLogoLoaded = false;

    // Holds changes to 0xFF01 (SB, Serial Transfer Data)
    uint8_t previousFF01 = 0x00;
    std::vector<uint8_t> FF01Changes;

    void handlePCTrace();
    void handleInterrupts();
    void handleDebugStepMode();
    
    // FOR JOYPAD
    uint8_t previousJoyPadState;

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

public:

    // True if CPU is halted
    bool halted = false;

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
public:
    // FOR PPU

    uint16_t lcdc; // Address of LCDC FF40
    uint16_t stat; // Address of LCD Status Register FF41
    uint16_t ly; // Address of the LY register FF44
    uint16_t vBlankIRQ;
    bool inVblank = false; // Is in vblank

};