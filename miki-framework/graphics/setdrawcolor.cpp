#include "SDL.h"

#include "mf/colors.h"

namespace draw {
	void SetDrawColor(SDL_Renderer* renderer, MF_Color color) {
        /**
         * @brief If you need to change your draw color for some reason, here you have it
         * @param rendered Window renderer
         * @param color Color which to be set as draw color
         */
        
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
	}
}
