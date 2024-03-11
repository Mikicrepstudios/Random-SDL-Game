#include "SDL2/SDL.h"

namespace player {
    void MouseEvent(SDL_Event event, bool inventory, int worldMap[250][250], int mapWidth, int mapHeight, int curHoverX, int curHoverY, int &curBlock, int camOffSetX, int camOffSetY) {
        if (event.type == SDL_MOUSEBUTTONDOWN && curHoverX <= mapWidth && curHoverY <= mapHeight && !inventory && worldMap[curHoverX - camOffSetX][curHoverY - camOffSetY] != 1) {
            if (event.button.button == SDL_BUTTON_LEFT)
                worldMap[curHoverX - camOffSetX][curHoverY - camOffSetY] = curBlock;
            else if (event.button.button == SDL_BUTTON_RIGHT)
                worldMap[curHoverX - camOffSetX][curHoverY - camOffSetY] = 0;
        }
    }
}

namespace overlay {
    void Mouse(SDL_Renderer* renderer, bool inventory, int worldMap[250][250], int mapWidth, int mapHeight, int curHoverX, int curHoverY, int camOffSetX, int camOffSetY, int camScale) {
        if (curHoverX <= mapWidth && curHoverY <= mapHeight && worldMap[curHoverX - camOffSetX][curHoverY - camOffSetY] != 1 && !inventory) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255 );
            SDL_Rect mouseRect = {curHoverX * camScale, curHoverY * camScale, camScale, camScale};
            SDL_RenderFillRect(renderer, &mouseRect);
        }
    }
}
