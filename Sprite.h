#pragma once
#include <cstdint>
#include <vector>

struct Sprite {
    uint8_t y;
    uint8_t x;
    std::vector<std::vector<uint8_t>> tile;
    uint8_t attributes;
};