#pragma once
#include <cstdint>
#include "Engine.h"
#include <array>

class Gpu
{
public:
	Gpu();
	~Gpu();

	uint8_t update(uint16_t additionalCycles, uint8_t lyReg, bool cpuInVblank); // Updates our Gpu's state
	void renderScanline(); // Renders a single scanline to the framebuffer
	void renderFrame(); // Renders single frame to the display window
	void updateTiles(); // Updates all Tiles in Vram
	std::vector<uint8_t> createTileRow(uint8_t lsb, uint8_t msb); // Creates a TileRow
	void setVblank(bool newVblank); // Sets Vblank
	void vramWrite(uint16_t addr, uint8_t data); // Writes to vram;
	void updateTile(uint16_t addr); // Updates a Single Tile

	// Vram
	std::vector<uint8_t> vram;

	// Tileset In Vram
	std::vector<std::vector<std::vector<uint8_t>>> tileSet;

	// Background Tiles
	std::vector<std::vector<std::vector<uint8_t>>> backgroundTiles;

	// OAM Data (Object Attribute Memory)
	std::vector<std::vector<std::vector<uint8_t>>> oam;

	// Returns InVblank
	bool InVblank();

private:
	bool gpuInVblank = false;
	int cyclesRan;
	uint16_t mode;
	uint16_t line;
	uint16_t yScroll;
	uint16_t xScroll;
	uint16_t tileMapAddress;

	// SDL Engine
	Engine engine = Engine(256 * 3, 256 * 3); // CHANGE SCALE IN ENGINE AS WELL

	// Total Frames Generated
	int totalFramesGenerated = 0;
};

