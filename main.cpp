#include <iostream>
#include <stdio.h>
#include "SDL2/SDL.h"
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "files.h"
#include "game.h"
#include "graphics.h"
#include "objects.h"
#include "overlay.h"
#include "player.h"

const char* windowtitle = "Mikicrep | Build 18";

int fps = 60;
int width = 1280;
int height = 800;

int main() {
    // SDL variables
    int bgcolor = 0;
    int mousex, mousey = 0;
    int colorr, colorg, colorb = 0;

    // Cam
    int camscale = 50;
    int camoffsetx, camoffsety = 0;

    // Game
    bool inventory = false;
    int curblock = 17;
    int curhoverx,curhovery = 0;

    // Game world
    int mapwidth = 250 - 1;
    int mapheight = 250 - 1;
    int worldmap[250][250];

    // Player
    int playerx, playery = 0;
    int playerspeed = 1;

    // Prepare game
	SDL_Window *window;
	window = SDL_CreateWindow(windowtitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Event event;

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	IMG_Init(IMG_INIT_PNG);
	TTF_Font* font = TTF_OpenFont("font.ttf", 24);
	gamemap::clearmap(worldmap, mapwidth, mapheight);
	files::loadmapevent(event, worldmap, mapwidth, mapheight);
    bool running = true;

	while(running) {
        // Main
        SDL_GetMouseState(&mousex, &mousey);
        curhoverx = mousex / camscale;
        curhovery = mousey / camscale;

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
                        inventory = !inventory;
                }
                // Player movement
                player::playermovement(event, worldmap, mapwidth, mapheight, playerspeed, playerx, playery);
                // Inventory
                player::inventoryevent(event, inventory);

                // Clear map
                if(event.key.keysym.sym == SDLK_c)
                    gamemap::clearmap(worldmap, mapwidth, mapheight);

                // Camera
                events::camera(event, inventory, camoffsetx, camoffsety, camscale);
            }

            // Place/Break
            player::mouseevent(event, inventory, worldmap, mapwidth, mapheight, curhoverx, curhovery, curblock, camoffsetx, camoffsety);
            player::mouseinvchooser(event, inventory, running, worldmap, mapwidth, mapheight, curblock, bgcolor, mousex, mousey);
        }

        // Set BG color to new color
        graphics::getColor(bgcolor, colorr, colorg, colorb);
        SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
        SDL_RenderClear(renderer);
        // ... or if there is custom bg
        SDL_Surface* backgroundsurface = IMG_Load("background.png");
        SDL_Texture* backgroundtexture = SDL_CreateTextureFromSurface(renderer, backgroundsurface);
        SDL_Rect backgroundrect = {0, 0, width, height};
        SDL_RenderCopy(renderer, backgroundtexture, NULL, &backgroundrect);

        // Pre logic
        worldmap[playerx][playery] = 1;

        // Draw map
        game::rendermap(renderer, worldmap, mapwidth, mapheight, camoffsetx, camoffsety, camscale);

        // Overlays
        overlay::inventory(renderer, font, width, height, inventory, curblock, bgcolor, mousex, mousey);
        overlay::mouse(renderer, inventory, worldmap, mapwidth, mapheight, curhoverx, curhovery, camoffsetx, camoffsety, camscale);

        // Show results
        SDL_RenderPresent(renderer);

        // Timer
        SDL_Delay(1000 / fps);
	}

	IMG_Quit();
	TTF_CloseFont(font);
    TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
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
