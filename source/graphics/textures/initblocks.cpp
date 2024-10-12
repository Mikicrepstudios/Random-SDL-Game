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
	    SDL_Surface* grassSurface = IMG_Load("customize/textures/blocks/grass.png");
	    /*SDL_Surface* Surface = IMG_Load("customize/textures/blocks/.png");
	    SDL_Surface* Surface = IMG_Load("customize/textures/blocks/.png");
	    SDL_Surface* Surface = IMG_Load("customize/textures/blocks/.png");
	    SDL_Surface* Surface = IMG_Load("customize/textures/blocks/.png");
	    SDL_Surface* Surface = IMG_Load("customize/textures/blocks/.png");

	    SDL_Surface* Surface = IMG_Load("customize/textures/blocks/.png");
	    SDL_Surface* Surface = IMG_Load("customize/textures/blocks/.png");
	    SDL_Surface* Surface = IMG_Load("customize/textures/blocks/.png");
	    SDL_Surface* Surface = IMG_Load("customize/textures/blocks/.png");
	    SDL_Surface* Surface = IMG_Load("customize/textures/blocks/.png");
	    SDL_Surface* Surface = IMG_Load("customize/textures/blocks/.png");
	    SDL_Surface* Surface = IMG_Load("customize/textures/blocks/.png");
	    SDL_Surface* Surface = IMG_Load("customize/textures/blocks/.png");*/


        blockTextures[0].texture = nullptr;
        blockTextures[1].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, placeholderSurface);
        blockTextures[2].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, bricksSurface);
        blockTextures[3].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, stoneSurface);
        // Temponary init theese before textures are actually done
        blockTextures[4].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, placeholderSurface);
        blockTextures[5].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, placeholderSurface);
        blockTextures[6].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, grassSurface);
        blockTextures[7].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, placeholderSurface);

        blockTextures[8].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, placeholderSurface);
        blockTextures[9].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, placeholderSurface);
        blockTextures[10].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, placeholderSurface);
        blockTextures[11].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, placeholderSurface);
        blockTextures[12].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, placeholderSurface);
        blockTextures[13].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, placeholderSurface);
        blockTextures[14].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, placeholderSurface);
        blockTextures[15].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, placeholderSurface);

        SDL_FreeSurface(placeholderSurface);
        SDL_FreeSurface(bricksSurface);
        /*SDL_FreeSurface(Surface);
        SDL_FreeSurface(Surface);
        SDL_FreeSurface(Surface);
        SDL_FreeSurface(Surface);
        SDL_FreeSurface(Surface);
        SDL_FreeSurface(Surface);

        SDL_FreeSurface(Surface);
        SDL_FreeSurface(Surface);
        SDL_FreeSurface(Surface);
        SDL_FreeSurface(Surface);
        SDL_FreeSurface(Surface);
        SDL_FreeSurface(Surface);
        SDL_FreeSurface(Surface);
        SDL_FreeSurface(Surface);*/
    }
}