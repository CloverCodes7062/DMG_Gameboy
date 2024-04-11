#pragma once
#include <cstdint>
#include <vector>
#include <variant>

struct Sprite {
    uint8_t y;
    uint8_t x;
    std::vector<uint16_t> topTile;
    std::vector<uint16_t> bottomTile;
    uint8_t attributes;
};