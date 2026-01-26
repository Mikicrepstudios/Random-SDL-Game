#include "SDL.h"

#include "mf/core.h"
#include "mf/colors.h"
#include "mf/graphics.h"

#include "block.h"
#include "game.h"
#include "settings.h"

#include <iostream>

namespace game {
	void MouseEvent(core::MF_Window &window, game::Game &game, game::Preset preset[10]) {
		/**
		 * @brief This function controls block placing
		 */

		auto& settings = game.settings;
		auto& cam = game.cam;
		auto& map = game.map;

		// SOMEWHERE HERE WAS* BUG #1
		// Calculate map-relative coordinates by adding positive camera offset
		int mapX = game.curHoverX + cam.offSetX;
		int mapY = game.curHoverY + cam.offSetY;

		// Bounds check to avoid crashing when mouse is outside map area
		if (mapX >= 0 && mapX < map.width &&
			mapY >= 0 && mapY < map.height &&
			!settings.inventory &&                   // Don't place blocks if inventory is open
			map.map[mapX][mapY].type != 1)          // Don't act on tiles of type 1 (e.g., solid or special)
		{
			if (!settings.cheats) {
				if (window.mouse.isDown) {
					Uint32 mouseButtons = SDL_GetMouseState(NULL, NULL);

					if (mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT)) {
						// Place block with user-selected settings
						map.map[mapX][mapY].type = 2;
						map.map[mapX][mapY].color = settings.blockColor;
						map.map[mapX][mapY].texture = settings.blockTextureId;
						map.map[mapX][mapY].isSolid = settings.placeSolidBlocks;
					}
					else if (mouseButtons & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
						// Remove block (reset tile to empty)
						map.map[mapX][mapY].type = 0;
						map.map[mapX][mapY].color = 0;
						map.map[mapX][mapY].texture = 0;
						map.map[mapX][mapY].isSolid = false;
					}
				}
			}
			else if (window.mouse.isDown && settings.cheats && settings.cheatsId == 3) {
				if (map.map[mapX][mapY].type == 2) {
					preset[settings.curPreset].blockColor = map.map[mapX][mapY].color;
					preset[settings.curPreset].textureId = map.map[mapX][mapY].texture;
					settings.cheats = false;  // Exit cheat mode after copying
				}
			}
		}
	}
	void MouseOverlay(core::MF_Window &window, game::Game &game) {
		/**
		 * @brief This function shows which block are you hovering
		 */

		auto& settings = game.settings;
		auto& cam = game.cam;
		auto& map = game.map;
		
		int curHoverX = game.curHoverX;
		int curHoverY = game.curHoverY;

		int x = curHoverX + cam.offSetX; // subtract positive offset
		int y = curHoverY + cam.offSetY; // subtract positive offset

		// Check bounds including non-negativity and max limits
		if (x >= 0 && x < map.width &&
			y >= 0 && y < map.height &&
			map.map[x][y].type != 1 &&
			!settings.inventory)
		{
			SDL_Rect mouseRect = {curHoverX * cam.scale, curHoverY * cam.scale, cam.scale, cam.scale};
			
			if (!cam.highlight) {
				if (settings.bgColor == 32 || map.map[x][y].color == 32)
					draw::DrawRect(window.renderer, mouseRect, colors::black);
				else
					draw::DrawRect(window.renderer, mouseRect, colors::white);
			} else {
				if (settings.bgColor == 27)
					draw::DrawRect(window.renderer, mouseRect, colors::darkred);
				else
					draw::DrawRect(window.renderer, mouseRect, colors::red);
			}
		}
	}
}
