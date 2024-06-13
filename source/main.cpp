#include <iostream>
#include <stdio.h>
#include "SDL2/SDL.h"
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "block.h"
#include "cheats.h"
#include "controls.h"
#include "files.h"
#include "game.h"
#include "graphics.h"
#include "overlay.h"
#include "player.h"
#include "presets.h"

const char* windowtitle = "Mikicrep | Build 43";

int fps = 60;
int width = 1280;
int height = 800;

int main() {
	// SDL variables
	int bgColor = 1;
	int mouseX, mouseY = 0;
	int colorR, colorG, colorB = 0;

	// Some colors
	SDL_Color textColor = {255, 255, 255};
	SDL_Color altTextColor = {0, 0, 0};

	// Cam
	int camScale = 50;
	int camOffSetX, camOffSetY = 0;

	// Game
	bool highlight = false;
	bool colorPickerTool, playerTp, camTp = false;
	bool inventory, colorPick, bgColorPick, playerColorPick = false;
	bool gameInfo = false;
	int blockColor = 2;
	Preset preset[10];
	int curPreset = 1;
	int curHoverX, curHoverY = 0;

	// Game world
	int mapWidth = 250 - 1;
	int mapHeight = 250 - 1;
	Block worldMap[250][250] = {};

	// Player
	int playerX, playerY = 0;
	int playerColor = 6;
	int playerSpeed = 1;

	// Prepare game
	SDL_Window *window;
	window = SDL_CreateWindow(windowtitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Event event;

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	TTF_Font* font = TTF_OpenFont("customize/font.ttf", 48);
	IMG_Init(IMG_INIT_PNG);
	gamemap::ClearMap(worldMap, mapWidth, mapHeight);
	files::LoadMap(worldMap, mapWidth, mapHeight);
	files::LoadSettings(playerX, playerY, camOffSetX, camOffSetY, camScale, playerColor, blockColor, bgColor);
	bool running = true;

	while(running) {
		// Main
		SDL_GetMouseState(&mouseX, &mouseY);
		curHoverX = mouseX / camScale;
		curHoverY = mouseY / camScale;

		// Event loop
		while(SDL_PollEvent(&event) != 0) {
			// Window
			if(event.type == SDL_QUIT)
				running = false;
			if(event.type == SDL_KEYDOWN) {
				if(event.key.keysym.sym == SDLK_ESCAPE) {
					if (!inventory)
						running = false;
					else
						colorPick = false;
					bgColorPick = false;

					inventory = false;
					fps = 60;
				}
				if(event.key.keysym.sym == SDLK_q) {
					colorPickerTool = !colorPickerTool;
					highlight = !highlight;
				}
				if(event.key.keysym.sym == SDLK_F3)
					gameInfo = !gameInfo;

			// Preset chooser
			controls::presetChooser(event, curPreset);
			// Player movement
			player::PlayerMovement(event, worldMap, mapWidth, mapHeight, playerSpeed, playerX, playerY);
			// Inventory
			player::InventoryEvent(event, inventory, colorPick, bgColorPick, fps);

			// Clear map
			if(event.key.keysym.sym == SDLK_c)
				gamemap::ClearMap(worldMap, mapWidth, mapHeight);

			// Camera
			events::Camera(event, inventory, camOffSetX, camOffSetY, camScale);
			}

			// Cheats
			if(camTp)
				cheats::camTp(event, camTp, highlight, camOffSetX - curHoverX, camOffSetY - curHoverY, camOffSetX, camOffSetY);
			else if(playerTp)
				cheats::playerTp(event, worldMap, playerTp, highlight, -camOffSetX + curHoverX, -camOffSetY + curHoverY, playerX, playerY);
			else
				player::MouseEvent(event, colorPickerTool, highlight, inventory, worldMap, mapWidth, mapHeight, curHoverX, curHoverY, blockColor, camOffSetX, camOffSetY);

			player::MouseInvChooser(renderer, event, inventory, running, highlight, camTp, playerTp, colorPick, bgColorPick, playerColorPick, gameInfo, worldMap, mapWidth, mapHeight, blockColor, bgColor, playerColor, mouseX, mouseY, width, height, playerX, playerY, camOffSetX, camOffSetY, camScale);
		}

		// Set BG color to new color
		graphics::GetColor(bgColor, colorR, colorG, colorB);
		SDL_SetRenderDrawColor(renderer, colorR, colorG, colorB, 255);
		SDL_RenderClear(renderer);
		// ... or if there is custom bg
		SDL_Surface* backgroundSurface = IMG_Load("customize/background.png");
		SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
		SDL_Rect backgroundRect = {0, 0, width, height};
		SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

		// Pre logic
		worldMap[playerX][playerY] = Block(1, playerColor);

		// Draw map
		game::RenderMap(renderer, worldMap, width, height, mapWidth, mapHeight, camOffSetX, camOffSetY, camScale);

		// Overlays
		overlay::Inventory(renderer, font, inventory, colorPick, bgColorPick, playerColorPick, gameInfo, blockColor, bgColor, playerColor, mouseX, mouseY, curPreset);
		overlay::Mouse(renderer, highlight, inventory, worldMap, mapWidth, mapHeight, curHoverX, curHoverY, camOffSetX, camOffSetY, camScale, bgColor);

		// Game info
		if (gameInfo) {
			if (bgColor == 32)
				player::gameInfo(renderer, font, altTextColor, fps, camScale, camOffSetX, camOffSetY, playerX, playerY);
			else
				player::gameInfo(renderer, font, textColor, fps, camScale, camOffSetX, camOffSetY, playerX, playerY);
		}

		// Show results
		SDL_RenderPresent(renderer);

		// Timer
		SDL_Delay(1000 / fps);
	}

	IMG_Quit();
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

