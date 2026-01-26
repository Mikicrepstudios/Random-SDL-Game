#include <iostream>
#include "SDL.h"

#include "mf/core.h"
#include "mf/colors.h"
#include "mf/graphics.h"
#include "mf/logic.h"

namespace draw {
	void DrawInputRect(core::MF_Window &window, SDL_Rect rect, MF_Color color, std::string &inputText) {
		/**
		 * @brief Draws input field where you can write text
		 * @param renderer Window renderer
		 * @param rect Cords of button
		 * @param color Color of drawn element (MF_Color)
		 * @param mouse Window mouse
		 */

		// If mouse hovers input, then darken it, if not then dont darken
		if (logic::IsMouseTouching(window.mouse, rect)) {
			color.r *= .65;
			color.g *= .65;
			color.b *= .65;
			SDL_SetRenderDrawColor(window.renderer, color.r, color.g, color.b, 255);
			SDL_RenderFillRect(window.renderer, &rect);
		}
		else
			SDL_SetRenderDrawColor(window.renderer, color.r, color.g, color.b, 255);
		SDL_RenderFillRect(window.renderer, &rect);

		if(inputText.length() != 0) {
			int fontRectWidth = inputText.length() * (rect.w / 10);
			if(fontRectWidth > rect.w) fontRectWidth = rect.w;
			
			SDL_Rect fontRect = {rect.x, rect.y, fontRectWidth, rect.h};
			MF_Color fontColor = {};

			if(color.r > 117 && color.g > 117 && color.b > 117) fontColor = colors::black;
			else fontColor = colors::white;

			draw::DrawText(window.renderer, window.font, fontRect, inputText.c_str(), fontColor);
		}

        if(window.mouse.isDown && logic::IsMouseTouching(window.mouse, rect)) {
            window.typingVariable = &inputText;
			if(!window.isTypingActive) {
				SDL_StartTextInput();
			}
            window.isTypingActive = true;
        }
	}
}