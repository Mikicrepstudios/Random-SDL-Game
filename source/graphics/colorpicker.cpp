#include "SDL2/SDL.h"

#include "addional.h"
#include "settings.h"

namespace player {
	int ColorPickerEvent(game::SDL_Settings sdlSettings, game::Settings &settings, int pickerId) {
		int startposw = sdlSettings.width / 2 - 500;
		int startposh = sdlSettings.height / 2 - 300;
		int curColor = 1;

		for(int y = 1; y <= 4; y++) {
			for(int x = 1; x <= 8; x++) {
				SDL_Rect curRect = {startposw + (100 * x), startposh + (100 * y), 100, 100};

				if (sdlSettings.mouseX >= curRect.x && sdlSettings.mouseX <= curRect.x + curRect.w &&
				sdlSettings.mouseY >= curRect.y && sdlSettings.mouseY <= curRect.y + curRect.h)
					return curColor;
				
				// Disable color picker based on pickerId
				switch (pickerId) {
				case 1:
					settings.colorPick = false;
					break;
				case 2:
					settings.bgColorPick = false;
					break;
				case 3:
					settings.playerColorPick = false;
					break;
				}

				curColor++;
			}
		}
		// If clicked outside then return same value
		switch (pickerId) {
			case 1:
				return settings.blockColor;
				break;
			case 2:
				return settings.bgColor;
				break;
			case 3:
				return settings.playerColor;
				break;
		}
		return -69; // Nice
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
