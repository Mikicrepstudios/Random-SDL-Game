#include "SDL2/SDL.h"

#include "block.h"
#include "settings.h"

namespace cheats {
	void CamTp(game::SDL_Settings sdlSettings, game::Settings &settings, game::Camera &camera) {
		if (sdlSettings.event.type == SDL_MOUSEBUTTONDOWN) {
			camera.offSetX -= sdlSettings.curHoverX;
			camera.offSetY -= sdlSettings.curHoverY;

			settings.cheats = false;
			camera.highlight = false;
		}
	}
	void PlayerTp(game::SDL_Settings sdlSettings, game::Settings &settings, game::Map &map, game::Camera &camera, game::Player &player) {
		if (sdlSettings.event.type == SDL_MOUSEBUTTONDOWN) {
			map.map[player.x][player.y].type = 0; // Remove current player so you dont have ghost player
			player.x = -camera.offSetX + sdlSettings.curHoverX;
			player.y = -camera.offSetY + sdlSettings.curHoverY;

			settings.cheats = false;
			camera.highlight = false;
		}
	}
}
