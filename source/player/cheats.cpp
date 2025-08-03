#include <iostream>
#include "SDL.h"

#include "mf/core.h"

#include "block.h"
#include "settings.h"

namespace cheats {
	int CamTp(core::MF_Window &window, game::Game game, game::Settings &settings, game::Map map, game::Camera &cam) {
		/**
		 * @brief This function executes Camera Teleport cheat
		 * @param window Game window
		 * @param game Game core
		 * @param settings Game settings
		 * @param cam Game camera
		 */

		if (window.event.type == SDL_MOUSEBUTTONDOWN) {
			// Calculate max camera offsets based on map size and window size/scale
			int maxX = (map.width - 1) - window.width / (cam.scale > 0 ? cam.scale : 1);
			int maxY = (map.height - 1) - window.height / (cam.scale > 0 ? cam.scale : 1);

			// Clamp the hover position to valid ranges so camera offset stays sane
			int hoverX = game.curHoverX;
			if (hoverX < 0) hoverX = 0;
			else if (hoverX > maxX) hoverX = maxX;

			int hoverY = game.curHoverY;
			if (hoverY < 0) hoverY = 0;
			else if (hoverY > maxY) hoverY = maxY;

			// Add clamped hover positions to camera offset (teleport camera)
			cam.offSetX += hoverX;
			cam.offSetY += hoverY;

			// Clamp camera offset to max allowed bounds
			if (cam.offSetX > maxX) cam.offSetX = maxX;
			if (cam.offSetY > maxY) cam.offSetY = maxY;

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
			player.x = cam.offSetX + game.curHoverX;
			player.y = cam.offSetY + game.curHoverY;

			return 1;
		}
		
		return 0;
	}
}
