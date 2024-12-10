#include "SDL.h"

/**
 * @brief This namespace contains functions with useful utilities
 */

namespace logic {
    // Random
    int GenRanNum(int min, int max);
    bool chancegen(int chance);
    
    // Collisions
    bool AreRectsTouching(const SDL_Rect& rect1, SDL_Rect& rect2);
    bool IsMouseTouching(int mouseX, int mouseY, SDL_Rect& rect);
}
