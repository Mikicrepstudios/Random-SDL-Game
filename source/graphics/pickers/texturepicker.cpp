#include <string>
#include "SDL2/SDL.h"

#include "draw.h"
#include "settings.h"
#include "textures.h"

namespace textures {
	int PickerEvent(game::SDL_Settings sdlSettings, game::Settings &settings) {
        // This will run on mouse click
		int startposw = sdlSettings.width / 2 - 500;
		int startposh = sdlSettings.height / 2 - 200;
		int curTexture = 1;

		for(int y = 1; y <= 2; y++) {
			for(int x = 1; x <= 8; x++) {
				SDL_Rect curRect = {startposw + (100 * x), startposh + (100 * y), 100, 100};

                // Disable picker
				settings.texturePicker = false;

				if (sdlSettings.mouseX >= curRect.x && sdlSettings.mouseX <= curRect.x + curRect.w &&
				sdlSettings.mouseY >= curRect.y && sdlSettings.mouseY <= curRect.y + curRect.h)
					return curTexture - 1; // Textures dont start from 1

				curTexture++;
			}
		}

		// Handle clicking outside
		settings.texturePicker = false;
		return settings.blockTextureId;
	}
	void PickerOverlay(game::SDL_Settings sdlSettings, textures::BlockTextures blockTextures[32]) {
		SDL_Color textColor = {255, 255, 255};
        int curTexture = 0;

		// Draw bg
		SDL_Rect bgRect = {sdlSettings.width / 2 - 425, sdlSettings.height / 2 - 150, 850, 275};
		draw::DrawRect(sdlSettings.renderer, bgRect, 2);

		int startposw = sdlSettings.width / 2 - 500;
		int startposh = sdlSettings.height / 2 - 200;

		// Draw text
		SDL_Rect titleTextRect = {sdlSettings.width / 2 - 100, sdlSettings.height / 2 - 150, 200, 50};


		draw::DrawText(sdlSettings.renderer, sdlSettings.font, titleTextRect, "Texture Picker", textColor);

		// Draw grid
		for(int y = 1; y <= 2; y++) {
			for(int x = 1; x <= 8; x++) {
				SDL_Rect curRect = {startposw + (100 * x), startposh + (100 * y), 100, 100};
				draw::DrawTextureRect(sdlSettings.renderer, curRect, blockTextures[curTexture].texture);

				curTexture++;
			}
		}
	}
}
