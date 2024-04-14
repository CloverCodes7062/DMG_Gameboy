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

class CpuInstructions;

class Cpu
{
public:
	Cpu(CpuInstructions& CpuInstructions, MMU& mmu, Mbc& mbc, std::vector<uint8_t>& rom);
	~Cpu();

    MMU& mmu;
    Mbc& mbc;

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

	// Set Title
	void setTitle();

    // GPU
    Gpu gpu = Gpu();

	// Helper functions
	bool getHasNotBroken();
	uint16_t getPc();
    void printVram();
    void printRombank();
    void printStatus();
	void setHasNotBroken(bool value);
    void writeStateToLog();
	std::string getTitle();

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
    bool ime;

    // Stores whether or not the last instruction was 0xCB, used to index the CB Table instead
    bool wasCB = false;

    // Loads the Boot Rom
    void loadBootRom();

    void loadRom(std::vector<uint8_t>& rom);

    // Handles the DMA Transfer into OAM
    void handleDMATransfer();


    enum MiscRegs
    {
        JOYPAD = 0xFF00,
        DIV = 0xFF04,
        SCY = 0xFF42,
        SCX = 0xFF43,
        PALETTE = 0xFF47,
        IE = 0xFFFF,
        IF = 0xFF0F,
        WY = 0xFF4A,
        WX = 0xFF4B,
    };

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

    std::vector<uint16_t> getTileRows();
    void clearTileRows();

    std::vector<uint32_t> getFrameBuffer();
    void clearFrameBuffer();
    void writeToJoyPad(uint8_t data);
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
    // Rom Type 32KB is ROM ONLY; 0x00 (NO MEMORY BANKING)
    uint8_t romType;
    // Sets Rom Type
    void setRomType();
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