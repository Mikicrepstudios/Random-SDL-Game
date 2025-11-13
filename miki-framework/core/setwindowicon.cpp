#include <iostream>
#include <string>
#include "SDL.h"
#include "SDL_image.h"

namespace core {
    void SetWindowIcon(SDL_Window* window, std::string path) {
        /**
         * @brief This sets window icon
         * @param window Window that will get its icon changed
         * @param path Path to icon
         */
        
        SDL_Surface* iconSurface = IMG_Load(path.c_str());
        if(iconSurface == NULL) {
            std::cout << "[MF] Error loading window icon: \"" << path << "\", did you enter correct path?" << std::endl;
            return;
        }
        SDL_SetWindowIcon(window, iconSurface);
        SDL_FreeSurface(iconSurface);
    }
}