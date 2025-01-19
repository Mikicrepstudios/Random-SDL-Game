#include <iostream>
#include <string>
#include "SDL.h"

#include "mf/core.h"
#include "mf/colors.h"
#include "mf/files.h"
#include "mf/graphics.h"
#include "mf/logic.h"

namespace core {
    void miki(core::MF_Window window) {
        /**
         * @brief This will reamain undocumented, figure it yourself ;)
         */

        // Draw text and underline
        draw::DrawText(window.renderer, window.font, {window.width / 2 - 400, 0, 800, 50}, "Mikicrep Framework is working", colors::white);
        MF_Pixel TextUnderLineS = {window.width / 2 - 400, 50};
        MF_Pixel TextUnderLineE = {window.width / 2 + 400, 50};
        draw::SetDrawColor(window.renderer, colors::red);
        draw::DrawLine(window.renderer, TextUnderLineS, TextUnderLineE);
    
        // RGB rects
        SDL_Rect rect1 = {50, 50, 50, 50};
        SDL_Rect rect2 = {75, 50, 50, 50};
        SDL_Rect rect3 = {62, 75, 50, 50};

        draw::DrawTransRect(window.renderer, rect1, colors::colorID[26], 255); // Color ID for red
        draw::DrawTransRect(window.renderer, rect2, colors::indigo, 150);
        draw::DrawTransRect(window.renderer, rect3, colors::grassgreen, 150);

        if(logic::IsMouseTouching(window.mouse.x, window.mouse.y, rect3))
            draw::DrawRect(window.renderer, {window.width - 50, window.height - 50, 50, 50}, colors::grassgreen);
        else if(logic::IsMouseTouching(window.mouse.x, window.mouse.y, rect2))
            draw::DrawRect(window.renderer, {window.width - 50, window.height - 50, 50, 50}, colors::indigo);
        else if(logic::IsMouseTouching(window.mouse.x, window.mouse.y, rect1))
            draw::DrawRect(window.renderer, {window.width - 50, window.height - 50, 50, 50}, colors::colorID[26]); // Color ID for red

        // Shaking rect
        SDL_Rect shakingRect = {150, 75, 50, 50};
        draw::DrawShakingRect(window.renderer, shakingRect, colors::gray, 5);

        // Draw logo
        SDL_Rect bordercircle = {window.width - 125, 25, 100, 100};
        draw::DrawCircleBorder(window.renderer, bordercircle, colors::red);
    }
}