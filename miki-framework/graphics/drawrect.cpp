#include "SDL.h"

#include "mf/colors.h"

namespace draw {
	void DrawRect(SDL_Renderer* renderer, SDL_Rect rect, MF_Color color) {
		/**
		 * @brief Draws simple rectangle
		 * @param renderer Window renderer
		 * @param rect Cords of rectangle
		 * @param color Color of drawn element (MF_Color)
		 */

		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
		SDL_RenderFillRect(renderer, &rect);
	}
}