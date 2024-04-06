#include "Gpu.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <conio.h>
#include <iomanip>
#include <cstdint>

Gpu::Gpu() : vram(0x10000, 0)
{
	cyclesRan = 0;
	line = 0;
	mode = 0;
	tileMapAddress = FIRST_MAP;

	updateTiles(0xE4);
}

Gpu::~Gpu()
{
}

uint8_t Gpu::update(uint16_t additionalCycles, uint8_t lyValue, bool cpuInVblank, uint8_t lcdcValue, uint8_t palette)
{
	cyclesRan += additionalCycles;

	if (cyclesRan >= 456)
	{
		lyValue++;
		cyclesRan = 0;

		if (lyValue % 8 == 0)
		{
			updateTiles(palette);
			for (size_t addr = tileMapAddress; addr < tileMapAddress + 32; ++addr)
			{
				if (!signedMode)
				{
					backgroundTiles.push_back(tileSet[vram[addr]]);
				}
				else
				{
					int offset = 256;

					int tileIndex = (static_cast<int8_t>(vram[addr])) + offset;

					backgroundTiles.push_back(tileSet[tileIndex]);
				}
			}
			tileMapAddress += 32;
		}

		if (lyValue >= 0x90 && !cpuInVblank)
		{
			totalFramesGenerated++;
			//std::cout << "TOTAL FRAMES GENERATED: " << totalFramesGenerated << std::endl;

			frameReady = true;

			if (prevLcdcValue != lcdcValue)
			{
				prevLcdcValue = lcdcValue;

				if (lcdcValue & 0x10)
				{
					std::cout << "SIGNED MODE OFF" << std::endl;
					signedMode = false;
				}
				else
				{
					std::cout << "SIGNED MODE ON" << std::endl;
					signedMode = true;
				}

				if (lcdcValue & 0x08)
				{
					std::cout << "USING SECOND TILE MAP" << std::endl;

					tileMapAddress = SECOND_MAP;
					lastMapUsed = SECOND_MAP;
				}
				else
				{
					std::cout << "USING FIRST TILE MAP" << std::endl;
					tileMapAddress = FIRST_MAP;
					lastMapUsed = FIRST_MAP;
				}
			}
			else
			{
				tileMapAddress = lastMapUsed;
			}
			gpuInVblank = true;
		}
	}

	return lyValue;
}

void Gpu::updateTiles(uint8_t palette)
{
	std::vector<std::vector<uint16_t>> tiles;
	std::vector<uint16_t> tile;

	for (size_t i = 0x8000; i <= 0x97FF; i += 2)
	{

		uint16_t tileRow = 0x0000;
		
		// CREATES A TILE ROW
		for (int j = 7; j >= 0; j--)
		{
			uint8_t lsbBit = (vram[i] >> j) & 0x01; // vram[i] is the least significant byte, extract bits from it
			uint8_t msbBit = (vram[i + 1] >> j) & 0x01; // vram[i + 1] is the most significant byte, extract bits from it

			uint8_t paletteIndex = (msbBit << 1) | lsbBit;

			uint8_t color3 = (palette >> 6) & 0x03;
			uint8_t color2 = (palette >> 4) & 0x03;
			uint8_t color1 = (palette >> 2) & 0x03;
			uint8_t color0 = palette & 0x03;

			uint8_t color;

			switch (paletteIndex)
			{
				case 0:
				{
					color = color0;
					break;
				}
				case 1:
				{
					color = color1;
					break;
				}
				case 2:
				{
					color = color2;
					break;
				}
				case 3:
				{
					color = color3;
					break;
				}
				default:
				{
					color = color3;
					std::cout << "INVALID PALETTE INDEX USING COLOR3 AS PLACEHOLDER" << std::endl;

					break;
				}
			}

			tileRow |= (color << (j * 2));
		}

		tile.push_back(tileRow);

		// ONCE 8 TILE ROWS HAVE BEEN PUSHED TO TILE
		// PUSH THE FINISHED TILE TO TILES AND THEN CLEAR TILE
		if (tile.size() == 8)
		{
			tiles.push_back(tile);
			tile.clear();
		}
	}

	tileSet = tiles;
}

void Gpu::vramWrite(uint16_t addr, uint8_t data)
{
	vram[addr] = data;

}

void Gpu::updateSprites()
{
	Sprites.clear();
	for (size_t addr = 0xFE00; addr <= 0xFE9F; addr += 4)
	{
		Sprites.push_back(Sprite{ vram[addr], vram[addr + 1], tileSet[vram[addr + 2]], vram[addr + 3] });
	}
}

/*
void Gpu::updateVramViewer()
{
	vramViewerEngine.setBuffer(tileSet);

	bool running = true;
	auto startTime = std::chrono::steady_clock::now();

	while (running) {

		auto currentTime = std::chrono::steady_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
		if (elapsedTime >= 1000 / 10000) {
			running = false;
		}

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
		}

		vramViewerEngine.render();
	}
}
*/

bool Gpu::InVblank()
{
	return gpuInVblank;
}

void Gpu::setVblank(bool newVblank)
{
	gpuInVblank = newVblank;
}