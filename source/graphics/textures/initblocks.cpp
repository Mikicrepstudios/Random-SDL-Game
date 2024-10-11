#include "SDL2/SDL.h"
#include <SDL_image.h>

#include "settings.h"
#include "textures.h"

#include <iostream>

namespace textures {
    void initBlocks(game::SDL_Settings &sdlSettings, BlockTextures blockTextures[32]) {
	    SDL_Surface* placeholderSurface = IMG_Load("customize/textures/blocks/placeholder.png");
        SDL_Surface* bricksSurface = IMG_Load("customize/textures/blocks/bricks.png");
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
	    SDL_Surface* Surface = IMG_Load("customize/textures/blocks/.png");
	    SDL_Surface* Surface = IMG_Load("customize/textures/blocks/.png");*/


        blockTextures[0].texture = nullptr;
        blockTextures[1].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, placeholderSurface);
        blockTextures[2].texture = SDL_CreateTextureFromSurface(sdlSettings.renderer, bricksSurface);
        /*SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(sdlSettings.renderer, );
        SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(sdlSettings.renderer, );
        SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(sdlSettings.renderer, );
        SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(sdlSettings.renderer, );
        SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(sdlSettings.renderer, );
        SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(sdlSettings.renderer, );

        SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(sdlSettings.renderer, );
        SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(sdlSettings.renderer, );
        SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(sdlSettings.renderer, );
        SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(sdlSettings.renderer, );
        SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(sdlSettings.renderer, );
        SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(sdlSettings.renderer, );
        SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(sdlSettings.renderer, );
        SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(sdlSettings.renderer, );*/


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