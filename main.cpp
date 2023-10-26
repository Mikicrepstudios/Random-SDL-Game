#include <iostream>
#include <fstream>
#include <stdio.h>
#include "SDL2/SDL.h"

#include "files.h"
#include "game.h"
#include "graphics.h"
#include "objects.h"
#include "overlay.h"
#include "player.h"

int main() {
    // SDL variables
    const char* windowtitle = "Mikicrep | Build 9";
    int width = 1280;
    int height = 800;
    int fps = 60;
    int mousex = 0;
    int mousey = 0;

    // Cam
    int camscale = 50;
    int camoffsetx = 0;
    int camoffsety = 0;

    // Game
    bool inventory = false;
    int curblock = 17;
    int curhoverx = 0;
    int curhovery = 0;

    // Game world
    int mapwidth = 250 - 1;
    int mapheight = 250 - 1;
    int worldmap[250][250];

    // Player
    int playerx = 0;
    int playery = 0;
    int playerspeed = 1;

    // Prepare game
	SDL_Window *window;
	window = SDL_CreateWindow(windowtitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_Init(SDL_INIT_VIDEO);
	gamemap::clearmap(worldmap, mapwidth, mapheight);
    bool running = true;

	while(running) {
        // Main
        SDL_GetMouseState(&mousex, &mousey);
        curhoverx = mousex / camscale;
        curhovery = mousey / camscale;
        SDL_Event event;

        // Event loop
        while(SDL_PollEvent(&event) != 0) {
            // Window
            if(event.type == SDL_QUIT) {
                running = false;
            }
            if(event.type == SDL_KEYDOWN) {
                if(event.key.keysym.sym == SDLK_ESCAPE) {
                    if (!inventory) {
                        running = false;
                    }
                    else {
                        inventory = !inventory;
                    }
                }
                // Player movement
                player::playermovement(event, worldmap, playerspeed, playerx, playery);
                // Inventory
                player::inventoryevent(event, inventory, curblock);

                // Clear map
                if(event.key.keysym.sym == SDLK_c) {
                    gamemap::clearmap(worldmap, mapwidth, mapheight);
                }

                // Camera
                events::camera(event, inventory, camoffsetx, camoffsety, camscale);

                // Save load
                files::savemapevent(event, worldmap, mapwidth, mapheight);
                files::loadmapevent(event, worldmap, mapwidth, mapheight);
            }

            // Place/Break
            player::mouseevent(event, worldmap, mapwidth, mapheight, curhoverx, curhovery, curblock, camoffsetx, camoffsety);
        }

        // Prepare for drawing next frame
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Pre logic
        worldmap[playerx][playery] = 1;

        // Draw map
        game::rendermap(renderer, worldmap, mapwidth, mapheight, camoffsetx, camoffsety, camscale);

        // Overlays
        overlay::inventory(renderer, width, height, inventory, curblock);
        overlay::mouse(renderer, worldmap, mapwidth, mapheight, curhoverx, curhovery, camoffsetx, camoffsety, camscale);

        // Show results
        SDL_RenderPresent(renderer);

        // Timer
        SDL_Delay(1000 / fps);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	return 0;
}

// 0 Black     8 Gray
// 1 Brown     9 Red
// 2 Green     10 Light green
// 3 Olive     11 Yellow
// 4 Dark blue 12 Royal blue
// 5 Purple    13 Light purple
// 6 Cyan      14 Aqua
// 7 Silver    15 White
