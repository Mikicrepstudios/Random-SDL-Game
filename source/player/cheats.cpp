#include "SDL2/SDL.h"

#include "block.h"
#include "settings.h"

namespace cheats {
	void CamTp(game::SDL_Settings sdlSettings, game::Settings &settings, game::Camera &cam) {
		if (sdlSettings.event.type == SDL_MOUSEBUTTONDOWN) {
			cam.offSetX -= sdlSettings.curHoverX;
			cam.offSetY -= sdlSettings.curHoverY;

			settings.cheats = false;
			cam.highlight = false;
		}
	}
	void PlayerTp(game::SDL_Settings sdlSettings, game::Settings &settings, game::Map &map, game::Camera &cam, game::Player &player) {
		if (sdlSettings.event.type == SDL_MOUSEBUTTONDOWN) {
			map.map[player.x][player.y].type = 0; // Remove current player so you dont have ghost player
			player.x = -cam.offSetX + sdlSettings.curHoverX;
			player.y = -cam.offSetY + sdlSettings.curHoverY;

			settings.cheats = false;
			cam.highlight = false;
		}
	}
}
