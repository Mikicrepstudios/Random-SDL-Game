#include <string>
#include "SDL2/SDL.h"

#include "addional.h"
#include "settings.h"

namespace player {
	int ColorPickerEvent(game::SDL_Settings sdlSettings, game::Settings &settings) {
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
				settings.colorPicker = false;

				curColor++;
			}
		}
		// If clicked outside then return same value
		switch (settings.colorPickerId) {
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
	void ColorPicker(game::SDL_Settings sdlSettings, game::Settings settings) {
		SDL_Color textColor = {255, 255, 255};
		std::string text = "";
		int curColor = 1;

		// Draw bg
		SDL_Rect bgRect = {sdlSettings.width / 2 - 425, sdlSettings.height / 2 - 250, 850, 475};
		draw::DrawRect(sdlSettings.renderer, bgRect, 2);

		int startposw = sdlSettings.width / 2 - 500;
		int startposh = sdlSettings.height / 2 - 300;

		// Draw text
		SDL_Rect titleTextRect = {sdlSettings.width / 2 - 100, sdlSettings.height / 2 - 250, 200, 50};

		switch(settings.colorPickerId) {
			case 1:
				text = "Block Color";
				break;
			case 2:
				text = "BG Color";
				break;
			case 3:
				text = "Player Color";
				break;
		}

		draw::DrawText(sdlSettings.renderer, sdlSettings.font, titleTextRect, text.c_str(), textColor);

		// Draw grid
		for(int y = 1; y <= 4; y++) {
			for(int x = 1; x <= 8; x++) {
				SDL_Rect curRect = {startposw + (100 * x), startposh + (100 * y), 100, 100};
				draw::DrawRect(sdlSettings.renderer, curRect, curColor);

				curColor++;
			}
		}
	}
}
