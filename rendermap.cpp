#include "SDL2/SDL.h"

#include "objects.h"
#include "graphics.h"

namespace game {
    void rendermap(SDL_Renderer* renderer, int worldmap[250][250], int mapwidth, int mapheight, int camoffsetx, int camoffsety) {
        for(int x = 0; x <= mapwidth; x++) {
            for(int y = 0; y <= mapheight; y++) {
                // Get current block id and define variables
                int objectid = worldmap[x][y];
                int colorid = 0;

                int colorr = 0;
                int colorg = 0;
                int colorb = 0;

                // Get color data
                objects::objectColor(objectid, colorid);
                graphics::getColor(colorid, colorr, colorg, colorb);

                // Do rendering
                SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
                SDL_Rect currect = {(camoffsetx * 50) + (x * 50), (camoffsety * 50) + (y * 50), 50, 50};
                SDL_RenderFillRect(renderer, &currect);
            }
        }
    }
}
