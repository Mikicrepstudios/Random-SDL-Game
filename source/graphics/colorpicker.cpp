#include "SDL2/SDL.h"

#include "addional.h"

namespace player {
	void ColorPickerEvent(bool &picker, int mouseX, int mouseY, int width, int height, int &color) {
		int startposw = width / 2 - 500;
		int startposh = height / 2 - 300;
		int curColor = 1;

		for(int y = 1; y <= 4; y++) {
			for(int x = 1; x <= 8; x++) {
				SDL_Rect curRect = {startposw + (100 * x), startposh + (100 * y), 100, 100};

				if (mouseX >= curRect.x && mouseX <= curRect.x + curRect.w &&
				mouseY >= curRect.y && mouseY <= curRect.y + curRect.h)
				color = curColor;
				picker = false;

				curColor++;
			}
		}
	}
}

namespace overlay {
	void ColorPicker(SDL_Renderer* renderer, TTF_Font* font, const char* text, int width, int height) {
		SDL_Color textColor = {255, 255, 255};
		int curColor = 1;

		// Draw bg
		SDL_Rect bgRect = {width / 2 - 425, height / 2 - 250, 850, 475};
		draw::DrawRect(renderer, bgRect, 2);

		int startposw = width / 2 - 500;
		int startposh = height / 2 - 300;

		// Draw text
		SDL_Rect titleTextRect = {width / 2 - 100, height / 2 - 250, 200, 50};
		draw::DrawText(renderer, font, titleTextRect, text, textColor);

		// Draw grid
		for(int y = 1; y <= 4; y++) {
			for(int x = 1; x <= 8; x++) {
				SDL_Rect curRect = {startposw + (100 * x), startposh + (100 * y), 100, 100};
				draw::DrawRect(renderer, curRect, curColor);

				curColor++;
			}
		}
	}
}
