#include "SDL2/SDL.h"
#include <SDL_image.h>

#include "settings.h"
#include "textures.h"

#include <iostream>

namespace textures {
    void initBlocks(game::SDL_Settings &sdlSettings, BlockTextures blockTextures[32]) {
	    SDL_Surface* placeholderSurface = IMG_Load("customize/textures/blocks/placeholder.png");
        SDL_Surface* bricksSurface = IMG_Load("customize/textures/blocks/bricks.png");
	    SDL_Surface* stoneSurface = IMG_Load("customize/textures/blocks/stone.png");
	    SDL_Surface* woodSurface = IMG_Load("customize/textures/blocks/wood.png");
	    SDL_Surface* logsSurface = IMG_Load("customize/textures/blocks/logs.png");
	    SDL_Surface* grassSurface = IMG_Load("customize/textures/blocks/grass.png");
	    SDL_Surface* dirtSurface = IMG_Load("customize/textures/blocks/dirt.png");

	    SDL_Surface* sandSurface = IMG_Load("customize/textures/blocks/sand.png");
	    SDL_Surface* oreSurface = IMG_Load("customize/textures/blocks/ore.png");
	    SDL_Surface* glassSurface = IMG_Load("customize/textures/blocks/glass.png");
	    SDL_Surface* waterSurface = IMG_Load("customize/textures/blocks/water.png");
	    SDL_Surface* lavaSurface = IMG_Load("customize/textures/blocks/lava.png");
	    SDL_Surface* woolSurface = IMG_Load("customize/textures/blocks/wool.png");
	    SDL_Surface* leavesSurface = IMG_Load("customize/textures/blocks/leaves.png");
	    // SDL_Surface* cactusSurface = IMG_Load("customize/textures/blocks/cactus.png");


        blockTextures[0].texture = nullptr;
        blockTextures[1].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, placeholderSurface);
        blockTextures[2].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, bricksSurface);
        blockTextures[3].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, stoneSurface);
        blockTextures[4].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, woodSurface);
        blockTextures[5].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, logsSurface);
        blockTextures[6].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, grassSurface);
        blockTextures[7].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, dirtSurface);

        blockTextures[8].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, sandSurface);
        blockTextures[9].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, oreSurface);
        blockTextures[10].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, glassSurface);
        blockTextures[11].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, waterSurface);
        blockTextures[12].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, lavaSurface);
        blockTextures[13].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, woolSurface);
        blockTextures[14].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, leavesSurface);
        blockTextures[15].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, placeholderSurface); //TODO

        SDL_FreeSurface(placeholderSurface);
        SDL_FreeSurface(bricksSurface);
        SDL_FreeSurface(stoneSurface);
        SDL_FreeSurface(woodSurface);
        SDL_FreeSurface(logsSurface);
        SDL_FreeSurface(grassSurface);
        SDL_FreeSurface(dirtSurface);

        SDL_FreeSurface(sandSurface);
        SDL_FreeSurface(oreSurface);
        SDL_FreeSurface(glassSurface);
        SDL_FreeSurface(waterSurface);
        SDL_FreeSurface(lavaSurface);
        SDL_FreeSurface(woolSurface);
        SDL_FreeSurface(leavesSurface);
        // SDL_FreeSurface(cactusSurface); // TODO
    }
}