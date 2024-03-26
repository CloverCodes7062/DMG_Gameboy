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

    // Sets Vram Tiles
    void setVramTiles();

	// Helper functions
	bool getHasNotBroken();
	uint16_t getPc();
    void printVram();
    void printStatus();
    void printSerialPorts();
	void setHasNotBroken(bool value);
	std::string getTitle();
    std::vector<uint8_t> createTileRow(uint8_t lsb, uint8_t msb);

    // Cycles
    uint64_t cycles = 0;
    // Cycles Ran
    int cyclesRan = 0;

    // SDL Engine
    //Engine engine = Engine(256, 256); UNUSED FOR NOW

    // INSTRUCTION LOOKUP TABLES
    struct INSTRUCTION
    {
        std::string name;
        void(Cpu::* operate)(void) = nullptr;
    };

    std::vector<INSTRUCTION> lookup;
    std::vector<INSTRUCTION> lookupCB;

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

    // Sets Step Mode
    bool stepMode = false;

    // Holds changes to 0xFF01 (SB, Serial Transfer Data)
    uint8_t previousFF01 = 0x00;
    std::vector<uint8_t> FF01Changes;

private:

    // Program counter, Stack Pointer, and Interrupts
    uint16_t pc;
    uint16_t stkp;
    uint8_t ie; // Interrupt Enable
    uint8_t ime; // Interrupe Master Flag

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

    void NOP(); void LDrrd16(); void LDarrR(); void INCrr(); void INCr(); void DECr(); void LDrd8(); void RLCA(); void LDa16SP(); void ADDrrRR(); void LDrARR(); void DECrr(); void RRCA();
    void STOP0(); void RLA(); void JRr8(); void RRA();
    void JRNZr8(); void LDarrINCR(); void DAA(); void JRZr8(); void LDaRRINC(); void CPL();
    void JRNCr8(); void LDarrDECR(); void INCaRR(); void DECaRR(); void LDarrD8(); void SCF(); void JRCr8(); void LDaRRDEC(); void CCF();
    void LDrR(); void HALT(); void DECB(); void DECHL();
    void ADDrR(); void ADDrARR(); void ADCrR(); void ADCrARR();
    void SUBr(); void SUBarr(); void SBCrR(); void SBCrARR();
    void ANDr(); void ANDarr(); void XORr(); void XORarr();
    void ORr(); void CPr(); void LDraRR(); void JRcr8();
    void RETc(); void POPrr(); void JPNZa16(); void JPca16(); void CALLNZa16(); void PUSHrr(); void ADDrd8(); void RSTn(); void RETZ(); void RET(); void JPZa16(); void PREFIXCB(); void CALLZa16(); void CALLa16(); void ACArd8(); void RST08();
    void RETNC(); void JPNCa16(); void CALLca16(); void SUBd8(); void RST10(); void RETC(); void RETI(); void JPCa16(); void CALLCa16(); void SBCrd8(); void RST18();
    void LDH(); void LD_C_A(); void LDaCr(); void ANDd8(); void RST20(); void ADDSPr8(); void JPaHL(); void LDa16A(); void XORd8(); void RST28();
    void LDHra8(); void LDraC(); void DI(); void ORd8(); void RST30(); void LDHLSPr8(); void LDSPHL(); void LDAa16(); void EI(); void CPd8(); void RST38();

    void RLCr8(); void RRCr8(); void RLr8(); void SLAr8(); void SRAr8();
    void SRLr8(); void RRr8(); void SWAPr8();

    void XXX();

private:
    // PPU

    uint16_t lcdc; // Address of LCDC FF40
    uint16_t stat; // Address of LCD Status Register FF41
    uint16_t ly; // Address of the LY register FF44

};