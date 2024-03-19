#pragma once
#include <SDL.h>
#include <vector>

class Engine
{
public:
	Engine(int width, int height);
	~Engine();

	void setBuffer(std::vector<std::vector<std::vector<uint8_t>>>& tiles);
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

