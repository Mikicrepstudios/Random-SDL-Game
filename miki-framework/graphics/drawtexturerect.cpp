#include <iostream>
#include "SDL.h"

#include "mf/colors.h"

namespace draw {
	void DrawTextureRect(SDL_Renderer* renderer, SDL_Rect rect, SDL_Texture* texture) {
		/**
		 * @brief Draws simple rectangle with specified texture
		 * @param renderer Window renderer
		 * @param rect Cords of rectangle
		 * @param texture Texture for the rect, must be SDL_Texture*
		 */

		if(texture == NULL) {std::cout << "[MF] Error using texture, did you load it correctly?" << std::endl; return;}
		SDL_RenderCopy(renderer, texture, NULL, &rect);
	}
}