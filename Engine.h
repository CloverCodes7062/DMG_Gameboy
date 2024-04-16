#pragma once
#include <SDL.h>
#include <vector>
#include "Sprite.h"

class Engine
{
public:
	Engine(int width, int height);
	~Engine();

	void setBuffer(std::vector<uint32_t> frameBuffer);
	void setBuffer(uint8_t vramDataArray[6144]);
	void setBuffer(uint8_t vramDataArray[6144], uint8_t SCY, uint8_t SCX, uint16_t tileMapAddress, bool signedMode);
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

