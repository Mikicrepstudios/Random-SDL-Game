#include <SDL.h>

#include <mf/graphics.h>

namespace draw {
    void DrawCircle(SDL_Renderer* renderer, SDL_Rect &rect, MF_Color color) {
        /**
         * @param Draws This function draws circle
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
            // Draw horizontal lines to fill the circle
            DrawLine(renderer, {centerX - x, centerY + y}, {centerX + x, centerY + y}, 1, color);
            DrawLine(renderer, {centerX - x, centerY - y}, {centerX + x, centerY - y}, 1, color);
            DrawLine(renderer, {centerX - y, centerY + x}, {centerX + y, centerY + x}, 1, color);
            DrawLine(renderer, {centerX - y, centerY - x}, {centerX + y, centerY - x}, 1, color);

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