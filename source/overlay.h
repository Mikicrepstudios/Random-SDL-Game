#pragma once

namespace overlay {
	extern void Inventory(SDL_Renderer* renderer, TTF_Font* font, bool inventory, bool colorPick, bool bgcolorPick, bool playerColorPick, bool gameInfo, int blockColor, int bgColor,  int playerColor, int mouseX, int mouseY, int preset);

	extern void Mouse(SDL_Renderer* renderer, bool highlight, bool inventory, Block worldMap[250][250], int mapWidth, int mapHeight, int curHoverX, int curHoverY, int camOffSetX, int camOffSetY, int camScale, int bgColor);

	extern void ColorPicker(SDL_Renderer* renderer, TTF_Font* font, const char* text, int width, int height);
}
