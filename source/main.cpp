#include <iostream>
#include <stdio.h>
#include "SDL2/SDL.h"
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "addional.h"
#include "block.h"
#include "cheats.h"
#include "controls.h"
#include "dialogues.h"
#include "files.h"
#include "game.h"
#include "inventory.h"
#include "overlay.h"
#include "player.h"
#include "presets.h"
#include "settings.h"

// 1.0
const char* windowtitle = "Mikicrep | 1.0 | Build 50";

int fps = 60;
int width = 1280;
int height = 800;

int main(int argc, char **argv) {
	// SDL variables
	bool fullscreen = false;
	bool isMouseDown = false;
	int mouseX = 0;
	int mouseY = 0;

	// Structs
	game::Settings settings = {};
	game::Player player = {};
	game::Camera camera = {};
	game::Dialogues dialogues = {};

	// Some colors
	SDL_Color textColor = {255, 255, 255};
	SDL_Color altTextColor = {0, 0, 0};

	// Game
	bool playerTp = false;
	bool camTp = false;

	// Preset stuff
	settings::Preset preset[10] = {};
	preset[0].blockColor = 2;

	int curHoverX = 0;
	int curHoverY = 0;

	// Game world
	int mapWidth = 250 - 1;
	int mapHeight = 250 - 1;
	Block worldMap[250][250] = {};

	// Prepare game
	// Initilize structs
	inventory::rects inventoryRects = inventory::initRects(width, height);

	SDL_Window *window;
	window = SDL_CreateWindow(windowtitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Event event;

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	TTF_Font* font = TTF_OpenFont("customize/font.ttf", 48);
	IMG_Init(IMG_INIT_PNG);
	gamemap::ClearMap(worldMap, mapWidth, mapHeight);
	files::LoadMap(worldMap, mapWidth, mapHeight);
	files::LoadSettings(settings, player, camera, preset[settings.curPreset].blockColor);
	bool running = true;

	while(running) {
		// Main
		SDL_GetMouseState(&mouseX, &mouseY);
		curHoverX = mouseX / camera.scale;
		curHoverY = mouseY / camera.scale;

		// Event loop
		while(SDL_PollEvent(&event) != 0) {
			// Check does player hold mouse button
			if(event.type == SDL_MOUSEBUTTONDOWN)
				isMouseDown = true;
			else if(event.type == SDL_MOUSEBUTTONUP)
				isMouseDown = false;

			if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    width = event.window.data1;
                    height = event.window.data2;

					// Update rects
					inventoryRects = inventory::initRects(width, height);
                }
			}

			// Window
			if(event.type == SDL_QUIT)
				running = false;
			if(event.type == SDL_KEYDOWN) {
				if(event.key.keysym.sym == SDLK_ESCAPE) {
					if (!settings.inventory)
						dialogues.exitDialogue = true;
					else
						settings.colorPick = false;
					settings.bgColorPick = false;

					settings.inventory = false;
					fps = 60;
				}

				if(event.key.keysym.sym == SDLK_F11) {
					switch(fullscreen) {
						case true:
							SDL_SetWindowFullscreen(window, 0);
							fullscreen = false;
							break;
						case false:
							SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
							fullscreen = true;
							break;
					}
				}

				if(event.key.keysym.sym == SDLK_q) {
					settings.colorPickerTool = !settings.colorPickerTool;
					camera.highlight = !camera.highlight;
				}
				if(event.key.keysym.sym == SDLK_F3)
					settings.gameInfo = !settings.gameInfo;

			// Preset chooser
			controls::presetChooser(event, settings.curPreset);
			// Player movement
			player::PlayerMovement(event, worldMap, mapWidth, mapHeight, player);
			// Inventory
			player::InventoryEvent(event, settings.inventory, settings.colorPick, settings.bgColorPick, fps);

			// Clear map
			if(event.key.keysym.sym == SDLK_c)
				dialogues.clearDialogue = true;

			// Camera
			events::Camera(event, settings.inventory, camera.offSetX, camera.offSetY, camera.scale);
			}

			// Dialogues : Yes
			if(dialogues.exitDialogue && dialogues::confirmDialogueEvent(event, mouseX, mouseY, width, height) == 3)
				running = false;
			else if(dialogues.exitDialogue && dialogues::confirmDialogueEvent(event, mouseX, mouseY, width, height) == 2) {
				files::SaveMap(worldMap, mapWidth, mapHeight);
				files::SaveSettings(settings, player, camera, preset[settings.curPreset].blockColor);
				running = false;
			}
			if(dialogues.clearDialogue && dialogues::confirmDialogueEvent(event, mouseX, mouseY, width, height) == 2) {
				gamemap::ClearMap(worldMap, mapWidth, mapHeight);
				dialogues.clearDialogue = false;
			}

			// Cheats
			if(camTp)
				cheats::camTp(event, camTp, camera.highlight, camera.offSetX - curHoverX, camera.offSetY - curHoverY, camera.offSetX, camera.offSetY);
			else if(playerTp)
				cheats::playerTp(event, worldMap, playerTp, camera.highlight, -camera.offSetX + curHoverX, -camera.offSetY + curHoverY, player.x, player.y);
			else
				player::MouseEvent(event, isMouseDown, settings.colorPickerTool, camera.highlight, settings.inventory, worldMap, mapWidth, mapHeight, curHoverX, curHoverY, preset[settings.curPreset].blockColor, camera.offSetX, camera.offSetY);

			player::MouseInvChooser(renderer, event, inventoryRects, settings.inventory, running, camera.highlight, camTp, playerTp, settings.colorPick, settings.bgColorPick, settings.playerColorPick, settings.gameInfo, worldMap, mapWidth, mapHeight, preset[settings.curPreset].blockColor, settings.bgColor, player.color, mouseX, mouseY, width, height, player.x, player.y, camera.offSetX, camera.offSetY, camera.scale, settings, player, camera);
		}

		// Set BG color to new color
		draw::SetCol(renderer, settings.bgColor);
		SDL_RenderClear(renderer);
		// ... or if there is custom bg
		SDL_Surface* backgroundSurface = IMG_Load("customize/background.png");
		SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
		SDL_Rect backgroundRect = {0, 0, width, height};
		SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

		// Pre logic
		worldMap[player.x][player.y] = Block(1, player.color);


		// Draw map
		game::RenderMap(renderer, worldMap, width, height, mapWidth, mapHeight, camera.offSetX, camera.offSetY, camera.scale);

		// Overlays
		overlay::Inventory(renderer, font, inventoryRects, settings.inventory, settings.colorPick, settings.bgColorPick, settings.playerColorPick, settings.gameInfo, preset[settings.curPreset].blockColor, settings.bgColor, player.color, width, height, mouseX, mouseY, settings.curPreset);
		overlay::Mouse(renderer, camera.highlight, settings.inventory, worldMap, mapWidth, mapHeight, curHoverX, curHoverY, camera.offSetX, camera.offSetY, camera.scale, settings.bgColor);

		// Game info
		if (settings.gameInfo) {
			if (settings.bgColor == 32)
				player::gameInfo(renderer, font, altTextColor, fps, camera.scale, camera.offSetX, camera.offSetY, player.x, player.y);
			else
				player::gameInfo(renderer, font, textColor, fps, camera.scale, camera.offSetX, camera.offSetY, player.x, player.y);
		}

		// Dialogues : No
		if (dialogues.exitDialogue && dialogues::confirmDialogue(renderer, font, width, height, mouseX, mouseY, 1) == 1)
				dialogues.exitDialogue = false;
		if (dialogues.clearDialogue && dialogues::confirmDialogue(renderer, font, width, height, mouseX, mouseY, 2) == 1)
				dialogues.clearDialogue = false;

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

