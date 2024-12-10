#include "SDL.h"

#include "mf/colors.h"
#include "mf/logic.h"

namespace draw {
	void DrawShakingRect(SDL_Renderer* renderer, SDL_Rect rect, MF_Color color, int intensity) {
		/**
		 * @brief Draws rectangle that shakes every frame
		 * @param renderer Window renderer
		 * @param rect Cords of rectangle
		 * @param color Color of drawn element (MF_Color)
         * @param intensity How much rect will shake in all directions (in pixels)
		 */

        bool invertX = logic::GenRanNum(0, 1);
        bool invertY = logic::GenRanNum(0, 1);

        int amountX = logic::GenRanNum(0, intensity);
        int amountY = logic::GenRanNum(0, intensity);

        if(invertX) amountX = -amountX;
        if(invertY) amountY = -amountY;

        rect.x += amountX;
        rect.y += amountY;

		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
		SDL_RenderFillRect(renderer, &rect);
	}
}