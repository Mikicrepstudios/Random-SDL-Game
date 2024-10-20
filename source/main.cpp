#include <iostream>
#include <stdio.h>
#include "SDL2/SDL.h"
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "block.h"
#include "cheats.h"
#include "controls.h"
#include "commands.h"
#include "dialogues.h"
#include "draw.h"
#include "files.h"
#include "game.h"
#include "hud.h"
#include "inventory.h"
#include "logic.h"
#include "mouse.h"
#include "player.h"
#include "presets.h"
#include "settings.h"
#include "textures.h"

// Latest release 1.1
const char* windowtitle = "Mikicrep | Build 77";

int main(int argc, char **argv) {
	// SDL variables
	bool debug = false;
	bool fullscreen = false;

	if(debug) std::cout << "Defining structs" << std::endl;

	// Structs
	game::SDL_Settings sdlSettings = {};
	game::Settings settings = {};
	game::Map map = {};
	game::Player player = {};
	game::Camera cam = {};

	if(debug) std::cout << "Defining presets" << std::endl;

	// Preset stuff
	game::Preset preset[10] = {};
	preset[0].blockColor = 2;

	if(debug) std::cout << "Start preparing game : Initilizing structs" << std::endl;

	// Prepare game
	// Initilize structs
	inventory::rects inventoryRects = inventory::InitRects(sdlSettings);
	dialogues::rects dialoguesRects = dialogues::InitRects(sdlSettings);
	SDL_Window* window = SDL_CreateWindow(windowtitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
							  sdlSettings.width, sdlSettings.height, SDL_WINDOW_RESIZABLE);
	sdlSettings.renderer = SDL_CreateRenderer(window, -1, 0);

	if(debug) std::cout << "Init textures" << std::endl;

	// Textures
	textures::BlockTextures blockTextures[32] = {};
	textures::initBlocks(sdlSettings, blockTextures);

	if(debug) std::cout << "Init icon.png" << std::endl;
	
	// Adding icon to window
	SDL_Surface* iconSurface = IMG_Load("icon.png");
	SDL_SetWindowIcon(window, iconSurface);
	SDL_FreeSurface(iconSurface);

	if(debug) std::cout << "Init customize/background.png" << std::endl;

	// Load custom bg
	SDL_Surface* backgroundSurface = IMG_Load("customize/background.png");
	SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(sdlSettings.renderer, backgroundSurface);
	SDL_FreeSurface(backgroundSurface);

	if(debug) std::cout << "Init SDL stuff; Load save" << std::endl;

	SDL_Event event = {};
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	sdlSettings.font = TTF_OpenFont("customize/font.ttf", 48);
	IMG_Init(IMG_INIT_PNG);
	gamemap::ClearMap(map);
	files::LoadMap(map);
	files::LoadSettings(settings, player, cam, preset);

	if(debug) std::cout << "Start running loop" << std::endl;

	while(sdlSettings.running) {
		// Main
		SDL_GetMouseState(&sdlSettings.mouseX, &sdlSettings.mouseY);
		sdlSettings.curHoverX = sdlSettings.mouseX / cam.scale;
		sdlSettings.curHoverY = sdlSettings.mouseY / cam.scale;

		// Update vars
		logic::UpdateVars(settings, player, cam, preset);

		// Event loop
		while(SDL_PollEvent(&event) != 0) {
			sdlSettings.event = event;
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
					inventoryRects = inventory::InitRects(sdlSettings);
					dialoguesRects = dialogues::InitRects(sdlSettings);
                }
			}

			// Window
			if(event.type == SDL_QUIT)
				sdlSettings.running = false;
			if(event.type == SDL_KEYDOWN) {
				if(event.key.keysym.sym == SDLK_ESCAPE) {
					if (!settings.inventory) {
						settings.dialogueId = 1;
						settings.dialogue = !settings.dialogue;
					}

					settings.colorPicker = false;
					settings.inventory = false;
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
					settings.cheatsId = 3;
					settings.cheats = !settings.cheats;
				}
				if(event.key.keysym.sym == SDLK_F3)
					settings.gameInfo = !settings.gameInfo;

				if(event.key.keysym.sym == SDLK_F9)
					sdlSettings.cliInput = true;

			// Preset chooser
			controls::PresetChooser(event, settings.curPreset);
			// Player movement
			player::Movement(event, map, player);
			// Inventory
			inventory::Event(event, settings);

			// Clear map
			if(event.key.keysym.sym == SDLK_c) {
				settings.dialogueId = 2;
				settings.dialogue = !settings.dialogue;
			}

			// Camera
			events::Camera(sdlSettings, settings, cam);
			}

			// Dialogues : Yes
			int dialogueResult = dialogues::ConfirmDialogueEvent(event, sdlSettings, dialoguesRects);
			if(dialogueResult == 2)
					switch(settings.dialogueId) {
						case 1:
							files::SaveMap(map);
							files::SaveSettings(settings, player, cam);
							sdlSettings.running = false;
							break;
						case 2:
							gamemap::ClearMap(map);
							settings.dialogueId = 0;
							settings.dialogue = false;
							break;
					}
			else if(dialogueResult == 3)
				if(settings.dialogueId == 1)
					sdlSettings.running = false;

			// Cheats
			if(settings.cheats) {
				int cheatsResult = 0;

				switch(settings.cheatsId) {
					case 1:
						cheatsResult = cheats::CamTp(sdlSettings, settings, cam);
						break;
					case 2:
						cheatsResult = cheats::PlayerTp(sdlSettings, settings, map, cam, player);
				}
				
				if(cheatsResult == 1) settings.cheats = false;
			}

			if(settings.canPlayerPlace == true) mouse::Event(event, sdlSettings, settings, map, cam, preset);

			if(settings.inventory) inventory::Chooser(sdlSettings, settings, map, player, cam, preset, inventoryRects);
			}

		// Set BG color to new color
		draw::SetCol(sdlSettings.renderer, settings.bgColor);
		SDL_RenderClear(sdlSettings.renderer);
		// ... or if there is custom bg
		SDL_Rect backgroundRect = {0, 0, sdlSettings.width, sdlSettings.height};
		SDL_RenderCopy(sdlSettings.renderer, backgroundTexture, NULL, &backgroundRect);

		// Pre logic
		// Prepare player
		map.map[player.x][player.y].type = 1;

		// Draw map
		game::RenderMap(sdlSettings, settings, map, cam, blockTextures);

		// Overlays
		inventory::Overlay(sdlSettings, settings, inventoryRects, blockTextures);
		mouse::Overlay(sdlSettings, settings, map, cam);

		// Game info
		if (settings.gameInfo) hud::GameInfo(sdlSettings, settings, cam, player);

		// Dialogues : No
		if(settings.dialogue && dialogues::ConfirmDialogue(sdlSettings, settings, dialoguesRects)) {
			settings.dialogueId = 0;
			settings.dialogue = false;
		}

		// Cli Input
		if(sdlSettings.cliInput) {
			std::string command = "";
			draw::SetCol(sdlSettings.renderer, settings.bgColor);
			draw::DrawRect(sdlSettings.renderer, {0, 0, 25, 25}, 27);
			SDL_RenderPresent(sdlSettings.renderer);
			std::getline(std::cin, command);
			commands::Executor(command, sdlSettings, settings, map, preset);
			sdlSettings.cliInput = false;
		}

		// Show results
		SDL_RenderPresent(sdlSettings.renderer);

		// Timer
		SDL_Delay(1000 / sdlSettings.fps);
	}

	IMG_Quit();
	TTF_Quit();
	SDL_DestroyRenderer(sdlSettings.renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

