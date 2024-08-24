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
#include "logic.h"
#include "overlay.h"
#include "player.h"
#include "presets.h"
#include "settings.h"

// Latest release 1.0
const char* windowtitle = "Mikicrep | Build 54";

int main(int argc, char **argv) {
	// SDL variables
	bool fullscreen = false;

	// Structs
	game::SDL_Settings sdlSettings = {};
	game::Settings settings = {};
	game::Map map = {};
	game::Player player = {};
	game::Camera camera = {};
	game::Dialogues dialogues = {};
	game::Cheats cheats = {};

	// Preset stuff
	game::Preset preset[10] = {};
	preset[0].blockColor = 2;

	// Prepare game
	// Initilize structs
	inventory::rects inventoryRects = inventory::initRects(sdlSettings);
	dialogues::rects dialoguesRects = dialogues::initRects(sdlSettings);
	SDL_Window *window;
	window = SDL_CreateWindow(windowtitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, sdlSettings.width, sdlSettings.height, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Event event;

	// Load custom bg
	SDL_Surface* backgroundSurface = IMG_Load("customize/background.png");
	SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	sdlSettings.font = TTF_OpenFont("customize/font.ttf", 48);
	IMG_Init(IMG_INIT_PNG);
	gamemap::ClearMap(map);
	files::LoadMap(map);
	files::LoadSettings(settings, player, camera);

	while(sdlSettings.running) {
		// Main
		SDL_GetMouseState(&sdlSettings.mouseX, &sdlSettings.mouseY);
		sdlSettings.curHoverX = sdlSettings.mouseX / camera.scale;
		sdlSettings.curHoverY = sdlSettings.mouseY / camera.scale;

		// Update vars
		logic::updateVars(settings, player, preset);

		// Event loop
		while(SDL_PollEvent(&event) != 0) {
			// Check does player hold mouse button
			if(event.type == SDL_MOUSEBUTTONDOWN)
				sdlSettings.isMouseDown = true;
			else if(event.type == SDL_MOUSEBUTTONUP)
				sdlSettings.isMouseDown = false;

			if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    sdlSettings.width = event.window.data1;
                    sdlSettings.height = event.window.data2;

					// Update rects
					inventoryRects = inventory::initRects(sdlSettings);
					dialoguesRects = dialogues::initRects(sdlSettings);
                }
			}

			// Window
			if(event.type == SDL_QUIT)
				sdlSettings.running = false;
			if(event.type == SDL_KEYDOWN) {
				if(event.key.keysym.sym == SDLK_ESCAPE) {
					if (!settings.inventory)
						dialogues.exitDialogue = true;
					else
						settings.colorPick = false;
					settings.bgColorPick = false;

					settings.inventory = false;
					sdlSettings.fps = 60;
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
			player::PlayerMovement(event, map, player);
			// Inventory
			player::InventoryEvent(event, settings, sdlSettings);

			// Clear map
			if(event.key.keysym.sym == SDLK_c)
				dialogues.clearDialogue = true;

			// Camera
			events::Camera(event, settings.inventory, camera.offSetX, camera.offSetY, camera.scale);
			}

			// Dialogues : Yes
			if(dialogues.exitDialogue && dialogues::confirmDialogueEvent(event, sdlSettings.mouseX, sdlSettings.mouseY, sdlSettings.width, sdlSettings.height, dialoguesRects) == 3)
				sdlSettings.running = false;
			else if(dialogues.exitDialogue && dialogues::confirmDialogueEvent(event, sdlSettings.mouseX, sdlSettings.mouseY, sdlSettings.width, sdlSettings.height, dialoguesRects) == 2) {
				files::SaveMap(map);
				files::SaveSettings(settings, player, camera);
				sdlSettings.running = false;
			}
			if(dialogues.clearDialogue && dialogues::confirmDialogueEvent(event, sdlSettings.mouseX, sdlSettings.mouseY, sdlSettings.width, sdlSettings.height, dialoguesRects) == 2) {
				gamemap::ClearMap(map);
				dialogues.clearDialogue = false;
			}

			// Cheats
			if(cheats.camTp)
				cheats::camTp(event, cheats.camTp, camera.highlight, camera.offSetX - sdlSettings.curHoverX, camera.offSetY - sdlSettings.curHoverY, camera.offSetX, camera.offSetY);
			else if(cheats.playerTp)
				cheats::playerTp(event, map.map, cheats.playerTp, camera.highlight, -camera.offSetX + sdlSettings.curHoverX, -camera.offSetY + sdlSettings.curHoverY, player.x, player.y);
			else
				player::MouseEvent(event, sdlSettings, settings, map, camera, preset);

			player::MouseInvChooser(renderer, event, inventoryRects, sdlSettings, settings, map, player, camera, cheats, preset);
			}

		// Set BG color to new color
		draw::SetCol(renderer, settings.bgColor);
		SDL_RenderClear(renderer);
		// ... or if there is custom bg
		SDL_Rect backgroundRect = {0, 0, sdlSettings.width, sdlSettings.height};
		SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

		// Pre logic
		map.map[player.x][player.y] = Block(1, settings.playerColor);

		// Draw map
		game::RenderMap(renderer, map.map, sdlSettings.width, sdlSettings.height, map.width, map.height, camera.offSetX, camera.offSetY, camera.scale);

		// Overlays
		overlay::Inventory(renderer, sdlSettings.font, inventoryRects, settings.inventory, settings.colorPick, settings.bgColorPick, settings.playerColorPick, settings.gameInfo, preset[settings.curPreset].blockColor, settings.bgColor, settings.playerColor, sdlSettings.width, sdlSettings.height, sdlSettings.mouseX, sdlSettings.mouseY, settings.curPreset);
		overlay::Mouse(renderer, camera.highlight, settings.inventory, map.map, map.width, map.height, sdlSettings.curHoverX, sdlSettings.curHoverY, camera.offSetX, camera.offSetY, camera.scale, settings.bgColor);

		// Game info
		if (settings.gameInfo) {
			if (settings.bgColor == 32)
				player::gameInfo(renderer, sdlSettings, camera, player, sdlSettings.altTextColor);
			else
				player::gameInfo(renderer, sdlSettings, camera, player, sdlSettings.textColor);
		}

		// Dialogues : No
		if (dialogues.exitDialogue && dialogues::confirmDialogue(renderer, sdlSettings.font, sdlSettings.width, sdlSettings.height, sdlSettings.mouseX, sdlSettings.mouseY, 1, dialoguesRects) == 1)
				dialogues.exitDialogue = false;
		if (dialogues.clearDialogue && dialogues::confirmDialogue(renderer, sdlSettings.font, sdlSettings.width, sdlSettings.height, sdlSettings.mouseX, sdlSettings.mouseY, 2, dialoguesRects) == 1)
				dialogues.clearDialogue = false;

		// Show results
		SDL_RenderPresent(renderer);

		// Timer
		SDL_Delay(1000 / sdlSettings.fps);
	}

	IMG_Quit();
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

