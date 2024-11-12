#include "SDL2/SDL.h"

namespace logic {
    bool IsMouseTouching(int mouseX, int mouseY, SDL_Rect& rect) {
        /**
         * @brief This function compares mouse position and rect, and returns 1 if they are touching
         * @param mouseX X axis of mouse
         * @param mouseY Y axis of mouse
         * @param rect Second rect
         */

        SDL_Rect mouseRect = {mouseX, mouseY, 1, 1};

        return (mouseRect.x < rect.x + rect.w &&
                mouseRect.x + mouseRect.w > rect.x &&
                mouseRect.y < rect.y + rect.h &&
                mouseRect.y + mouseRect.h > rect.y);
    }
}