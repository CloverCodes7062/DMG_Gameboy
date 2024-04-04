#pragma once
#include <SDL.h>
#include <vector>
#include "Sprite.h"

class Engine
{
public:
	Engine(int width, int height);
	~Engine();

	void setBuffer(std::vector<std::vector<std::vector<uint8_t>>>& tiles, std::vector<Sprite> Sprites); // RENDERS BG TILES AND SPRITES ON TOP
	void setBuffer(std::vector<std::vector<std::vector<uint8_t>>>& tiles); // RENDERS ONLY BG TILES FOR VRAM VIEWER
	void renderTile(int x, int y, std::vector<uint8_t>& tile);
	void render();

private:
	int screenWidth;
	int screenHeight;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	std::vector<uint32_t> pixelBuffer;
};

