#include <SDL.h>

#include "mf/core.h"
#include "mf/graphics.h"

namespace draw {
    void DrawLine(SDL_Renderer* renderer, MF_Pixel start, MF_Pixel end, int thickness, MF_Color color) {
        /**
         * @brief Draws line
         * @param start Start point
         * @param end End point
         */

        draw::SetDrawColor(renderer, color);
        
        // Way less performance intensive to draw normal line than thick line
        if(thickness == 1)
            SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
        else {
            for (int i = -thickness / 2; i <= thickness / 2; ++i) {
                SDL_RenderDrawLine(renderer, start.x + i, start.y, end.x + i, end.y); // Offset in x-direction
                SDL_RenderDrawLine(renderer, start.x, start.y + i, end.x, end.y + i); // Offset in y-direction
            }
        }
    }
}
