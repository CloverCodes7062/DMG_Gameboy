#pragma once
#include <cstdint>
#include "Engine.h"
#include <array>
#include "Sprite.h"

class Cpu;

class Gpu
{
public:
	Gpu();
	~Gpu();

	uint8_t update(uint16_t additionalCycles, uint8_t lyReg, bool cpuInVblank, uint8_t lcdcValue, uint8_t palette); // Updates our Gpu's state

	void updateTiles(uint8_t palette); // Updates all Tiles in Vram
	void setVblank(bool newVblank); // Sets Vblank

	void vramWrite(uint16_t addr, uint8_t data); // Writes to vram;

	// Vram
	std::vector<uint8_t> vram;

	// Entire Tileset In Vram
	std::vector<std::vector<uint16_t>> tileSet;

	// Background Tiles
	std::vector<std::vector<uint16_t>> backgroundTiles;

	void updateSprites(); // Updates Sprits with data from OAM

	std::vector<Sprite> Sprites;

	// Returns inVblank
	bool InVblank();

	bool frameReady = false;

private:
	bool gpuInVblank = false;
	int cyclesRan;
	uint16_t tileMapAddress;
	uint8_t prevLcdcValue = 0x91;

	// Total Frames Generated
	int totalFramesGenerated = 0;

	enum TileMapAddresses
	{
		FIRST_MAP = 0x9800,
		SECOND_MAP = 0x9C00,
	};

	uint16_t lastMapUsed = FIRST_MAP;

	bool signedMode = false;
};

