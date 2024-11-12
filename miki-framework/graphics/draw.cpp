#include <iostream>
#include "SDL2/SDL.h"
#include <SDL_ttf.h>

#include "mf/core.h"
#include "mf/colors.h"

namespace draw {
	void DrawButton(SDL_Renderer* renderer, SDL_Rect rect, MF_Color color, int mouseX, int mouseY) {
		/**
		 * @brief Draws button which gets darker on mouse hover
		 * @param renderer Window renderer
		 * @param rect Cords of button
		 * @param color Color of drawn element (MF_Color)
		 * @param mouseX X position of mouse
		 * @param mouseY Y position of mouse
		 */

		// If mouse hovers button then darken it, if not then dont darken
		if (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
			mouseY >= rect.y && mouseY <= rect.y + rect.h) {
			color.r *= .25;
			color.g *= .25;
			color.b *= .25;
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
			SDL_RenderFillRect(renderer, &rect);
		}
		else
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
		SDL_RenderFillRect(renderer, &rect);
	}

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

	void DrawTextureRect(SDL_Renderer* renderer, SDL_Rect rect, SDL_Texture* texture) {
		/**
		 * @brief Draws simple rectangle with specified texture
		 * @param renderer Window renderer
		 * @param rect Cords of rectangle
		 * @param texture Texture for the rect, must be SDL_Texture*
		 */

		SDL_RenderCopy(renderer, texture, NULL, &rect);
	}

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
