#pragma once
#include <cstdint>
#include <vector>
#include <chrono>

class Mbc
{
public:
    Mbc(std::vector<uint8_t>& passedRam, std::vector<uint8_t>& passedRom, int passedRomType, int passedRom_banks, int passedRam_banks);
    ~Mbc();

    std::vector<uint8_t>& rom;
    std::vector<uint8_t>& ram;
    std::vector<uint8_t> cartRam;

    int romType;
    int rom_banks = 0;
    int ram_banks = 0;
    int rom_bank = 1;
    int ram_bank = 0;

    bool hasRomLoaded = false;
    bool ram_enabled = false;
    bool mode = false;

    uint8_t read_byte(uint16_t addr);
    void write_byte(uint16_t addr, uint8_t value);

    void mbc0_write_byte(uint16_t addr, uint8_t data);
    void mbc1_write_byte(uint16_t addr, uint8_t data);
    void mbc3_write_byte(uint16_t addr, uint8_t data);

    uint8_t mbc0_read_byte(uint16_t addr);
    uint8_t mbc1_read_byte(uint16_t addr);
    uint8_t mbc3_read_byte(uint16_t addr);

    void setRomLoaded();
    void dumpCartRam();
    std::chrono::steady_clock::time_point lastDumpTime;
};

