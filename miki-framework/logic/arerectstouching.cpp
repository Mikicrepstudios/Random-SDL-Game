#include "SDL2/SDL.h"

namespace logic {
    bool AreRectsTouching(const SDL_Rect& rect1, SDL_Rect& rect2) {
        /**
         * @brief This function compares 2 rectangles, and returns 1 if they are touching
         * @param rect1 First rect
         * @param rect2 Second rect
         */

        return (rect1.x < rect2.x + rect2.w &&
                rect1.x + rect1.w > rect2.x &&
                rect1.y < rect2.y + rect2.h &&
                rect1.y + rect1.h > rect2.y);
    }
}