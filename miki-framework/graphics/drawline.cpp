#include <SDL.h>

#include <mf/core.h>

namespace draw {
    void DrawLine(SDL_Renderer* renderer, MF_Pixel start, MF_Pixel end) {
        /**
         * @brief Draws line
         * @param start Start point
         * @param end End point
         */
        
        SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
    }
}