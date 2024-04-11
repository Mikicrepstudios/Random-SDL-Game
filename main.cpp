#include <iostream>
#include <stdio.h>
#include "SDL2/SDL.h"
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "block.h"
#include "files.h"
#include "game.h"
#include "graphics.h"
#include "overlay.h"
#include "player.h"

const char* windowtitle = "Mikicrep | Build 24";

int fps = 60;
int width = 1280;
int height = 800;

int main() {
    // SDL variables
    int bgColor = 1;
    int mouseX, mouseY = 0;
    int colorR, colorG, colorB = 0;

    // Cam
    int camScale = 50;
    int camOffSetX, camOffSetY = 0;

    // Game
    bool inventory = false;
    bool colorPick = false;
    bool bgcolorPick = false;
    int curBlock = 2;
    int curHoverX, curHoverY = 0;

    // Game world
    int mapWidth = 250 - 1;
    int mapHeight = 250 - 1;
    Block worldMap[250][250] = {};

    // Player
    int playerX, playerY = 0;
    int playerSpeed = 1;

    // Prepare game
	SDL_Window *window;
	window = SDL_CreateWindow(windowtitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Event event;

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	IMG_Init(IMG_INIT_PNG);
	TTF_Font* font = TTF_OpenFont("font.ttf", 24);
	gamemap::ClearMap(worldMap, mapWidth, mapHeight);
	files::LoadMap(event, worldMap, mapWidth, mapHeight);
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
                        inventory = !inventory;
                }
                // Player movement
                player::PlayerMovement(event, worldMap, mapWidth, mapHeight, playerSpeed, playerX, playerY);
                // Inventory
                player::InventoryEvent(event, inventory, colorPick, bgcolorPick);

                // Clear map
                if(event.key.keysym.sym == SDLK_c)
                    gamemap::ClearMap(worldMap, mapWidth, mapHeight);

                // Camera
                events::Camera(event, inventory, camOffSetX, camOffSetY, camScale);
            }

            // Place/Break
            player::MouseEvent(event, inventory, worldMap, mapWidth, mapHeight, curHoverX, curHoverY, curBlock, camOffSetX, camOffSetY);
            player::MouseInvChooser(renderer, event, inventory, running, colorPick, bgcolorPick, worldMap, mapWidth, mapHeight, curBlock, bgColor, mouseX, mouseY, width, height);
        }

        // Set BG color to new color
        graphics::GetColor(bgColor, colorR, colorG, colorB);
        SDL_SetRenderDrawColor(renderer, colorR, colorG, colorB, 255);
        SDL_RenderClear(renderer);
        // ... or if there is custom bg
        SDL_Surface* backgroundSurface = IMG_Load("background.png");
        SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_Rect backgroundRect = {0, 0, width, height};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        // Pre logic
        worldMap[playerX][playerY] = Block(1, 6);

        // Draw map
        game::RenderMap(renderer, worldMap, mapWidth, mapHeight, camOffSetX, camOffSetY, camScale);

        // Overlays
        overlay::Inventory(renderer, font, inventory, colorPick, bgcolorPick, curBlock, bgColor, mouseX, mouseY);
        overlay::Mouse(renderer, inventory, worldMap, mapWidth, mapHeight, curHoverX, curHoverY, camOffSetX, camOffSetY, camScale);

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

// 800x400
