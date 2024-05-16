#pragma once

namespace player {
	// Hud
	extern void gameInfo(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, int camScale, int camOffSetX, int camOffSetY, int playerX, int playerY);

	// Events
	extern void PlayerMovement(SDL_Event event, Block worldMap[250][250], int mapWidth, int mapHeight, int playerSpeed, int &playerX, int &playerY);

	extern void MouseEvent(SDL_Event event, bool &colorPickerTool, bool &highlight, bool inventory, Block worldMap[250][250], int mapWidth, int mapHeight, int curHoverX, int curHoverY, int &blockColor, int camOffSetX, int camOffSetY);

	extern void InventoryEvent(SDL_Event event, bool &inventory, bool &colorPick, bool &bgColorPick, int &fps);

	extern void MouseInvChooser(SDL_Renderer* renderer, SDL_Event event, bool &inventory, bool &running, bool &highlight, bool &camTp, bool &playerTp, bool &colorPick, bool &bgColorPick, bool &playerColorPick, bool &gameInfo, Block worldMap[250][250], int mapWidth, int mapHeight, int &blockColor, int &bgColor, int &playerColor, int mouseX, int mouseY, int width, int height);

	extern void ColorPickerEvent(bool &picker, int mouseX, int mouseY, int width, int height, int &color);
}
