#pragma once

namespace overlay {
	extern void Inventory(SDL_Renderer* renderer, TTF_Font* font, inventory::rects rects, bool inventory, bool gameInfo, int blockColor, int bgColor,  int playerColor, int width, int height, int mouseX, int mouseY, int preset, game::SDL_Settings sdlSettings, game::Settings settings);

	extern void Mouse(SDL_Renderer* renderer, bool highlight, bool inventory, Block worldMap[250][250], int mapWidth, int mapHeight, int curHoverX, int curHoverY, int camOffSetX, int camOffSetY, int camScale, int bgColor);

	extern void ColorPicker(game::SDL_Settings sdlSettings, game::Settings settings);
}
