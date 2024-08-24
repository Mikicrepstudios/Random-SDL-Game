#include "SDL2/SDL.h"

#include "addional.h"
#include "block.h"
#include "presets.h"
#include "settings.h"

namespace player {
	void MouseEvent(SDL_Event event, game::SDL_Settings sdlSettings, game::Settings &settings, game::Map &map, game::Camera &camera, game::Preset preset[10]) {
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
}

namespace overlay {
	void Mouse(SDL_Renderer* renderer, bool highlight, bool inventory, Block worldMap[250][250], int mapWidth, int mapHeight, int curHoverX, int curHoverY, int camOffSetX, int camOffSetY, int camScale, int bgColor) {
		if (curHoverX <= mapWidth && curHoverY <= mapHeight && worldMap[curHoverX - camOffSetX][curHoverY - camOffSetY].type != 1 && !inventory) {
			SDL_Rect mouseRect = {curHoverX * camScale, curHoverY * camScale, camScale, camScale};
			if (highlight == false) {
                if (bgColor == 32 || worldMap[curHoverX - camOffSetX][curHoverY - camOffSetY].color == 32)
					draw::DrawRect(renderer, mouseRect, 1);
				else
					draw::DrawRect(renderer, mouseRect, 32);
			}
			else {
				if (bgColor == 27)
					draw::DrawRect(renderer, mouseRect, 26);
				else
					draw::DrawRect(renderer, mouseRect, 27);
			}
		}
	}
}
