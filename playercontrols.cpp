#include "SDL2/SDL.h"

#include "block.h"

namespace player {
    void MouseEvent(SDL_Event event, bool inventory, Block worldMap[250][250], int mapWidth, int mapHeight, int curHoverX, int curHoverY, int &curBlock, int camOffSetX, int camOffSetY) {
        if (event.type == SDL_MOUSEBUTTONDOWN && curHoverX <= mapWidth && curHoverY <= mapHeight && !inventory && worldMap[curHoverX - camOffSetX][curHoverY - camOffSetY].type != 1) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                worldMap[curHoverX - camOffSetX][curHoverY - camOffSetY].type = 2;
                worldMap[curHoverX - camOffSetX][curHoverY - camOffSetY].color = curBlock;
            }
            else if (event.button.button == SDL_BUTTON_RIGHT)
                worldMap[curHoverX - camOffSetX][curHoverY - camOffSetY].type = 0;
        }
    }
}

namespace overlay {
    void Mouse(SDL_Renderer* renderer, bool inventory, Block worldMap[250][250], int mapWidth, int mapHeight, int curHoverX, int curHoverY, int camOffSetX, int camOffSetY, int camScale) {
        if (curHoverX <= mapWidth && curHoverY <= mapHeight && worldMap[curHoverX - camOffSetX][curHoverY - camOffSetY].type != 1 && !inventory) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255 );
            SDL_Rect mouseRect = {curHoverX * camScale, curHoverY * camScale, camScale, camScale};
            SDL_RenderFillRect(renderer, &mouseRect);
        }
    }
}
