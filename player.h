#pragma once

namespace player {
    extern void PlayerMovement(SDL_Event event, Block worldMap[250][250], int mapWidth, int mapHeight, int playerSpeed, int &playerX, int &playerY);
    extern void MouseEvent(SDL_Event event, bool inventory, Block worldMap[250][250], int mapWidth, int mapHeight, int curHoverX, int curHoverY, int &blockColor, int camOffSetX, int camOffSetY);
    extern void InventoryEvent(SDL_Event event, bool &inventory, bool &colorPick, bool &bgColorPick);
    extern void MouseInvChooser(SDL_Renderer* renderer, SDL_Event event, bool inventory, bool &running, bool &colorPick, bool &bgColorPick, bool &playerColorPick, Block worldMap[250][250], int mapWidth, int mapHeight, int &blockColor, int &bgColor, int &playerColor, int mouseX, int mouseY, int width, int height);
    extern void ColorPickerEvent(bool &picker, int mouseX, int mouseY, int width, int height, int &color);
}
