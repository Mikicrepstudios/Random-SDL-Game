#pragma once

namespace draw {
    extern void drawButton(SDL_Renderer* renderer, SDL_Rect rect, int color, int colorhover, int mousex, int mousey);
    extern void drawPreview(SDL_Renderer* renderer, SDL_Rect selrect, int color);
    extern void drawText(SDL_Renderer* renderer, TTF_Font* font, SDL_Rect rect, const char* text, SDL_Color color);
}
