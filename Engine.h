#pragma once
#include <SDL.h>
#include <vector>
#include "Sprite.h"

class Engine
{
public:
	Engine(int width, int height);
	~Engine();

	void setBuffer(std::vector<std::vector<uint16_t>> tileMap, std::vector<Sprite> Sprites, uint8_t SCY, uint8_t SCX); // RENDERS BG TILES AND SPRITES ON TOP
	void setBuffer(std::vector<std::vector<uint16_t>> tiles); // RENDERS ONLY BG TILES FOR VRAM VIEWER
	void render();

	int frameCount = 0;
	int lastFrameTime = 0;

private:
	int screenWidth;
	int screenHeight;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	std::vector<uint32_t> pixelBuffer;
	std::vector<uint32_t> pixelBufferA;
};

