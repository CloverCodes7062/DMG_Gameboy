#pragma once

// Misc Regs Enum
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
    OBPO = 0xFF48,
    OBP1 = 0xFF49,
    LY = 0xFF44,
    LCDC = 0xFF40,
    STAT = 0xFF41,
};