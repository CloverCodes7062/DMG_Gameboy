#include "Mbc.h"
#include <iostream>

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
    if (addr >= 0x4000 && addr <= 0x7FFF)
    {
        int romBankOffset = (rom_bank == 0) ? 0x4000 : (rom_bank * 0x4000);
        return rom[static_cast<int>((addr & 0x3FFF) + romBankOffset)];
    }
    else if (addr >= 0xA000 && addr <= 0xBFFF)
    {
        if (ram_enabled)
        {
            int ramBankOffset = (ram_bank * 0x2000);
            return rom[static_cast<int>((addr & 0x3FFF) + ramBankOffset)];
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
        if (data == 0xA)
        {
            ram_enabled = true;
        }
        else
        {
            ram_enabled = false;
        }
    }
    else if (addr >= 0x2000 && addr <= 0x3FFF)
    {
        uint8_t bankNumber = (data & 0x1F);

        if (rom.size() == (256 * 1024))
        {
            bankNumber &= 0x0F;
        }
        rom_bank = (bankNumber == 0) ? 1 : bankNumber;
    }
    else if (addr >= 0x4000 && addr <= 0x5FFF)
    {

    }
    else if (addr >= 0x8000)
    {
        ram[addr] = data;
    }
}

uint8_t Mbc::mbc3_read_byte(uint16_t addr)
{
    if (addr >= 0x4000 && addr <= 0x7FFF)
    {
        int romBankOffset = (rom_bank == 0) ? 0x4000 : (rom_bank * 0x4000);
        return rom[static_cast<int>((addr & 0x3FFF) + romBankOffset)];
    }
    else if (addr >= 0xA000 && addr <= 0xBFFF)
    {

    }
    else
    {
        return ram[addr];
    }
}

void Mbc::mbc3_write_byte(uint16_t addr, uint8_t data)
{

}

void Mbc::setRomLoaded()
{
    hasRomLoaded = true;
}
