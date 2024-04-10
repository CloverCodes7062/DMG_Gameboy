#pragma once
#include <cstdint>
#include "Engine.h"
#include <array>
#include "Sprite.h"
#include <queue>

class Cpu;

class Gpu
{
public:
	Gpu();
	~Gpu();

	uint8_t update(uint16_t additionalCycles, uint8_t lyReg, bool cpuInVblank, uint8_t lcdcValue, uint8_t palette, uint8_t SCY, uint8_t SCX); // Updates our Gpu's state

	void setVblank(bool newVblank); // Sets Vblank

	void vramWrite(uint16_t addr, uint8_t data, uint8_t palette); // Writes to vram;

	// Vram
	std::vector<uint8_t> vram;

	// Entire Tileset In Vram
	std::vector<std::vector<uint16_t>> tileSet;

	// Background Tiles
	std::vector<std::vector<uint16_t>> backgroundTiles;

	void updateSprites(uint8_t lcdcValue); // Updates Sprits with data from OAM

	std::vector<Sprite> Sprites;

	// Returns inVblank
	bool InVblank();

	bool frameReady = false;

	bool is8x16Mode = false;
	int mode;
	bool objectsEnabled = false;
	std::vector<uint32_t> frameBuffer;

	void renderScanline(uint8_t lyValue, uint8_t lcdcValue, uint8_t SCY, uint8_t SCX, uint8_t palette);
	void renderFrame(uint8_t lcdcValue);
	uint32_t backgroundTileAddress(uint8_t lyValue, uint8_t SCY, uint8_t tileNumber);
	std::queue<int> OAMQueue;

private:
	bool gpuInVblank = false;
	int cyclesRan;
	uint16_t tileMapAddress = FIRST_MAP;
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

