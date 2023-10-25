#include "SDL2/SDL.h"
#include <stdio.h>

#include "objects.h"
#include "colors.h"

int main() {
    // SDL variables
    int width = 800;
    int height = 600;
    int fps = 60;
    int mousex = 0;
    int mousey = 0;

    // Graphics
    int colorid = 0;
    int colorr = 0;
    int colorg = 0;
    int colorb = 0;

    // Game
    bool inventory = false;
    int curblock = 17;
    int curhoverx = 0;
    int curhovery = 0;

    // Game world
    int mapwidth = width / 50;
    int mapheight = height / 50;
    int worldmap[mapwidth][mapheight];

    // Player
    int playerx = width / 2 / 50;
    int playery = height / 2 / 50;
    int playerspeed = 1;
    // 16x12

    bool running = true;

	SDL_Window *window;
	window = SDL_CreateWindow("Mikicrep ;) | Build 3", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_Init(SDL_INIT_VIDEO);

	for (int x = 0; x < mapwidth; x++) {
        for (int y = 0; y < mapheight; y++) {
            worldmap[x][y] = 0;
        }
    }

	while(running) {
        // Main
        SDL_GetMouseState(&mousex, &mousey);
        curhoverx = mousex / 50;
        curhovery = mousey / 50;
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
                // PLAYER MOVEMENT
                else if(event.key.keysym.sym == SDLK_w) {
                    if (worldmap[playerx][playery - playerspeed] == 0) {
                        worldmap[playerx][playery] = 0;
                        playery -= playerspeed;
                    }
                }
                else if(event.key.keysym.sym == SDLK_a) {
                    if (worldmap[playerx - playerspeed][playery] == 0) {
                        worldmap[playerx][playery] = 0;
                        playerx -= playerspeed;
                    }
                }
                else if(event.key.keysym.sym == SDLK_s) {
                    if (worldmap[playerx][playery + playerspeed] == 0) {
                        worldmap[playerx][playery] = 0;
                        playery += playerspeed;
                    }
                }
                else if(event.key.keysym.sym == SDLK_d) {
                    if (worldmap[playerx + playerspeed][playery] == 0) {
                        worldmap[playerx][playery] = 0;
                        playerx += playerspeed;
                    }
                }
                else if(event.key.keysym.sym == SDLK_e) {
                    inventory = !inventory;
                }
                else if(event.key.keysym.sym == SDLK_c) {
                    for (int x = 0; x < mapwidth; x++) {
                        for (int y = 0; y < mapheight; y++) {
                            worldmap[x][y] = 0;
                        }
                    }
                }
                // Inventory
                if (inventory) {
                    if (event.key.keysym.sym == SDLK_LEFT) {
                        if (curblock == 10) {
                            curblock = 25;
                        }
                        else {
                            curblock -= 1;
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_RIGHT) {
                        if (curblock == 25) {
                            curblock = 10;
                        }
                        else {
                            curblock += 1;
                        }
                    }
                }
            }
            // Place/Break
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (worldmap[curhoverx][curhovery] != 1) {
                        worldmap[curhoverx][curhovery] = curblock;
                    }
                }
                else if (event.button.button == SDL_BUTTON_RIGHT) {
                    if (worldmap[curhoverx][curhovery] != 1) {
                        worldmap[curhoverx][curhovery] = 0;
                    }
                }
            }
        }

        // Prepare for drawing next frame
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Pre logic
        worldmap[playerx][playery] = 1;

        // Draw map
        for(int x = 0; x <= mapwidth; x++) {
            for(int y = 0; y <= mapheight; y++) {
                // Get current block ID
                int objectid = worldmap[x][y];

                objectColor(objectid, colorid);
                graphics::getColor(colorid, colorr, colorg, colorb);

                // Do rendering
                SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
                SDL_Rect tmprect = {x * 50, y * 50, 50, 50};
                SDL_RenderFillRect(renderer, &tmprect);
            }
        }

        // Menus
        // Inventory
        if (inventory) {
            graphics::getColor(8, colorr, colorg, colorb);
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_Rect bgrect = {25, 25, width - 50, height - 50};
            SDL_RenderFillRect(renderer, &bgrect);

            graphics::getColor(curblock - 10, colorr, colorg, colorb);
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_Rect curblockrect = {50, 50, 50, 50};
            SDL_RenderFillRect(renderer, &curblockrect);
        }

        // Overlay
        if (worldmap[curhoverx][curhovery] != 1) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255 );
            SDL_Rect mouserect = {curhoverx * 50, curhovery * 50, 50, 50};
            SDL_RenderFillRect(renderer, &mouserect);
        }

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
