#pragma once
#include <SDL.h>
#include <vector>
#include "Sprite.h"

class vramViewerEngine
{
public:
	vramViewerEngine(int width, int height);
	~vramViewerEngine();

	void createWindow();

	void destroyWindow();

	void setBuffer(std::vector<uint32_t> frameBuffer);
	void setBuffer(uint8_t vramDataArray[6144]);
	void setBuffer(uint8_t vramDataArray[6144], uint8_t SCY, uint8_t SCX, uint16_t tileMapAddress, bool signedMode);
	void render();
	void renderDebug();

	void renderRAMViewer(std::vector<uint8_t>& ramCopy);

	int frameCount = 0;
	int lastFrameTime = 0;
	bool vramViewerVisible = false;

private:
	int screenWidth;
	int screenHeight;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	std::vector<uint32_t> pixelBuffer;
	std::vector<uint32_t> pixelBufferA;
};

