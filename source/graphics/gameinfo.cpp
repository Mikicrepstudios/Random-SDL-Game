#include "SDL2/SDL.h"
#include <SDL_ttf.h>
#include <string>

#include "draw.h"
#include "settings.h"

namespace hud {
	void GameInfo(game::SDL_Settings sdlSettings, game::Settings settings, game::Camera cam, game::Player player) {
		SDL_Color color = {};
		// If background is white then set text color to black otherwise its white
		if(settings.bgColor == 32)
			color = sdlSettings.altTextColor;
		else
			color = sdlSettings.textColor;
		
		std::string gameInfoTexts[16] = {
			"Width: " + std::to_string(sdlSettings.width),
			"Height: " + std::to_string(sdlSettings.height),
			"Fps: " + std::to_string(sdlSettings.fps),
			"", // Empty string for skipped line
			"Cur preset: " + std::to_string(settings.curPreset + 1),
			"Block color: " + std::to_string(settings.blockColor),
			"BG color: " + std::to_string(settings.bgColor),
			"", // Empty string for skipped line
			"Cam X: " + std::to_string(abs(cam.offSetX)),
			"Cam Y: " + std::to_string(abs(cam.offSetY)),
			"Camscale: " + std::to_string(cam.scale),
			"", // Empty string for skipped line
			"Player X: " + std::to_string(player.x),
			"Player Y: " + std::to_string(player.y),
			"Player color: " + std::to_string(settings.playerColor)
		};
		// Draw gameInfo hud
		for(int i = 0; i <= 13; i++) {
			SDL_Rect curRect = {0, i * 20, 100, 20};
			draw::DrawText(sdlSettings.renderer, sdlSettings.font, curRect, gameInfoTexts[i].c_str(), color);
		}
	}
}
