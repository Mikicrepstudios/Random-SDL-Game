#pragma once

namespace overlay {
    extern void Inventory(SDL_Renderer* renderer, TTF_Font* font, bool inventory, int curBlock, int bgColor, int mouseX, int mouseY);
    extern void Mouse(SDL_Renderer* renderer, bool inventory, Block worldMap[250][250], int mapWidth, int mapHeight, int curHoverX, int curHoverY, int camOffSetX, int camOffSetY, int camScale);
}
