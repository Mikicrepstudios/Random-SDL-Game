#include "SDL2/SDL.h"
#include <SDL_ttf.h>

#include "graphics.h"

int colorR, colorG, colorB = 0;

namespace draw {
	void DrawButton(SDL_Renderer* renderer, SDL_Rect rect, int color, int mouseX, int mouseY) {
		graphics::GetColor(color, colorR, colorG, colorB);
		if (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
			mouseY >= rect.y && mouseY <= rect.y + rect.h) {
			colorR -= colorR * .25;
			colorG -= colorG * .25;
			colorB -= colorB * .25;
			SDL_SetRenderDrawColor(renderer, colorR, colorG, colorB, 255);
			SDL_RenderFillRect(renderer, &rect);
		}
		else
			SDL_SetRenderDrawColor(renderer, colorR, colorG, colorB, 255);
		SDL_RenderFillRect(renderer, &rect);
	}
	void DrawRect(SDL_Renderer* renderer, SDL_Rect rect, int color) {
		graphics::GetColor(color, colorR, colorG, colorB);
		SDL_SetRenderDrawColor(renderer, colorR, colorG, colorB, 255);
		SDL_RenderFillRect(renderer, &rect);
	}
	void DrawText(SDL_Renderer* renderer, TTF_Font* font, SDL_Rect rect, const char* text, SDL_Color color) {
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		SDL_RenderCopy(renderer, textTexture, NULL, &rect);
	}
}
