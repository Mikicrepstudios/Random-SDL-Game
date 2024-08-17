#pragma once

#include "settings.h"

namespace player {
	// Hud
	extern void gameInfo(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, int fps, int camScale, int camOffSetX, int camOffSetY, int playerX, int playerY);

	// Events
	extern void PlayerMovement(SDL_Event event, Block worldMap[250][250], int mapWidth, int mapHeight, game::Player &player);

	extern void MouseEvent(SDL_Event event, bool isMouseDown, bool &colorPickerTool, bool &highlight, bool inventory, Block worldMap[250][250], int mapWidth, int mapHeight, int curHoverX, int curHoverY, int &blockColor, int camOffSetX, int camOffSetY);

	extern void InventoryEvent(SDL_Event event, bool &inventory, bool &colorPick, bool &bgColorPick, int &fps);

	extern void MouseInvChooser(SDL_Renderer* renderer, SDL_Event event, inventory::rects rects, bool &inventory, bool &running, bool &highlight, bool &camTp, bool &playerTp, bool &colorPick, bool &bgColorPick, bool &playerColorPick, bool &gameInfo, Block worldMap[250][250], int mapWidth, int mapHeight, int &blockColor, int &bgColor, int &playerColor, int mouseX, int mouseY, int width, int height, int &playerX, int &playerY, int &camOffSetX, int &camOffSetY, int &camScale, game::Settings settings, game::Player player, game::Camera camera);

	extern void ColorPickerEvent(bool &picker, int mouseX, int mouseY, int width, int height, int &color);
}
