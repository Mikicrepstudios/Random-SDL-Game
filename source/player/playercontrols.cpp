#include "SDL2/SDL.h"
#include <iostream>
#include "draw.h"
#include "block.h"
#include "presets.h"
#include "settings.h"

namespace mouse {
	void Event(SDL_Event event, game::SDL_Settings sdlSettings, game::Settings &settings, game::Map &map, game::Camera &cam, game::Preset preset[10]) {
		if (sdlSettings.curHoverX <= map.width && sdlSettings.curHoverY <= map.height && !settings.inventory && map.map[sdlSettings.curHoverX - cam.offSetX][sdlSettings.curHoverY - cam.offSetY].type != 1) {
			if (!settings.cheats) {
				if(sdlSettings.isMouseDown) {
					Uint32 mouseButtons = SDL_GetMouseState(NULL, NULL);
					if (mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT)) {
						map.map[sdlSettings.curHoverX - cam.offSetX][sdlSettings.curHoverY - cam.offSetY].type = 2;
						map.map[sdlSettings.curHoverX - cam.offSetX][sdlSettings.curHoverY - cam.offSetY].color = settings.blockColor;
					}
					else if (mouseButtons & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
						map.map[sdlSettings.curHoverX - cam.offSetX][sdlSettings.curHoverY - cam.offSetY].type = 0;
						map.map[sdlSettings.curHoverX - cam.offSetX][sdlSettings.curHoverY - cam.offSetY].color = 0;
					}
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN && settings.cheats && settings.cheatsId == 3) {
				if (map.map[sdlSettings.curHoverX - cam.offSetX][sdlSettings.curHoverY - cam.offSetY].type == 2) {
					preset[settings.curPreset].blockColor = map.map[sdlSettings.curHoverX - cam.offSetX][sdlSettings.curHoverY - cam.offSetY].color;
					settings.cheats = false;
				}
			}
		}
	}
	void Overlay(game::SDL_Settings sdlSettings, game::Settings settings, game::Map map, game::Camera cam) {
		int curHoverX = sdlSettings.curHoverX;
		int curHoverY = sdlSettings.curHoverY;

		if (curHoverX <= map.width && curHoverY <= map.height && map.map[curHoverX - cam.offSetX][curHoverY - cam.offSetY].type != 1 && !settings.inventory) {
			SDL_Rect mouseRect = {curHoverX * cam.scale, curHoverY * cam.scale, cam.scale, cam.scale};
			if (cam.highlight == false) {
                if (settings.bgColor == 32 || map.map[curHoverX - cam.offSetX][curHoverY - cam.offSetY].color == 32)
					draw::DrawRect(sdlSettings.renderer, mouseRect, 1);
				else
					draw::DrawRect(sdlSettings.renderer, mouseRect, 32);
			}
			else {
				if (settings.bgColor == 27)
					draw::DrawRect(sdlSettings.renderer, mouseRect, 26);
				else
					draw::DrawRect(sdlSettings.renderer, mouseRect, 27);
			}
		}
	}
}
