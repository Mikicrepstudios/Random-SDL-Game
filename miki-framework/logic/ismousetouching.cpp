#include "SDL2/SDL.h"

#include "mf/core.h"

namespace logic {
    bool IsMouseTouching(core::MF_Window::Mouse mouse, SDL_Rect& rect) {
        /**
         * @brief This function compares mouse position and rect, and returns 1 if they are touching
         * @param mouse Window mouse
         * @param rect Second rect
         */

        SDL_Rect mouseRect = {mouse.x, mouse.y, 1, 1};

        return (mouseRect.x < rect.x + rect.w &&
                mouseRect.x + mouseRect.w > rect.x &&
                mouseRect.y < rect.y + rect.h &&
                mouseRect.y + mouseRect.h > rect.y);
    }
}