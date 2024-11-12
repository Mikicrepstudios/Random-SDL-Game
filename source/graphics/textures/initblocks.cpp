#include "SDL2/SDL.h"
#include <SDL_image.h>

#include "mf/core.h"

#include "textures.h"

namespace textures {
    void initBlocks(core::MF_Window &window, BlockTextures blockTextures[32]) {
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
        blockTextures[1].texture = SDL_CreateTextureFromSurface(window.renderer, placeholderSurface);
        blockTextures[2].texture = SDL_CreateTextureFromSurface(window.renderer, bricksSurface);
        blockTextures[3].texture = SDL_CreateTextureFromSurface(window.renderer, stoneSurface);
        blockTextures[4].texture = SDL_CreateTextureFromSurface(window.renderer, woodSurface);
        blockTextures[5].texture = SDL_CreateTextureFromSurface(window.renderer, logsSurface);
        blockTextures[6].texture = SDL_CreateTextureFromSurface(window.renderer, grassSurface);
        blockTextures[7].texture = SDL_CreateTextureFromSurface(window.renderer, dirtSurface);

        blockTextures[8].texture = SDL_CreateTextureFromSurface(window.renderer, sandSurface);
        blockTextures[9].texture = SDL_CreateTextureFromSurface(window.renderer, oreSurface);
        blockTextures[10].texture = SDL_CreateTextureFromSurface(window.renderer, glassSurface);
        blockTextures[11].texture = SDL_CreateTextureFromSurface(window.renderer, waterSurface);
        blockTextures[12].texture = SDL_CreateTextureFromSurface(window.renderer, lavaSurface);
        blockTextures[13].texture = SDL_CreateTextureFromSurface(window.renderer, woolSurface);
        blockTextures[14].texture = SDL_CreateTextureFromSurface(window.renderer, leavesSurface);
        blockTextures[15].texture = SDL_CreateTextureFromSurface(window.renderer, placeholderSurface); //TODO

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