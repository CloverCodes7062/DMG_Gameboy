#pragma once
#include "Cpu.h"
#include <string>

class Cpu;

class CpuInstructions
{
public:
    CpuInstructions();
    ~CpuInstructions();

public:
    // INSTRUCTION LOOKUP TABLES
    struct INSTRUCTION
    {
        std::string name;
        void(CpuInstructions::* operate)(Cpu& cpu) = nullptr;
    };

    std::vector<INSTRUCTION> lookup;
    std::vector<INSTRUCTION> lookupCB;

    void runInstruction(Cpu& cpu);
    void runCBInstruction(Cpu& cpu);

    std::string getCBInstructionName(Cpu& cpu, uint16_t pc);
    std::string getInstructionName(Cpu& cpu, uint16_t pc);

    // INTERRUPTS
    void RST40(Cpu& cpu);

    // CB TABLE
    void RLCr8(Cpu& cpu); void RRCr8(Cpu& cpu); void RLr8(Cpu& cpu);
    void SLAr8(Cpu& cpu); void SRAr8(Cpu& cpu); void SRLr8(Cpu& cpu);
    void RRr8(Cpu& cpu); void SWAPr8(Cpu& cpu); void RESu3r8(Cpu& cpu);
    void BIT0r8(Cpu& cpu); void BIT1r8(Cpu& cpu); void BIT2r8(Cpu& cpu); 
    void BIT3r8(Cpu& cpu); void BIT4r8(Cpu& cpu); void BIT5r8(Cpu& cpu);
    void BIT6r8(Cpu& cpu); void BIT7r8(Cpu& cpu); void RES0r8(Cpu& cpu);
    void RES1r8(Cpu& cpu); void RES2r8(Cpu& cpu); void RES3r8(Cpu& cpu);
    void RES4r8(Cpu& cpu); void RES5r8(Cpu& cpu); void RES6r8(Cpu& cpu);
    void RES7r8(Cpu& cpu); void SET0r8(Cpu& cpu); void SET1r8(Cpu& cpu);
    void SET2r8(Cpu& cpu); void SET3r8(Cpu& cpu); void SET4r8(Cpu& cpu);
    void SET5r8(Cpu& cpu); void SET6r8(Cpu& cpu); void SET7r8(Cpu& cpu);

    // ILLEGAL/UNIMPLEMENTED INSTRUCTION
    void XXX(Cpu& cpu);

    // INSTRUCTION MODE ENUM
    enum InstructionMode
    {
        INVALID = 0,
        LD_RR_D16 = 1,
        INR_R = 2,
        LD_R_D8 = 3,
        ADD_RR_RR = 4,
        ADD_R_R = 5,
        SUB_R_R = 6,
        AND_R = 7,
        XOR_R = 8,
        OR_R = 9,
        CP_R = 10,
        RST_N = 11,
    };

    enum CBInstructionMode
    {
        ILLEGAL = 0,
        RLC_R8 = 1,
        RRC_R8 = 2,
        RL_R8 = 3,
        RR_R8 = 4,
        SLA_R8 = 5,
        SRA_R8 = 6,
        SWAP_R8 = 7,
        SRL_R8 = 8,
        BIT_0_R8 = 9,
        BIT_1_R8 = 10,
        BIT_2_R8 = 11,
        BIT_3_R8 = 12,
        BIT_4_R8 = 13,
        BIT_5_R8 = 14,
        BIT_6_R8 = 15,
        BIT_7_R8 = 16,
        RES_0_R8 = 17,
        RES_1_R8 = 18,
        RES_2_R8 = 19,
        RES_3_R8 = 20,
        RES_4_R8 = 21,
        RES_5_R8 = 22,
        RES_6_R8 = 23,
        RES_7_R8 = 24,
        SET_0_R8 = 25,
        SET_1_R8 = 26,
        SET_2_R8 = 27,
        SET_3_R8 = 28,
        SET_4_R8 = 29,
        SET_5_R8 = 30,
        SET_6_R8 = 31,
        SET_7_R8 = 32,
    };
};

