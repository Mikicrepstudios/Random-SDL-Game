#include "SDL.h"

#include "mf/core.h"

#include "block.h"
#include "settings.h"

namespace cheats {
	int CamTp(core::MF_Window &window, game::Game game, game::Settings &settings, game::Camera &cam) {
		/**
		 * @brief This function executes Camera Teleport cheat
		 * @param window Game window
		 * @param game Game core
		 * @param settings Game settings
		 * @param cam Game camera
		 */

		if (window.event.type == SDL_MOUSEBUTTONDOWN) {
			cam.offSetX -= game.curHoverX;
			cam.offSetY -= game.curHoverY;

			return 1;
		}

		return 0;
	}
	int PlayerTp(core::MF_Window &window, game::Game game, game::Settings &settings, game::Map &map, game::Camera &cam, game::Player &player) {
		/**
		 * @brief This function executes Player Teleport cheat
		 * @param window Game window
		 * @param game Game core
		 * @param settings Game settings
		 * @param map Game map
		 * @param cam Game camera
		 * @param player Game player
		 */
		
		if (window.event.type == SDL_MOUSEBUTTONDOWN) {
			map.map[player.x][player.y].type = 0; // Remove current player so you dont have ghost player
			player.x = -cam.offSetX + game.curHoverX;
			player.y = -cam.offSetY + game.curHoverY;

			return 1;
		}
		
		return 0;
	}
}
