#include "SDL.h"
#include "SDL_ttf.h"

#include "mf/colors.h"

namespace draw {
	void DrawText(SDL_Renderer* renderer, TTF_Font* font, SDL_Rect rect, const char* text, MF_Color color) {
		/**
		 * @brief Draws text on screen
		 * @param renderer Window renderer
		 * @param rect Cords of text box
		 * @param text Text that will be drawn, must be char*, you can convert std::string to it using .c_str()
		 * @param textColor Color of text, must be SDL_Color, you can use colors::text::white, replace white with your wanted color
		 */
		
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, {color.r, color.g, color.b});
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		SDL_RenderCopy(renderer, textTexture, NULL, &rect);
		SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
	}
}