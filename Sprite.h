#pragma once
#include <cstdint>
#include <vector>
#include <variant>

struct Sprite {
    uint8_t y;
    uint8_t x;
    std::variant<std::vector<uint16_t>, std::pair<std::vector<uint16_t>, std::vector<uint16_t>>> tiles;
    uint8_t attributes;
};