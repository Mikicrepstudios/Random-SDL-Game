#pragma once

#include <SDL_ttf.h>

namespace draw {
	extern void DrawButton(SDL_Renderer* renderer, SDL_Rect rect, int color, int mouseX, int mouseY);
	extern void DrawRect(SDL_Renderer* renderer, SDL_Rect selRect, int color);
	extern void DrawText(SDL_Renderer* renderer, TTF_Font* font, SDL_Rect rect, const char* text, SDL_Color color);
}
