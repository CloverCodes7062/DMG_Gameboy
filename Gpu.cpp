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

	updateTiles();
}

Gpu::~Gpu()
{
}

uint8_t Gpu::update(uint16_t additionalCycles, uint8_t lyValue, bool cpuInVblank, uint8_t lcdcValue)
{
	cyclesRan += additionalCycles;

	if (cyclesRan >= 456)
	{
		lyValue++;
		cyclesRan = 0;

		if (lyValue % 8 == 0 && backgroundTiles.size() < 576)
		{
			updateTiles();
			updateVramViewer();

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

		if (lyValue >= 0x99 && !cpuInVblank)
		{
			totalFramesGenerated++;
			//std::cout << "TOTAL FRAMES GENERATED: " << totalFramesGenerated << std::endl;

			engine.setBuffer(backgroundTiles, Sprites);

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

				engine.render();
			}

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
			backgroundTiles.clear();

			gpuInVblank = true;
		}
	}

	return lyValue;
}

void Gpu::updateTiles()
{
	std::vector<std::vector<std::vector<uint8_t>>> tiles;
	std::vector<std::vector<uint8_t>> tile;

	for (size_t i = 0x8000; i <= 0x97FF; i += 2)
	{
		std::vector<uint8_t> tileRow = createTileRow(vram[i], vram[i + 1]);

		tile.push_back(tileRow);

		if (tile.size() == 8)
		{
			tiles.push_back(tile);
			tile.clear();
		}
	}

	tileSet = tiles;
}

std::vector<uint8_t> Gpu::createTileRow(uint8_t lsb, uint8_t msb)
{
	std::vector<uint8_t> tileRow;

	for (int i = 7; i >= 0; i--)
	{
		uint8_t lsbBit = (lsb >> i) & 0x01;
		uint8_t msbBit = (msb >> i) & 0x01;

		uint8_t color = (msbBit << 1) | lsbBit;

		tileRow.push_back(color);
	}

	return tileRow;
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

void Gpu::renderFrame()
{
	totalFramesGenerated++;

	//std::cout << "VRAM TILES SIZE: " << vramTiles.size() << std::endl;

	std::cout << "TOTAL FRAMES GENERATED: " << totalFramesGenerated << std::endl;


	engine.setBuffer(backgroundTiles);

	bool running = true;
	auto startTime = std::chrono::steady_clock::now();

	while (running) {

		auto currentTime = std::chrono::steady_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
		if (elapsedTime >= 1000 / 60) {
			running = false;
		}

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
		}

		engine.render();
	}

	tileMapAddress = 0x9800;
	backgroundTiles.clear();
}

bool Gpu::InVblank()
{
	return gpuInVblank;
}

void Gpu::setVblank(bool newVblank)
{
	gpuInVblank = newVblank;
}