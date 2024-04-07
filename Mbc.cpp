#include "Mbc.h"
#include <iostream>

Mbc::Mbc(std::vector<uint8_t>* rom) : rom(rom) {

}

Mbc::Mbc(std::vector<uint8_t>* rom, std::vector<uint8_t>* ram, int rom_banks_count, int ram_banks_count)
    : rom(rom), ram(ram), rom_banks_count(rom_banks_count), ram_banks_count(ram_banks_count) 
{

}

uint8_t MBC1::read_byte(uint16_t address) 
{
    if (address < 0x4000) 
    {
        int bank = mode * (ram_bank << 5) % rom_banks_count;
        return (*rom)[bank * 0x4000 + address];
    }
    else if (address < 0x8000) 
    {
        int bank = ((ram_bank << 5) | rom_bank) % rom_banks_count;
        return (*rom)[bank * 0x4000 + address - 0x4000];
    }
    else if (address >= 0xA000 && address < 0xC000) 
    {
        if (ram_enabled) 
        {
            int bank = mode * ram_bank % ram_banks_count;
            return (*ram)[bank * 0x2000 + address - 0xA000];
        }
    }
    return 0xFF;
}

void MBC1::write_byte(uint16_t address, uint8_t value) 
{
    if (address < 0x2000) 
    {
        ram_enabled = (value & 0x0F) == 0x0A;
    }
    else if (address < 0x4000) 
    {
        value &= 0x1F;
        if (value == 0)
        {
            value = 1;
        }
        rom_bank = value;
    }
    else if (address < 0x6000) 
    {
        ram_bank = value & 0x3;
    }
    else if (address < 0x8000) 
    {
        mode = value & 0x1;
    }
    else if (address >= 0xA000 && address < 0xC000) 
    {
        if (ram_enabled) 
        {
            int bank = (ram_bank * mode) % ram_banks_count;
            (*ram)[bank * 0x2000 + address - 0xA000] = value;
        }
    }
}
