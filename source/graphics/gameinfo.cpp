#include "SDL2/SDL.h"
#include <SDL_ttf.h>
#include <string>

#include "draw.h"
#include "settings.h"

std::string gameInfoTexts[16] = {};

namespace hud {
	void GameInfo(game::SDL_Settings sdlSettings, game::Settings settings, game::Camera cam, game::Player player) {
		SDL_Color color = {};
		// If background is white then set text color to black otherwise its white
		if(settings.bgColor == 32)
			color = sdlSettings.altTextColor;
		else
			color = sdlSettings.textColor;
		
		// Empty string mean skipping line
		gameInfoTexts[0] = "Width: " + std::to_string(sdlSettings.width);
		gameInfoTexts[1] = "Height: " + std::to_string(sdlSettings.height);
		gameInfoTexts[2] = "Fps: " + std::to_string(sdlSettings.fps);
		//gameInfoTexts[3] = "";

		gameInfoTexts[4] = "Block color: " + std::to_string(settings.blockColor);
		gameInfoTexts[5] = "BG color: " + std::to_string(settings.bgColor);
		//gameInfoTexts[6] = "";

		gameInfoTexts[7] = "Cam X: " + std::to_string(abs(cam.offSetX));
		gameInfoTexts[8] = "Cam Y: " + std::to_string(abs(cam.offSetY));
		gameInfoTexts[9] = "Camscale: " + std::to_string(cam.scale);
		//gameInfoTexts[10] = "";

		gameInfoTexts[11] = "Player X: " + std::to_string(player.x);
		gameInfoTexts[12] = "Player Y: " + std::to_string(player.y);
		gameInfoTexts[13] = "Player color: " + std::to_string(settings.playerColor);

		// Draw gameInfo hud
		for(int i = 0; i <= 13; i++) {
			SDL_Rect curRect = {0, i * 20, 100, 20};
			draw::DrawText(sdlSettings.renderer, sdlSettings.font, curRect, gameInfoTexts[i].c_str(), color);
		}
	}
}
