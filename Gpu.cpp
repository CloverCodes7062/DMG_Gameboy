
#include "Gpu.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <conio.h>

Gpu::Gpu() : vram(0x10000, 0)
{
	cyclesRan = 0;
	line = 0;
	mode = 2;
	tileMapAddress = 0x9800;
}

Gpu::~Gpu()
{
}

uint8_t Gpu::update(uint16_t additionalCycles, uint8_t lyValue, bool cpuInVblank)
{
	cyclesRan += additionalCycles;

	/*
	switch (mode)
	{
		// OAM read mode, scanline active
	case 2:
	{
		if (cycles >= 80)
		{
			cycles = 0;
			mode = 3;
		}

		break;
	}
	// VRAM read mode, scanline active
	// Treat end of mode 3 as end of scanline
	case 3:
	{
		if (cycles >= 172)
		{
			cycles = 0;
			mode = 0;

			renderScanline();
		}

		break;
	}
	// Hblank
	// After the last hblank, push the screen data to canvas
	case 0:
	{
		if (cycles >= 204)
		{
			cycles = 0;
			line++;

			if (line == 143)
			{
				mode = 1;
				inVblank = true;
				renderFrame();
			}
			else
			{
				mode = 2;
			}
		}

		break;
	}
	// Vblank (10 lines)
	case 1:
	{
		if (cycles >= 456)
		{
			cycles = 0;
			line++;

			if (line > 153)
			{
				// Restart scanning modes
				mode = 2;
				line = 0;
				inVblank = false;
			}
		}

		break;
	}
	}
	*/


	
	/*
	if (cyclesRan >= 456)
	{
		write(ly, read(ly) + 1);
		cyclesRan = 0;

		if (read(ly) % 8 == 0)
		{
			setVramTiles();

			for (size_t addr = tileMapAddress; addr < tileMapAddress + 32; ++addr)
			{
				//std::cout << "VRAM TILES SIZE: " << vramTiles.size() << std::endl;
				//std::cout << "ADDR: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(addr) << std::endl;
				backgroundTiles.push_back(vramTiles[read(addr)]);
			}
			tileMapAddress += 32;

			//std::cout << "SCANLINE GENERATED BG TILES SIZE: 0x" << std::hex << std::setw(2) << std::setfill('0') << backgroundTiles.size() << std::endl;

			scanlinesGenerated++;

			//std::cout << "TILE MAP ADDRESS: " << tileMapAddress << std::endl;

		}

		if (read(ly) >= 0x99 && !inVblank)
		{

			//std::cout << "FRAME GENERATED BG TILES SIZE: 0x" << std::hex << std::setw(2) << std::setfill('0') << backgroundTiles.size() << std::endl;
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

			// SETS VBLANK IN INTERRUPT FLAG
			uint8_t IFValue = read(IF);
			IFValue |= (1 << 0);
			write(IF, IFValue);
		}
	}
	*/
		

	if (cyclesRan >= 456)
	{
		lyValue++;
		cyclesRan = 0;

		if (lyValue % 8 == 0 && backgroundTiles.size() < 576)
		{
			updateTiles();

			for (size_t addr = tileMapAddress; addr < tileMapAddress + 32; ++addr)
			{
				backgroundTiles.push_back(tileSet[vram[addr]]);
			}
			tileMapAddress += 32;

		}

		if (lyValue >= 0x99 && !cpuInVblank)
		{
			totalFramesGenerated++;

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

			gpuInVblank = true;
		}
	}

	return lyValue;
}

void Gpu::updateTiles()
{
	/*
	addr &= 0x1FFE;

	auto tile = (addr >> 4) & 511;
	auto y = (addr >> 1) & 7;

	uint8_t sx;

	for (size_t x = 0; x < 8; x++)
	{
		sx = 1 << (7 - x);

		tileSet[tile][y][x] =
			((vram[addr] & sx) ? 1 : 0) +
			((vram[addr + 1] & sx) ? 2 : 0);
	}
	*/

	std::vector<std::vector<std::vector<uint8_t>>> tiles;
	std::vector<std::vector<uint8_t>> tile;

	for (size_t i = 0x8000; i <= 0x8FFF; i += 2)
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

void Gpu::renderScanline()
{

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