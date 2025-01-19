#include <cmath>
#include <SDL.h>

#include "mf/graphics.h"

namespace draw {
    void DrawCircleBorder(SDL_Renderer* renderer, SDL_Rect &rect, MF_Color color) {
        /**
         * @brief This function draws a circle border
         * @param renderer Window renderer
         * @param rect Where to draw
         * @param color Color of circle
         */
        
        int centerX = rect.x + rect.w / 2;
        int centerY = rect.y + rect.h / 2;
        int radius = (rect.w < rect.h ? rect.w : rect.h) / 2;

        int x = 0;
        int y = radius;
        int decision = 1 - radius;

        draw::SetDrawColor(renderer, color);

        while (x <= y) {
            // Draw the 8 symmetric points of the circle
            DrawPixel(renderer, centerX + x, centerY + y);
            DrawPixel(renderer, centerX - x, centerY + y);
            DrawPixel(renderer, centerX + x, centerY - y);
            DrawPixel(renderer, centerX - x, centerY - y);
            DrawPixel(renderer, centerX + y, centerY + x);
            DrawPixel(renderer, centerX - y, centerY + x);
            DrawPixel(renderer, centerX + y, centerY - x);
            DrawPixel(renderer, centerX - y, centerY - x);

            if (decision < 0) {
                decision += 2 * x + 3;
            } else {
                decision += 2 * (x - y) + 5;
                y--;
            }
            x++;
        }
    }
}