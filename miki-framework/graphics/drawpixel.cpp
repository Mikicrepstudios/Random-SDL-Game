#include <SDL.h>

namespace draw {
    void DrawPixel(SDL_Renderer* renderer, int x, int y) {
        /**
         * @brief Draws single pixel on desired position, must use before calling function draw::SetDrawColor()
         * @param x X position
         * @param y Y position
         */
        
        SDL_RenderDrawPoint(renderer, x, y);
    }
}