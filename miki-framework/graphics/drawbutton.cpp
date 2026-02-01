#include "SDL.h"

#include "mf/core.h"
#include "mf/colors.h"
#include "mf/logic.h"

namespace draw {
	void DrawButton(SDL_Renderer* renderer, SDL_Rect rect, MF_Color color, core::MF_Window::Mouse mouse) {
		/**
		 * @brief Draws button which gets darker on mouse hover
		 * @param renderer Window renderer
		 * @param rect Cords of button
		 * @param color Color of drawn element (MF_Color)
		 * @param mouse Window mouse
		 */

		// If mouse hovers button then darken it, if not then dont darken
		if (logic::IsMouseTouching(mouse, rect)) {
			color.r *= .65;
			color.g *= .65;
			color.b *= .65;
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
			SDL_RenderFillRect(renderer, &rect);
		}
		else
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
		SDL_RenderFillRect(renderer, &rect);
	}
}