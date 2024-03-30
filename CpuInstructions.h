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

    // REGULAR TABLE
    void NOP(Cpu& cpu); void LDrrd16(Cpu& cpu); void LDarrR(Cpu& cpu); 
    void INCrr(Cpu& cpu); void INCr(Cpu& cpu); void DECr(Cpu& cpu); 
    void LDrd8(Cpu& cpu); void RLCA(Cpu& cpu); void LDa16SP(Cpu& cpu); 
    void ADDrrRR(Cpu& cpu); void DECrr(Cpu& cpu); void RRCA(Cpu& cpu); 
    void STOP0(Cpu& cpu); void RLA(Cpu& cpu); void JRr8(Cpu& cpu); 
    void RRA(Cpu& cpu); void DAA(Cpu& cpu); void CPL(Cpu& cpu);
    void INCaRR(Cpu& cpu); void DECaRR(Cpu& cpu); void SCF(Cpu& cpu);  
    void CCF(Cpu& cpu); void LDrR(Cpu& cpu); void HALT(Cpu& cpu); 
    void DECB(Cpu& cpu); void DECHL(Cpu& cpu); void ADDrR(Cpu& cpu);  
    void SUBr(Cpu& cpu); void ANDr(Cpu& cpu); void XORr(Cpu& cpu);
    void POPAF(Cpu& cpu); void PUSHAF(Cpu& cpu); void ORr(Cpu& cpu); 
    void CPr(Cpu& cpu); void LDraRR(Cpu& cpu); void JRcr8(Cpu& cpu);
    void INCHL(Cpu& cpu); void RETc(Cpu& cpu); void POPrr(Cpu& cpu);
    void JPca16(Cpu& cpu); void PUSHrr(Cpu& cpu);  void RSTn(Cpu& cpu); 
    void PREFIXCB(Cpu& cpu); void CALLca16(Cpu& cpu); void RETI(Cpu& cpu);
    void LDH(Cpu& cpu); void LD_C_A(Cpu& cpu); void ADDSPr8(Cpu& cpu); 
    void JPaHL(Cpu& cpu); void LDa16A(Cpu& cpu); void DI(Cpu& cpu); 
    void LDHLSPr8(Cpu& cpu); void LDSPHL(Cpu& cpu); void EI(Cpu& cpu);
    void JP16(Cpu& cpu);

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
};

