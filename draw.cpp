#include "SDL2/SDL.h"
#include <SDL_ttf.h>

#include "graphics.h"

int colorR, colorG, colorB = 0;

namespace draw {
    void DrawButton(SDL_Renderer* renderer, SDL_Rect rect, int color, int colorHover, int mouseX, int mouseY) {
        if (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
            mouseY >= rect.y && mouseY <= rect.y + rect.h)
            graphics::GetColor(colorHover, colorR, colorG, colorB);
        else
            graphics::GetColor(color, colorR, colorG, colorB);
            SDL_SetRenderDrawColor(renderer, colorR, colorG, colorB, 255);
            SDL_RenderFillRect(renderer, &rect);
    }
    void DrawPreview(SDL_Renderer* renderer, SDL_Rect rect, int color) {
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
