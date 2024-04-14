#include "Mbc.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <fstream>

Mbc::Mbc(std::vector<uint8_t>& passedRam, std::vector<uint8_t>& passedRom, int passedRomType, int passedRom_banks, int passedRam_banks)
    : ram(passedRam), rom(passedRom), romType(passedRomType), rom_banks(passedRom_banks), ram_banks(passedRam_banks)
{

}

Mbc::~Mbc()
{
}

uint8_t Mbc::read_byte(uint16_t addr)
{
    if (!hasRomLoaded)
    {
        return ram[addr];
    }

    switch (romType)
    {
        case 0x00:
            return mbc0_read_byte(addr);
        case 0x01:
            return mbc1_read_byte(addr);
        case 0x03:
            return mbc1_read_byte(addr);
        case 0x13:
            return mbc3_read_byte(addr);
        default:
            std::cout << "UNKNOWN ROM TYPE: " << std::dec << romType << std::endl;
            break;
    }
}

void Mbc::write_byte(uint16_t addr, uint8_t data)
{
    if (!hasRomLoaded)
    {
        ram[addr] = data;
        return;
    }

    switch (romType)
    {
        case 0x00:
            mbc0_write_byte(addr, data);
            break;
        case 0x01:
            mbc1_write_byte(addr, data);
            break;
        case 0x03:
            mbc1_write_byte(addr, data);
            break;
        case 0x13:
            mbc3_write_byte(addr, data);
            break;
        default:
            std::cout << "INVALID ROM TYPE" << std::endl;
            break;
    }
}


uint8_t Mbc::mbc0_read_byte(uint16_t addr)
{
    return ram[addr];
}

void Mbc::mbc0_write_byte(uint16_t addr, uint8_t data)
{
    if (addr <= 0x8000)
    {
        return;
    }
    else
    {
        ram[addr] = data;
    }
}

uint8_t Mbc::mbc1_read_byte(uint16_t addr)
{
    if (addr < 0x4000)
    {
        int bank = mode * (ram_bank << 5) % rom_banks;
        return rom[bank * 0x4000 + addr];
    }
    if (addr >= 0x4000 && addr <= 0x7FFF)
    {
        int bank = ((ram_bank << 5) | rom_bank) % rom_banks;
        return rom[bank * 0x4000 + addr - 0x4000];
    }
    else if (addr >= 0xA000 && addr < 0xC000)
    {
        if (ram_enabled)
        {
            int bank = mode * ram_bank % ram_banks;
            return ram[bank * 0x2000 + addr - 0xA000];
        }
        else
        {
            return 0xFF;
        }
    }
    else
    {
        return ram[addr];
    }
}

void Mbc::mbc1_write_byte(uint16_t addr, uint8_t data)
{
    if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        ram_enabled = (data & 0x0F) == 0x0A;
    }
    else if (addr >= 0x2000 && addr <= 0x3FFF)
    {
        data &= 0x1F;

        if (data == 0)
        {
            data = 1;
        }
        rom_bank = data;
    }
    else if (addr >= 0x4000 && addr <= 0x5FFF)
    {
        ram_bank = data & 0x3;
    }
    else if (addr >= 0x6000 && addr <= 0x7FFF)
    {
        mode = data & 0x01;
    }
    else if (addr >= 0xA000 && addr < 0xC000)
    {
        if (ram_enabled)
        {
            int bank = (ram_bank * mode) % ram_banks;
            ram[bank * 0x2000 + addr - 0xA000] = data;
        }
    }
    else if (addr >= 0x8000)
    {
        ram[addr] = data;
    }
}

uint8_t Mbc::mbc3_read_byte(uint16_t addr)
{
    if (addr < 0x4000)
    {
        return rom[addr];
    }
    else if (addr < 0x8000)
    {
        return rom[rom_bank * 0x4000 + addr - 0x4000];
    }
    else if (addr >= 0xA000 && addr < 0xC000)
    {
        if (ram_enabled)
        {
            if (ram_bank <= 0x03)
            {
                return ram[ram_bank * 0x2000 + addr - 0xA000];
            }
        }
    }
    else
    {
        return ram[addr];
    }

    return 0;
}

void Mbc::mbc3_write_byte(uint16_t addr, uint8_t data)
{
    if (addr < 0x2000)
    {
        ram_enabled = (data & 0x0F) == 0x0A;
    }
    else if (addr < 0x4000)
    {
        rom_bank = data & 0x7F;
        if (rom_bank == 0x00)
        {
            rom_bank = 0x01;
        }
    }
    else if (addr < 0x6000)
    {
        ram_bank = data & 0x0F;
    }
    else if (addr >= 0xA000 && addr < 0xC000)
    {
        if (ram_enabled)
        {
            ram[ram_bank * 0x2000 + addr - 0xA000] = data;
        }
    }
    else
    {
        ram[addr] = data;
    }

}

void Mbc::setRomLoaded()
{
    hasRomLoaded = true;
}
