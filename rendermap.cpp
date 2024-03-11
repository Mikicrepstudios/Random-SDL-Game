#include "SDL2/SDL.h"

#include "objects.h"
#include "graphics.h"

namespace game {
    void RenderMap(SDL_Renderer* renderer, int worldMap[250][250], int mapWidth, int mapHeight, int camOffSetX, int camOffSetY, int camScale) {
        for(int x = 0; x <= mapWidth; x++) {
            for(int y = 0; y <= mapHeight; y++) {
                // Get current block id and define variables
                int objectId = worldMap[x][y];
                int colorId = 0;
                int colorR, colorG, colorB = 0;

                // Get color data
                if (objectId != 0) {
                    objects::ObjectColor(objectId, colorId);
                    graphics::GetColor(colorId, colorR, colorG, colorB);

                    // Do rendering
                    SDL_SetRenderDrawColor(renderer, colorR, colorG, colorB, 255);
                    SDL_Rect curRect = {(camOffSetX * camScale) + (x * camScale), (camOffSetY * camScale) + (y * camScale), camScale, camScale};
                    SDL_RenderFillRect(renderer, &curRect);
                }
            }
        }
    }
}
