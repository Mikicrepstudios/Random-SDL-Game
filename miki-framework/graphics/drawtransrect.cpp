#include "SDL.h"

#include "mf/colors.h"

namespace draw {
	void DrawTransRect(SDL_Renderer* renderer, SDL_Rect rect, MF_Color color, int colorA) {
		/**
		 * @brief Draws simple transparent rectangle
		 * @param renderer Window renderer
		 * @param rect Cords of rectangle
		 * @param color Color of drawn element (MF_Color)
		 * @param colorA Opacity of rect (255 - visible, 0 - transparent)
		 */

		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, colorA);
		SDL_RenderFillRect(renderer, &rect);
	}
}