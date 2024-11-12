#include <string>
#include "SDL2/SDL.h"

#include "mf/core.h"
#include "mf/colors.h"
#include "mf/graphics.h"
#include "mf/logic.h"

#include "settings.h"

namespace colorpicker {
	int Event(core::MF_Window &window, game::Settings &settings) {
        // This will run on mouse click
		int startposw = window.width / 2 - 500;
		int startposh = window.height / 2 - 300;
		int curColor = 1;

		for(int y = 1; y <= 4; y++) {
			for(int x = 1; x <= 8; x++) {
				SDL_Rect curRect = {startposw + (100 * x), startposh + (100 * y), 100, 100};

				// Disable color picker based on pickerId
				settings.colorPicker = false;

				if(logic::IsMouseTouching(window.mouseX, window.mouseY, curRect)) return curColor;

				curColor++;
			}
		}

		// Handle clicking outside
		settings.colorPicker = false;

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
	void Overlay(core::MF_Window &window, game::Settings settings) {
		SDL_Color textColor = {255, 255, 255};
		std::string text = "";
		int curColor = 1;

		// Draw bg
		SDL_Rect bgRect = {window.width / 2 - 425, window.height / 2 - 250, 850, 475};
		draw::DrawRect(window.renderer, bgRect, colors::gray);

		int startposw = window.width / 2 - 500;
		int startposh = window.height / 2 - 300;

		// Draw text
		SDL_Rect titleTextRect = {window.width / 2 - 100, window.height / 2 - 250, 200, 50};

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

		draw::DrawText(window.renderer, window.font, titleTextRect, text.c_str(), colors::white);

		// Draw grid
		for(int y = 1; y <= 4; y++) {
			for(int x = 1; x <= 8; x++) {
				SDL_Rect curRect = {startposw + (100 * x), startposh + (100 * y), 100, 100};
				draw::DrawRect(window.renderer, curRect, colors::colorID[curColor]);

				curColor++;
			}
		}
	}
}
