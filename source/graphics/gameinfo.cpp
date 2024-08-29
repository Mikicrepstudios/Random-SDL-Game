#include "SDL2/SDL.h"
#include <SDL_ttf.h>
#include <string>

#include "addional.h"
#include "settings.h"

namespace player {
	void InitGameInfoTexts(game::SDL_Settings sdlSettings, game::Settings settings, game::Camera camera, game::Player player, std::string gameInfoTexts[16]) {
		// Empty string mean skipping line
		gameInfoTexts[0] = "Width: " + std::to_string(sdlSettings.width);
		gameInfoTexts[1] = "Height: " + std::to_string(sdlSettings.height);
		gameInfoTexts[2] = "Fps: " + std::to_string(sdlSettings.fps);
		//gameInfoTexts[3] = "";

		gameInfoTexts[4] = "Block color: " + std::to_string(settings.blockColor);
		gameInfoTexts[5] = "BG color: " + std::to_string(settings.bgColor);
		//gameInfoTexts[6] = "";

		gameInfoTexts[7] = "Cam X: " + std::to_string(abs(camera.offSetX));
		gameInfoTexts[8] = "Cam Y: " + std::to_string(abs(camera.offSetY));
		gameInfoTexts[9] = "Camscale: " + std::to_string(camera.scale);
		//gameInfoTexts[10] = "";

		gameInfoTexts[11] = "Player X: " + std::to_string(player.x);
		gameInfoTexts[12] = "Player Y: " + std::to_string(player.y);
		gameInfoTexts[13] = "Player color: " + std::to_string(settings.playerColor);
	}

	void GameInfo(game::SDL_Settings sdlSettings, SDL_Color color, std::string gameInfoTexts[16]) {
		// Draw gameInfo hud
		for(int i = 0; i <= 13; i++) {
			SDL_Rect curRect = {0, i * 20, 100, 20};
			draw::DrawText(sdlSettings.renderer, sdlSettings.font, curRect, gameInfoTexts[i].c_str(), color);
		}
	}
}
