#pragma once
#include <cstdint>
#include <vector>

class Mbc
{
public:
    std::vector<uint8_t>* rom;
    std::vector<uint8_t>* ram;
    int rom_banks_count = 1;
    int ram_banks_count = 1;

    Mbc(std::vector<uint8_t>* rom);
    Mbc(std::vector<uint8_t>* rom, std::vector<uint8_t>* ram, int rom_banks_count, int ram_banks_count);

    virtual uint8_t read_byte(uint16_t address) = 0;
    virtual void write_byte(uint16_t address, uint8_t value) = 0;
};

class MBC1 : public Mbc {
public:
    bool ram_enabled = false;
    bool mode = false;
    uint8_t rom_bank = 1;
    uint8_t ram_bank = 0;

    using Mbc::Mbc;
    uint8_t read_byte(uint16_t address);
    void write_byte(uint16_t address, uint8_t value);
};

