#include "SDL2/SDL.h"

namespace player {
    void mouseevent(SDL_Event event, int worldmap[250][250], int curhoverx, int curhovery, int &curblock) {
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                if (worldmap[curhoverx][curhovery] != 1) {
                    worldmap[curhoverx][curhovery] = curblock;
                }
            }
            else if (event.button.button == SDL_BUTTON_RIGHT) {
                if (worldmap[curhoverx][curhovery] != 1) {
                    worldmap[curhoverx][curhovery] = 0;
                }
            }
        }
    }
}

namespace overlay {
    void mouse(SDL_Renderer* renderer, int worldmap[250][250], int curhoverx, int curhovery) {
        if (worldmap[curhoverx][curhovery] != 1) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255 );
            SDL_Rect mouserect = {curhoverx * 50, curhovery * 50, 50, 50};
            SDL_RenderFillRect(renderer, &mouserect);
        }
    }
}
