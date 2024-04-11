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
	mode = 2;
	frameBuffer.resize(160 * 144);
}

Gpu::~Gpu()
{
}

uint8_t Gpu::update(uint16_t additionalCycles, uint8_t lyValue, bool cpuInVblank, uint8_t lcdcValue, uint8_t palette, uint8_t SCY, uint8_t SCX)
{
	cyclesRan += additionalCycles;

	switch (mode)
	{
		case 2:
		{
			if (cyclesRan >= 80)
			{
				cyclesRan = 0;
				mode = 3;
			}

			break;
		}
		case 3:
		{
			if (cyclesRan >= 172)
			{
				cyclesRan = 0;
				mode = 0;

				renderScanline(lyValue, lcdcValue, SCY, SCX, palette);
			}

			break;
		}
		case 0:
		{
			cyclesRan = 0;
			lyValue++;

			if (lyValue == 144 && !cpuInVblank)
			{
				mode = 1;
				renderFrame(lcdcValue);
			}
			else
			{
				mode = 2;
			}

			break;
		}
		case 1:
		{
			if (cyclesRan >= 456)
			{
				cyclesRan = 0;
				lyValue++;

				if (lyValue > 153)
				{
					mode = 2;
					lyValue = 0;
				}
			}

			break;
		}
	}

	return lyValue;
}

void Gpu::vramWrite(uint16_t addr, uint8_t data, uint8_t palette)
{
	vram[addr] = data;
}

void Gpu::renderScanline(uint8_t lyValue, uint8_t lcdcValue, uint8_t SCY, uint8_t SCX, uint8_t palette)
{
	//std::cout << "SCANLINE SCX: " << std::dec << SCX << std::endl;
	for (int x = 0; x < 160; x += 8)
	{
		auto tMapAddress = (0x9800 | ((tileMapAddress == FIRST_MAP ? 0 : 1) << 10) | ((((lyValue + SCY) & 0xFF) >> 3) << 5) | (((x + SCX) & 0xFF) >> 3));
		
		uint8_t tileNumber = vram[tMapAddress];

		uint32_t tileAddress = backgroundTileAddress(lyValue, SCY, tileNumber);

		int frameBufferIndex = lyValue * 160 + x;

		for (int j = 0; j < 8; j++)
		{
			uint8_t lsbBit = (vram[tileAddress] >> (7 - j)) & 0x01;
			uint8_t msbBit = (vram[tileAddress + 1] >> (7 - j)) & 0x01;

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

			switch (color)
			{
				case 0:
					frameBuffer[frameBufferIndex++] = 0xFFFFFFFF; // White
					break;
				case 1:
					frameBuffer[frameBufferIndex++] = 0xFFC0C0C0; // Light gray
					break;
				case 2:
					frameBuffer[frameBufferIndex++] = 0xFF808080; // Dark gray
					break;
				case 3:
					frameBuffer[frameBufferIndex++] = 0xFF000000; // Black
					break;
				default:
					std::cout << "UNKNOWN VALUE, USING BLACK AS DEFAULT" << std::endl;
					frameBuffer[frameBufferIndex++] = 0xFF000000; // Black
					break;
			}
		}
	}
}

uint32_t Gpu::backgroundTileAddress(uint8_t lyValue, uint8_t SCY, uint8_t tileNumber)
{
	uint32_t b12 = !signedMode ? 0 : ((tileNumber & 0x80) ^ 0x80) << 5;
	uint32_t hbits = 0;
	uint32_t ybits = (lyValue + SCY) & 7;

	return (0x8000 | b12 | (tileNumber << 4) | (ybits << 1)) + hbits;
}

void Gpu::renderFrame(uint8_t lcdcValue)
{
	totalFramesGenerated++;

	frameReady = true;

	int visibleWidth = 160;
	int visibleHeight = 144;

	for (const auto& sprite : Sprites) {
		uint8_t priority = sprite.attributes & 0x80;
		uint8_t yFlip = sprite.attributes & 0x40;
		uint8_t xFlip = sprite.attributes & 0x20;
		uint8_t dmgPalette = sprite.attributes & 0x10;

		int tileHeight = 8;
		if (is8x16Mode) {
			tileHeight = 16;
		}

		std::vector<uint16_t> tiles;

		if (is8x16Mode)
		{
			tiles.insert(tiles.end(), sprite.topTile.begin(), sprite.topTile.end());
			tiles.insert(tiles.end(), sprite.bottomTile.begin(), sprite.bottomTile.end());

			//std::cout << "TILES SIZE: " << std::dec << tiles.size() << std::endl;
		}
		else
		{
			tiles = sprite.topTile;
		}

		for (int y = 0; y < tileHeight; y++) {

			uint16_t packedPixels = tiles[yFlip ? (tileHeight - 1 - y) : y];
			for (int x = 0; x < 8; x++) {
				int adjustedX = (sprite.x - 8) + (xFlip ? (7 - x) : x);
				int adjustedY = (sprite.y - 16) + (yFlip ? (tileHeight - 1 - y) : y);

				if (adjustedX >= 0 && adjustedX < visibleWidth && adjustedY >= 0 && adjustedY < visibleHeight) {
					uint8_t pixelValue = (packedPixels >> ((xFlip ? x : 7 - x) * 2)) & 0x03;

					int frameBufferIndex = adjustedY * visibleWidth + adjustedX;

					if (pixelValue != 0) // Don't render 0 for spirtes, its transparent
					{
						switch (pixelValue)
						{
						case 0:
							frameBuffer[frameBufferIndex] = 0xFFFFFFFF; // White
							break;
						case 1:
							frameBuffer[frameBufferIndex] = 0xFFC0C0C0; // Light gray
							break;
						case 2:
							frameBuffer[frameBufferIndex] = 0xFF808080; // Dark gray
							break;
						case 3:
							frameBuffer[frameBufferIndex] = 0xFF000000; // Black
							break;
						default:
							std::cout << "UNKNOWN VALUE, USING BLACK AS DEFAULT" << std::endl;
							frameBuffer[frameBufferIndex] = 0xFF000000; // Black
							break;
						}
					}
				}
			}
		}
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
	gpuInVblank = true;
}

void Gpu::updateSprites(uint8_t lcdcValue)
{
	Sprites.clear();

	for (size_t addr = 0xFE00; addr <= 0xFE9F; addr += 4)
	{
		uint8_t y = vram[addr];
		uint8_t x = vram[addr + 1];
		uint8_t tileIndex = vram[addr + 2];
		uint8_t attributes = vram[addr + 3];

		if (is8x16Mode)
		{
			tileIndex &= 0xFE;

			std::vector<uint16_t> topTile;
			std::vector<uint16_t> bottomTile;

			for (int i = 0x8000 + (tileIndex * 16); i < 0x8000 + (tileIndex * 16) + 16; i += 2)
			{
				uint16_t tileRow = 0x0000;
				for (int j = 0; j < 8; j++)
				{
					uint8_t lsbBit = (vram[i] >> (7 - j)) & 0x01;
					uint8_t msbBit = (vram[i + 1] >> (7 - j)) & 0x01;

					uint8_t paletteIndex = (msbBit << 1) | lsbBit;
					//std::cout << "PALETTE INDEX: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(paletteIndex) << std::endl;
					tileRow |= (paletteIndex << (14 - j * 2));
				}

				//std::cout << "TILE ROW: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(tileRow) << std::endl;
				topTile.push_back(tileRow);
			}

			for (int i = 0x8000 + ((tileIndex | 0x01) * 16); i < 0x8000 + ((tileIndex | 0x01) * 16) + 16; i += 2)
			{
				uint16_t tileRow = 0x0000;
				for (int j = 0; j < 8; j++)
				{
					uint8_t lsbBit = (vram[i] >> (7 - j)) & 0x01;
					uint8_t msbBit = (vram[i + 1] >> (7 - j)) & 0x01;

					uint8_t paletteIndex = (msbBit << 1) | lsbBit;
					//std::cout << "PALETTE INDEX: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(paletteIndex) << std::endl;
					tileRow |= (paletteIndex << (14 - j * 2));
					//std::cout << "TILE ROW: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(tileRow) << std::endl;
				}

				bottomTile.push_back(tileRow);
			}

			Sprites.push_back(Sprite{ y, x, topTile, bottomTile, attributes });
		}
		else
		{
			std::vector<uint16_t> topTile;

			for (int i = 0x8000 + (tileIndex * 16); i < 0x8000 + (tileIndex * 16) + 16; i += 2)
			{
				uint16_t tileRow = 0x0000;
				for (int j = 0; j < 8; j++)
				{
					uint8_t lsbBit = (vram[i] >> (7 - j)) & 0x01;
					uint8_t msbBit = (vram[i + 1] >> (7 - j)) & 0x01;

					uint8_t paletteIndex = (msbBit << 1) | lsbBit;
					//std::cout << "PALETTE INDEX: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(paletteIndex) << std::endl;
					tileRow |= (paletteIndex << (14 - j * 2));
				}
				//std::cout << "TILE ROW: 0x" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(tileRow) << std::endl;
				topTile.push_back(tileRow);
			}

			Sprites.push_back(Sprite{ y, x, topTile, topTile, attributes });
		}
	}
}

bool Gpu::InVblank()
{
	return gpuInVblank;
}

void Gpu::setVblank(bool newVblank)
{
	gpuInVblank = newVblank;
}