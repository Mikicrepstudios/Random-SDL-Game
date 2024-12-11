#include "SDL.h"

#include "mf/core.h"
#include "mf/colors.h"
#include "mf/graphics.h"

#include "block.h"
#include "game.h"
#include "settings.h"

namespace game {
	void MouseEvent(core::MF_Window &window, game::Game game, game::Settings &settings, game::Map &map, game::Camera &cam, game::Preset preset[10]) {
		/**
		 * @brief This function controls block placing
		 */

		if (game.curHoverX <= map.width && game.curHoverY <= map.height && !settings.inventory && map.map[game.curHoverX - cam.offSetX][game.curHoverY - cam.offSetY].type != 1) {
			if (!settings.cheats) {
				if(window.mouse.isDown) {
					Uint32 mouseButtons = SDL_GetMouseState(NULL, NULL);
					if (mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT)) {
						map.map[game.curHoverX - cam.offSetX][game.curHoverY - cam.offSetY].type = 2;
						map.map[game.curHoverX - cam.offSetX][game.curHoverY - cam.offSetY].color = settings.blockColor;
						map.map[game.curHoverX - cam.offSetX][game.curHoverY - cam.offSetY].texture = settings.blockTextureId;
						map.map[game.curHoverX - cam.offSetX][game.curHoverY - cam.offSetY].isSolid = settings.placeSolidBlocks;
					}
					else if (mouseButtons & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
						map.map[game.curHoverX - cam.offSetX][game.curHoverY - cam.offSetY].type = 0;
						map.map[game.curHoverX - cam.offSetX][game.curHoverY - cam.offSetY].color = 0;
						map.map[game.curHoverX - cam.offSetX][game.curHoverY - cam.offSetY].texture = 0;
						map.map[game.curHoverX - cam.offSetX][game.curHoverY - cam.offSetY].isSolid = false;
					}
				}
			}
			else if (window.event.type == SDL_MOUSEBUTTONDOWN && settings.cheats && settings.cheatsId == 3) {
				if (map.map[game.curHoverX - cam.offSetX][game.curHoverY - cam.offSetY].type == 2) {
					preset[settings.curPreset].blockColor = map.map[game.curHoverX - cam.offSetX][game.curHoverY - cam.offSetY].color;
					settings.cheats = false;
				}
			}
		}
	}
	void MouseOverlay(core::MF_Window &window, game::Game game, game::Settings settings, game::Map map, game::Camera cam) {
		/**
		 * @brief This function shows which block are you hovering
		 */
		
		int curHoverX = game.curHoverX;
		int curHoverY = game.curHoverY;

		if (curHoverX <= map.width && curHoverY <= map.height && map.map[curHoverX - cam.offSetX][curHoverY - cam.offSetY].type != 1 && !settings.inventory) {
			SDL_Rect mouseRect = {curHoverX * cam.scale, curHoverY * cam.scale, cam.scale, cam.scale};
			if (cam.highlight == false) {
                if (settings.bgColor == 32 || map.map[curHoverX - cam.offSetX][curHoverY - cam.offSetY].color == 32)
					draw::DrawRect(window.renderer, mouseRect, colors::black);
				else
					draw::DrawRect(window.renderer, mouseRect, colors::white);
			}
			else {
				if (settings.bgColor == 27)
					draw::DrawRect(window.renderer, mouseRect, colors::darkred);
				else
					draw::DrawRect(window.renderer, mouseRect, colors::red);
			}
		}
	}
}
