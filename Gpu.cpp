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
	for (int x = 0; x < 160; x += 8)
	{
		auto tMapAddress = (0x9800 | ((tileMapAddress == FIRST_MAP ? 0 : 1) << 10) | ((((lyValue + SCY) & 0xFF) >> 3) << 5) | (((x) & 0xFF) >> 3));
		
		uint8_t tileNumber = vram[tMapAddress];

		uint32_t tileAddress = backgroundTileAddress(lyValue, SCY, tileNumber);

		uint16_t tileRow = 0x0000;
		// CREATES A TILE ROW
		for (int j = 7; j >= 0; j--)
		{
			uint8_t lsbBit = (vram[tileAddress] >> j) & 0x01;
			uint8_t msbBit = (vram[tileAddress + 1] >> j) & 0x01;

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


			switch (color)
			{
				case 0:
					frameBuffer[lyValue * 160 + (x + j)] = 0xFFFFFFFF; // White
					break;
				case 1:
					frameBuffer[lyValue * 160 + (x + j)] = 0xFFC0C0C0; // Light gray
					break;
				case 2:
					frameBuffer[lyValue * 160 + (x + j)] = 0xFF808080; // Dark gray
					break;
				case 3:
					frameBuffer[lyValue * 160 + (x + j)] = 0xFF000000; // Black
					break;
				default:
					std::cout << "UNKNOWN VALUE, USING BLACK AS DEFAULT" << std::endl;
					frameBuffer[lyValue * 160 + (x + j)] = 0xFF000000; // Black
					break;
			}
		}

		tileRows.push_back(tileRow);
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

bool Gpu::InVblank()
{
	return gpuInVblank;
}

void Gpu::setVblank(bool newVblank)
{
	gpuInVblank = newVblank;
}