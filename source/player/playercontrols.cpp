#include "SDL2/SDL.h"

#include "addional.h"
#include "block.h"

namespace player {
	void MouseEvent(SDL_Event event, bool isMouseDown, bool &colorPickerTool, bool &highlight, bool inventory, Block worldMap[250][250], int mapWidth, int mapHeight, int curHoverX, int curHoverY, int &blockColor, int camOffSetX, int camOffSetY) {
		if (curHoverX <= mapWidth && curHoverY <= mapHeight && !inventory && worldMap[curHoverX - camOffSetX][curHoverY - camOffSetY].type != 1) {
			if (!highlight) {
				if(isMouseDown) {
					Uint32 mouseButtons = SDL_GetMouseState(NULL, NULL);
					if (mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT)) {
						worldMap[curHoverX - camOffSetX][curHoverY - camOffSetY].type = 2;
						worldMap[curHoverX - camOffSetX][curHoverY - camOffSetY].color = blockColor;
					}
					else if (mouseButtons & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
						worldMap[curHoverX - camOffSetX][curHoverY - camOffSetY].type = 0;
						worldMap[curHoverX - camOffSetX][curHoverY - camOffSetY].color = 0;
					}
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN && colorPickerTool) {
				if (worldMap[curHoverX - camOffSetX][curHoverY - camOffSetY].type == 2) {
					blockColor = worldMap[curHoverX - camOffSetX][curHoverY - camOffSetY].color;
					colorPickerTool = !colorPickerTool;
					highlight = !highlight;
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
