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
	void renderScanline(); // Renders a single scanline to the framebuffer
	void renderFrame(); // Renders single frame to the display window
	void updateTiles(uint8_t palette); // Updates all Tiles in Vram
	uint16_t createTileRow(uint8_t lsb, uint8_t msb); // Creates a TileRow
	void setVblank(bool newVblank); // Sets Vblank
	void vramWrite(uint16_t addr, uint8_t data); // Writes to vram;
	void updateTile(uint16_t addr); // Updates a Single Tile

	// Vram
	std::vector<uint8_t> vram;

	// Entire Tileset In Vram
	std::vector<std::vector<uint16_t>> tileSet;

	// Background Tiles
	std::vector<std::vector<uint16_t>> backgroundTiles;

	void updateSprites(); // Updates Sprits with data from OAM

	// DEBUG VRAM VIEWER ENGINE
	//Engine vramViewerEngine = Engine(256 * 3, 256 * 3);

	void updateVramViewer();

	std::vector<Sprite> Sprites;

	// Returns inVblank
	bool InVblank();

	bool frameReady = false;

private:
	bool gpuInVblank = false;
	int cyclesRan;
	uint16_t mode;
	uint16_t line;
	uint16_t yScroll;
	uint16_t xScroll;
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

