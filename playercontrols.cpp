#include "SDL2/SDL.h"

namespace player {
    void mouseevent(SDL_Event event, bool inventory, int worldmap[250][250], int mapwidth, int mapheight, int curhoverx, int curhovery, int &curblock, int camoffsetx, int camoffsety) {
        if (curhoverx <= mapwidth && curhovery <= mapheight && !inventory) {
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (worldmap[curhoverx - camoffsetx][curhovery - camoffsety] != 1) {
                        worldmap[curhoverx - camoffsetx][curhovery - camoffsety] = curblock;
                    }
                }
                else if (event.button.button == SDL_BUTTON_RIGHT) {
                    if (worldmap[curhoverx - camoffsetx][curhovery - camoffsety] != 1) {
                        worldmap[curhoverx - camoffsetx][curhovery - camoffsety] = 0;
                    }
                }
            }
        }
    }
}

namespace overlay {
    void mouse(SDL_Renderer* renderer, bool inventory, int worldmap[250][250], int mapwidth, int mapheight, int curhoverx, int curhovery, int camoffsetx, int camoffsety, int camscale) {
        if (curhoverx <= mapwidth && curhovery <= mapheight) {
            if (worldmap[curhoverx - camoffsetx][curhovery - camoffsety] != 1) {
                if (!inventory) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255 );
                    SDL_Rect mouserect = {curhoverx * camscale, curhovery * camscale, camscale, camscale};
                    SDL_RenderFillRect(renderer, &mouserect);
                }
            }
        }
    }
}
