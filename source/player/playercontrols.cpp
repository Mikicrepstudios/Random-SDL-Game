#include "SDL2/SDL.h"

#include "addional.h"
#include "block.h"
#include "presets.h"
#include "settings.h"

namespace mouse {
	void Event(SDL_Event event, game::SDL_Settings sdlSettings, game::Settings &settings, game::Map &map, game::Camera &camera, game::Preset preset[10]) {
		if (sdlSettings.curHoverX <= map.width && sdlSettings.curHoverY <= map.height && !settings.inventory && map.map[sdlSettings.curHoverX - camera.offSetX][sdlSettings.curHoverY - camera.offSetY].type != 1) {
			if (!camera.highlight) {
				if(sdlSettings.isMouseDown) {
					Uint32 mouseButtons = SDL_GetMouseState(NULL, NULL);
					if (mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT)) {
						map.map[sdlSettings.curHoverX - camera.offSetX][sdlSettings.curHoverY - camera.offSetY].type = 2;
						map.map[sdlSettings.curHoverX - camera.offSetX][sdlSettings.curHoverY - camera.offSetY].color = settings.blockColor;
					}
					else if (mouseButtons & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
						map.map[sdlSettings.curHoverX - camera.offSetX][sdlSettings.curHoverY - camera.offSetY].type = 0;
						map.map[sdlSettings.curHoverX - camera.offSetX][sdlSettings.curHoverY - camera.offSetY].color = 0;
					}
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN && settings.colorPickerTool) {
				if (map.map[sdlSettings.curHoverX - camera.offSetX][sdlSettings.curHoverY - camera.offSetY].type == 2) {
					preset[settings.curPreset].blockColor = map.map[sdlSettings.curHoverX - camera.offSetX][sdlSettings.curHoverY - camera.offSetY].color;
					settings.colorPickerTool = !settings.colorPickerTool;
					camera.highlight = !camera.highlight;
				}
			}
		}
	}
	void Overlay(game::SDL_Settings sdlSettings, game::Settings settings, game::Map map, game::Camera camera) {
		int curHoverX = sdlSettings.curHoverX;
		int curHoverY = sdlSettings.curHoverY;

		if (curHoverX <= map.width && curHoverY <= map.height && map.map[curHoverX - camera.offSetX][curHoverY - camera.offSetY].type != 1 && !settings.inventory) {
			SDL_Rect mouseRect = {curHoverX * camera.scale, curHoverY * camera.scale, camera.scale, camera.scale};
			if (camera.highlight == false) {
                if (settings.bgColor == 32 || map.map[curHoverX - camera.offSetX][curHoverY - camera.offSetY].color == 32)
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
