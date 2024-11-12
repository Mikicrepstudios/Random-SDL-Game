#include <string>
#include "SDL.h"
#include "SDL_image.h"

#include "mf/core.h"

namespace core {
    SDL_Texture* LoadImg(SDL_Renderer* renderer, std::string path) {
        /**
         * @brief This function loads image and returns it as SDL_Texture*
         * @param renderer Renderer of your window
         * @param path Path to image (recommended to use / for paths)
         */
        
        SDL_Surface* tmpSurface = IMG_Load(path.c_str());
        SDL_Texture* tmpTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);

        return tmpTexture;
    }
}