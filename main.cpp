#include "SDL2/SDL.h"
#include <stdio.h>

int main() {
    // SDL variables
    int width = 800;
    int height = 600;
    int fps = 60;
    int mousey = 0;

    // Game
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
	window = SDL_CreateWindow("Mikicrep ;)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);
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
            if(event.type == SDL_KEYDOWN) {
                // Window
                if(event.type == SDL_QUIT) {
                    running = false;
                }
                else if(event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
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
                else if(event.key.keysym.sym == SDLK_c) {
                    for (int x = 0; x < mapwidth; x++) {
                        for (int y = 0; y < mapheight; y++) {
                            worldmap[x][y] = 0;
                        }
                    }
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        if (worldmap[curhoverx][curhovery] != 1) {
                            worldmap[curhoverx][curhovery] = 10;
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
                if(worldmap[x][y] == 0) {
                    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255 );
                }
                else if(worldmap[x][y] == 1) {
                    SDL_SetRenderDrawColor(renderer, 0, 100, 255, 255 );
                }
                else if(worldmap[x][y] == 10) {
                    SDL_SetRenderDrawColor(renderer, 135, 135, 135, 255 );
                }
                SDL_Rect tmprect = {x * 50, y * 50, 50, 50};
                SDL_RenderFillRect(renderer, &tmprect);
            }
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
