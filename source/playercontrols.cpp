#include "SDL2/SDL.h"

#include "block.h"
#include "graphics.h"

namespace player {
	void MouseEvent(SDL_Event event, bool &colorPickerTool, bool &highlight, bool inventory, Block worldMap[250][250], int mapWidth, int mapHeight, int curHoverX, int curHoverY, int &blockColor, int camOffSetX, int camOffSetY) {
		if (event.type == SDL_MOUSEBUTTONDOWN && curHoverX <= mapWidth && curHoverY <= mapHeight && !inventory && worldMap[curHoverX - camOffSetX][curHoverY - camOffSetY].type != 1) {
			if (!highlight) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					worldMap[curHoverX - camOffSetX][curHoverY - camOffSetY].type = 2;
					worldMap[curHoverX - camOffSetX][curHoverY - camOffSetY].color = blockColor;
				}
				else if (event.button.button == SDL_BUTTON_RIGHT)
					worldMap[curHoverX - camOffSetX][curHoverY - camOffSetY].type = 0;
			}
			else if (colorPickerTool) {
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
			int colorR, colorG, colorB = 0;
			if (highlight == false) {
                		if (bgColor == 32)
					graphics::GetColor(1, colorR, colorG, colorB);
				else
					graphics::GetColor(32, colorR, colorG, colorB);
			}
			else {
				if (bgColor == 27)
					graphics::GetColor(26, colorR, colorG, colorB);
				else
					graphics::GetColor(27, colorR, colorG, colorB);
			}
			SDL_SetRenderDrawColor(renderer, colorR, colorG, colorB, 255);
			SDL_Rect mouseRect = {curHoverX * camScale, curHoverY * camScale, camScale, camScale};
			SDL_RenderFillRect(renderer, &mouseRect);
		}
	}
}
