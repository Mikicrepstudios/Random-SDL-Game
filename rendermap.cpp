#include "SDL2/SDL.h"

#include "objects.h"
#include "graphics.h"

namespace game {
    void rendermap(SDL_Renderer* renderer, int worldmap[250][250], int mapwidth, int mapheight, int camoffsetx, int camoffsety, int camscale) {
        for(int x = 0; x <= mapwidth; x++) {
            for(int y = 0; y <= mapheight; y++) {
                // Get current block id and define variables
                int objectid = worldmap[x][y];
                int colorid = 0;

                int colorr = 0;
                int colorg = 0;
                int colorb = 0;

                // Get color data
                if (objectid != 0) {
                    objects::objectColor(objectid, colorid);
                    graphics::getColor(colorid, colorr, colorg, colorb);

                    // Do rendering
                    SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
                    SDL_Rect currect = {(camoffsetx * camscale) + (x * camscale), (camoffsety * camscale) + (y * camscale), camscale, camscale};
                    SDL_RenderFillRect(renderer, &currect);
                }
            }
        }
    }
}
