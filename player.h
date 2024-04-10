#pragma once

namespace player {
    extern void PlayerMovement(SDL_Event event, Block worldMap[250][250], int mapWidth, int mapHeight, int playerSpeed, int &playerX, int &playerY);
    extern void MouseEvent(SDL_Event event, bool inventory, Block worldMap[250][250], int mapWidth, int mapHeight, int curHoverX, int curHoverY, int &curBlock, int camOffSetX, int camOffSetY);
    extern void InventoryEvent(SDL_Event event, bool &inventory);
    extern void MouseInvChooser(SDL_Event event, bool inventory, bool &running, Block worldMap[250][250], int mapWidth, int mapHeight, int &curBlock, int &bgColor, int mouseX, int mouseY);
}
