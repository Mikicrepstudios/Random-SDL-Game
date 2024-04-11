#pragma once

namespace overlay {
    extern void Inventory(SDL_Renderer* renderer, TTF_Font* font, bool inventory, bool colorPick, bool bgcolorPick, int curBlock, int &bgColor, int mouseX, int mouseY);
    extern void Mouse(SDL_Renderer* renderer, bool inventory, Block worldMap[250][250], int mapWidth, int mapHeight, int curHoverX, int curHoverY, int camOffSetX, int camOffSetY, int camScale);
    extern void ColorPicker(SDL_Renderer* renderer, int width, int height);
}
