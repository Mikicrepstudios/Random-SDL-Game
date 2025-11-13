#pragma once

#include "SDL.h"

#include "mf/core.h"

/**
 * @brief This namespace contains functions with useful utilities
 */
namespace logic {
    // Random
    int GenRanNum(int min, int max);
    bool chancegen(int chance);
    
    // Collisions
    bool AreRectsTouching(const SDL_Rect& rect1, SDL_Rect& rect2);
    bool IsMouseTouching(core::MF_Window::Mouse mouse, SDL_Rect& rect);
}
