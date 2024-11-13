#include <iostream>
#include <stdio.h>
#include "SDL2/SDL.h"
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "mf/core.h"
#include "mf/colors.h"
#include "mf/graphics.h"

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

int main(int argc, char **argv) {
	bool running = true;
    std::cout << "-------Mikicrep Framework-------" << std::endl
              << "-------Ver: 1.3.0M--------------" << std::endl
              << "Copyright Mikicrep Studios 2024" << std::endl;
	std::cout << "Random SDL Game" << std::endl
			  << "Ver: DEV" << std::endl
			  << "Copyright Mikicrep Studios 2023-2024" << std::endl;

    // Main stuff
    const std::string title = "Mikicrep | Build 81"; // Latest release 1.1
    core::MF_Window window = {};
    SDL_Event event = {};
	bool debug = false;

    // Create window
	if(debug) std::cout << "Creating window" << std::endl;
    if(core::InitWindow(window, title, 1280, 800) == false) running = false;

	// Structs
	if(debug) std::cout << "Defining structs" << std::endl;
	game::Game game = {};
	game::Settings settings = {};
	game::Map map = {};
	game::Player player = {};
	game::Camera cam = {};


	// Preset stuff
	if(debug) std::cout << "Defining presets" << std::endl;
	game::Preset preset[10] = {};
	preset[0].blockColor = 2;


	// Prepare game
	// Initilize structs
	if(debug) std::cout << "Start preparing game : Initilizing structs" << std::endl;
	inventory::rects inventoryRects = inventory::InitRects(window);
	dialogues::rects dialoguesRects = dialogues::InitRects(window);


	// Textures
	if(debug) std::cout << "Init textures" << std::endl;
	textures::BlockTextures blockTextures[32] = {};
	textures::initBlocks(window, blockTextures);

	if(debug) std::cout << "Init icon.png" << std::endl;
	
	// Adding icon to window
	SDL_Surface* iconSurface = IMG_Load("icon.png");
	SDL_SetWindowIcon(window.window, iconSurface);
	SDL_FreeSurface(iconSurface);

	if(debug) std::cout << "Init assets/background.png" << std::endl;

	// Load custom bg
	SDL_Surface* backgroundSurface = IMG_Load("assets/background.png");
	SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(window.renderer, backgroundSurface);
	SDL_FreeSurface(backgroundSurface);

	if(debug) std::cout << "Init SDL stuff; Load save" << std::endl;

	gamemap::ClearMap(map);
	files::LoadMap(map);
	files::LoadSettings(settings, player, cam, preset);

	if(debug) std::cout << "Start running loop" << std::endl;

	while(running) {
		// Main
		SDL_GetMouseState(&window.mouseX, &window.mouseY);
		game.curHoverX = window.mouseX / cam.scale;
		game.curHoverY = window.mouseY / cam.scale;

		// Update vars
		logic::UpdateVars(settings, player, cam, preset);

		// Event loop
		while(SDL_PollEvent(&event) != 0) {
			window.event = event;
			// Check does player hold mouse button
			if(event.type == SDL_MOUSEBUTTONDOWN)
				window.isMouseDown = true;
			else if(event.type == SDL_MOUSEBUTTONUP)
				window.isMouseDown = false;

			if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    window.width = event.window.data1;
                    window.height = event.window.data2;

					// Update rects
					inventoryRects = inventory::InitRects(window);
					dialoguesRects = dialogues::InitRects(window);
                }
			}

			// Window
			if(event.type == SDL_QUIT)
				running = false;
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
					switch(window.fullscreen) {
						case true:
							SDL_SetWindowFullscreen(window.window, 0);
							window.fullscreen = false;
							break;
						case false:
							SDL_SetWindowFullscreen(window.window, SDL_WINDOW_FULLSCREEN);
							window.fullscreen = true;
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
					game.cliInput = true;

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
			events::Camera(window, settings, cam);
			}

			// Dialogues : Yes
			int dialogueResult = dialogues::ConfirmDialogueEvent(window, dialoguesRects);
			if(dialogueResult == 2)
					switch(settings.dialogueId) {
						case 1:
							files::SaveMap(map);
							files::SaveSettings(settings, player, cam);
							running = false;
							break;
						case 2:
							gamemap::ClearMap(map);
							settings.dialogueId = 0;
							settings.dialogue = false;
							break;
					}
			else if(dialogueResult == 3)
				if(settings.dialogueId == 1)
					running = false;

			// Cheats
			if(settings.cheats) {
				int cheatsResult = 0;

				switch(settings.cheatsId) {
					case 1:
						cheatsResult = cheats::CamTp(window, game, settings, cam);
						break;
					case 2:
						cheatsResult = cheats::PlayerTp(window, game, settings, map, cam, player);
				}
				
				if(cheatsResult == 1) settings.cheats = false;
			}

			if(settings.canPlayerPlace == true) mouse::Event(window, game, settings, map, cam, preset);

			if(settings.inventory) inventory::Chooser(window, settings, map, player, cam, preset, inventoryRects);
			}

		// Set BG color to new color
		draw::SetCol(window.renderer, settings.bgColor);
		SDL_RenderClear(window.renderer);
		// ... or if there is custom bg
		SDL_Rect backgroundRect = {0, 0, window.width, window.height};
		SDL_RenderCopy(window.renderer, backgroundTexture, NULL, &backgroundRect);

		// Pre logic
		// Prepare player
		map.map[player.x][player.y].type = 1;

		// Draw map
		game::RenderMap(window, settings, map, cam, blockTextures);

		// Overlays
		inventory::Overlay(window, settings, inventoryRects, blockTextures);
		mouse::Overlay(window, game, settings, map, cam);

		// Game info
		if (settings.gameInfo) hud::GameInfo(window, settings, cam, player);

		// Dialogues : No
		if(settings.dialogue && dialogues::ConfirmDialogue(window, settings, dialoguesRects)) {
			settings.dialogueId = 0;
			settings.dialogue = false;
		}

		// Cli Input
		if(game.cliInput) {
			std::string command = "";
			draw::SetCol(window.renderer, settings.bgColor);
			draw::DrawRect(window.renderer, {0, 0, 25, 25}, colors::red);
			SDL_RenderPresent(window.renderer);
			std::getline(std::cin, command);
			commands::Executor(command, window, settings, map, preset);
			game.cliInput = false;
		}

		// Show results
		SDL_RenderPresent(window.renderer);

		// Timer
		SDL_Delay(1000 / window.fps);
	}

	core::Exit(window);
	return 0;
}

