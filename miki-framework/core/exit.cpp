#include <iostream>
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

#include "mf/core.h"

namespace core {
    void Exit(MF_Window &window) {
        /**
         * @brief Clean up RAM before exiting game
         * @param window Window to clean up from
         */

        std::cout << "[MF] Exiting engine" << std::endl;
        
        IMG_Quit();
        TTF_Quit();
        SDL_DestroyRenderer(window.renderer);
        SDL_DestroyWindow(window.window);
        SDL_Quit();
    }
}