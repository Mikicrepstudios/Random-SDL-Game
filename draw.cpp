#include "SDL2/SDL.h"
#include <SDL_ttf.h>

#include "graphics.h"

int colorr, colorg, colorb = 0;

namespace draw {
    void drawButton(SDL_Renderer* renderer, SDL_Rect rect, int color, int colorhover, int mousex, int mousey) {
        if (mousex >= rect.x && mousex <= rect.x + rect.w &&
            mousey >= rect.y && mousey <= rect.y + rect.h)
            graphics::getColor(colorhover, colorr, colorg, colorb);
        else
            graphics::getColor(color, colorr, colorg, colorb);
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_RenderFillRect(renderer, &rect);
    }
    void drawPreview(SDL_Renderer* renderer, SDL_Rect rect, int color) {
        graphics::getColor(color, colorr, colorg, colorb);
        SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
    void drawText(SDL_Renderer* renderer, TTF_Font* font, SDL_Rect rect, const char* text, SDL_Color color) {
        SDL_Surface* textsurface = TTF_RenderText_Solid(font, text, color);
        SDL_Texture* texttexture = SDL_CreateTextureFromSurface(renderer, textsurface);
        SDL_RenderCopy(renderer, texttexture, NULL, &rect);
    }
}
