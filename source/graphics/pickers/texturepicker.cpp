#include <string>
#include "SDL.h"

#include "mf/core.h"
#include "mf/colors.h"
#include "mf/graphics.h"
#include "mf/logic.h"

#include "settings.h"
#include "textures.h"

namespace textures {
	int PickerEvent(core::MF_Window &window, game::Settings &settings) {
		/**
		 * @brief This function checks when and which texture is selected from texture picker
		 * @param window Game window
		 * @param settings Game settings
		 */

        // This will run on mouse click
		int startposw = window.width / 2 - 500;
		int startposh = window.height / 2 - 200;
		int curTexture = 1;

		for(int y = 1; y <= 2; y++) {
			for(int x = 1; x <= 8; x++) {
				SDL_Rect curRect = {startposw + (100 * x), startposh + (100 * y), 100, 100};

                // Disable picker
				settings.texturePicker = false;

				if(logic::IsMouseTouching(window.mouse, curRect)) return curTexture - 1; // Textures dont start from 1

				curTexture++;
			}
		}

		// Handle clicking outside
		settings.texturePicker = false;
		return settings.blockTextureId;
	}
	void PickerOverlay(core::MF_Window &window, textures::BlockTextures blockTextures[32]) {
		/**
		 * @brief This function draws texture picker overlay
		 * @param window Game window
		 * @param blockTextures All block textures
		 */
		
        int curTexture = 0;

		// Draw bg
		SDL_Rect bgRect = {window.width / 2 - 425, window.height / 2 - 150, 850, 275};
		draw::DrawRect(window.renderer, bgRect, colors::gray);

		int startposw = window.width / 2 - 500;
		int startposh = window.height / 2 - 200;

		// Draw text
		SDL_Rect titleTextRect = {window.width / 2 - 100, window.height / 2 - 150, 200, 50};


		draw::DrawText(window.renderer, window.font, titleTextRect, "Texture Picker", colors::white);

		// Draw grid
		for(int y = 1; y <= 2; y++) {
			for(int x = 1; x <= 8; x++) {
				SDL_Rect curRect = {startposw + (100 * x), startposh + (100 * y), 100, 100};
				draw::DrawTextureRect(window.renderer, curRect, blockTextures[curTexture].texture);

				curTexture++;
			}
		}
	}
}
