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

uint8_t Gpu::update(uint16_t additionalCycles, uint8_t lyValue, bool cpuInVblank)
{
	cyclesRan += additionalCycles;

	switch (mode)
	{
		case 2:
		{
			if (lyValue == mmu->read(MiscRegs::WY))
			{
				windowLine = true;
			}

			if (cyclesRan >= 80)
			{
				//OAMScan(lyValue, lcdcValue);
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

				renderScanline(lyValue);
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
				renderFrame();
				windowLine = false;
				windowLineOffset = 0;
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

void Gpu::vramWrite(uint16_t addr, uint8_t data)
{
	vram[addr] = data;
}

void Gpu::renderScanline(uint8_t lyValue)
{

	uint8_t lcdcValue = mmu->read(MiscRegs::LCDC);
	uint8_t SCY = mmu->read(MiscRegs::SCY);
	uint8_t SCX = mmu->read(MiscRegs::SCX);
	uint8_t palette = mmu->read(MiscRegs::PALETTE);
	uint8_t WY = mmu->read(MiscRegs::WY);
	uint8_t WX = mmu->read(MiscRegs::WX);

	if (lyValue > 0x90)
	{
		return;
	}

	bool WXCondition = false;
	int x = 0;

	while (x < 160)
	{
		uint32_t tileAddress;
		if (WX - 7 <= x)
		{
			WXCondition = true;
		}

		if (windowLine && (lcdcValue & 0b00100000) && WXCondition)
		{
			uint16_t tMapArea = ((lcdcValue & 0b01000000 ? SECOND_MAP : FIRST_MAP) | (((windowLineOffset & 0xFF) >> 3) << 5) | (x >> 3));

			auto tMapAddress = tMapArea;
			
			uint8_t tileNumber = vram[tMapAddress];

			tileAddress = windowTileAddress(windowLineOffset, tileNumber);
		}
		else
		{
			auto tMapAddress = (tileMapAddress | ((((lyValue + SCY) & 0xFF) >> 3) << 5) | (((x + SCX) & 0xFF) >> 3));

			uint8_t tileNumber = vram[tMapAddress];

			tileAddress = backgroundTileAddress(lyValue, SCY, SCX, tileNumber);
		}

		int frameBufferIndex = lyValue * 160 + x;

		int startingJ = 0;
		if (x == 0 && !windowLine)
		{
			startingJ = (SCX % 8);
		}

		for (int j = startingJ; j < 8; j++)
		{
			if (x > 160)
			{
				continue;
			}

			uint8_t lsbBit = (vram[tileAddress] >> (7 - j)) & 0x01;
			uint8_t msbBit = (vram[tileAddress + 1] >> (7 - j)) & 0x01;

			uint8_t paletteIndex = (msbBit << 1) | lsbBit;

			uint8_t color = (palette >> (paletteIndex * 2)) & 0x03;

			if (frameBufferIndex < (160 * 144))
			{
				switch (color)
				{
				case 0:
					frameBuffer[frameBufferIndex++] = 0xFFE8FCCC; // White
					break;
				case 1:
					frameBuffer[frameBufferIndex++] = 0xFFACD490; // Light gray
					break;
				case 2:
					frameBuffer[frameBufferIndex++] = 0xFF548C70; // Dark gray
					break;
				case 3:
					frameBuffer[frameBufferIndex++] = 0xFF142C38; // Black
					break;
				default:
					std::cout << "UNKNOWN VALUE, USING BLACK AS DEFAULT" << std::endl;
					frameBuffer[frameBufferIndex++] = 0xFF000000; // Black
					break;
				}
			}
		}

		if ((SCX % 8) != 0 && x == 0 && !windowLine)
		{
			x += 8 - (SCX % 8);
		}
		else
		{
			x += 8;
		}
	}

	if (windowLine && (lcdcValue & 0b00100000))
	{
		windowLineOffset += 1;
	}
}

uint32_t Gpu::backgroundTileAddress(uint8_t lyValue, uint8_t SCY, uint8_t SCX, uint8_t tileNumber)
{
	uint32_t b12 = !signedMode ? 0 : ((tileNumber & 0x80) ^ 0x80) << 5;
	uint32_t ybits = (lyValue + SCY) & 7;

	return (0x8000 | b12 | (tileNumber << 4) | (ybits << 1));
}

uint32_t Gpu::windowTileAddress(uint8_t windowLineOffset, uint8_t tileNumber)
{
	uint32_t b12 = !signedMode ? 0 : ((tileNumber & 0x80) ^ 0x80) << 5;
	uint32_t hbits = 0;
	uint32_t ybits = (windowLineOffset & 7);

	return (0x8000 | b12 | (tileNumber << 4) | (ybits << 1));
}

void Gpu::renderFrame()
{
	uint8_t lcdcValue = mmu->read(MiscRegs::LCDC);
	uint8_t OBPO = mmu->read(MiscRegs::OBPO);
	uint8_t OBP1 = mmu->read(MiscRegs::OBP1);

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

		uint8_t palette;
		if (dmgPalette)
		{
			palette = OBP1;
		}
		else
		{
			palette = OBPO;
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

			uint16_t packedPixels = tiles[yFlip ? tileHeight - y - 1 : y];
			for (int x = 0; x < 8; x++) {
				int adjustedX = (sprite.x - 8) + (x);
				int adjustedY = (sprite.y - 16) + (y);

				if (adjustedX >= 0 && adjustedX < visibleWidth && adjustedY >= 0 && adjustedY < visibleHeight) {
					uint8_t pixelValue = (packedPixels >> (xFlip ? x * 2 : (7 - x) * 2)) & 0x03;

					int frameBufferIndex = adjustedY * visibleWidth + adjustedX;

					uint8_t color = ((palette >> (pixelValue * 2)) & 0x03);
					if (pixelValue == 0)
					{
						continue;
					}

					switch (color)
					{
					case 0:
						frameBuffer[frameBufferIndex] = 0xFFE8FCCC; // White;
						break;
					case 1:
						frameBuffer[frameBufferIndex] = 0xFFACD490; // Light gray
						break;
					case 2:
						frameBuffer[frameBufferIndex] = 0xFF548C70; // Dark gray
						break;
					case 3:
						frameBuffer[frameBufferIndex] = 0xFF142C38; // Black
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

void Gpu::OAMScan(uint8_t lyValue, uint8_t lcdcValue)
{
	objTileHeight = 8;

	if (lcdcValue & 0b00000100)
	{
		objTileHeight = 16;
	}

	OAMScanObjs.clear();

	for (uint16_t addr = 0xFE00; addr <= 0xFE9F; addr += 4)
	{
		if (vram[addr] <= lyValue && lyValue < (vram[addr] + objTileHeight) && OAMScanObjs.size() < 10)
		{
			OAMScanObjs.push_back(addr);
		}
	}
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