#include "SDL2/SDL.h"

#include "graphics.h"

namespace player {
    void colorPickerEvent(int mouseX, int mouseY, int width, int height, int &color) {
        int startposw = width / 2 - 500;
        int startposh = height / 2 - 300;
        int curColor = 1;

        for(int y = 1; y <= 4; y++) {
            for(int x = 1; x <= 8; x++) {
                SDL_Rect curRect = {startposw + (100 * x), startposh + (100 * y), 100, 100};

                if (mouseX >= curRect.x && mouseX <= curRect.x + curRect.w &&
                    mouseY >= curRect.y && mouseY <= curRect.y + curRect.h)
                    color = curColor;

                curColor++;
            }
        }
    }
}

namespace overlay {
    void colorPicker(SDL_Renderer* renderer, int width, int height) {
        int colorR, colorG, colorB = 0;
        int curColor = 1;

        // Draw bg
        SDL_Rect bgRect = {width / 2 - 425, height / 2 - 225, 850, 450};
        SDL_Color textColor = {255, 255, 255};
        graphics::GetColor(2, colorR, colorG, colorB);
        SDL_SetRenderDrawColor(renderer, colorR, colorG, colorB, 255);
        SDL_RenderFillRect(renderer, &bgRect);

        int startposw = width / 2 - 500;
        int startposh = height / 2 - 300;

        // Draw grid
        for(int y = 1; y <= 4; y++) {
            for(int x = 1; x <= 8; x++) {
                SDL_Rect curRect = {startposw + (100 * x), startposh + (100 * y), 100, 100};
                graphics::GetColor(curColor, colorR, colorG, colorB);
                SDL_SetRenderDrawColor(renderer, colorR, colorG, colorB, 255);
                SDL_RenderFillRect(renderer, &curRect);

                curColor++;
            }
        }
    }
}
