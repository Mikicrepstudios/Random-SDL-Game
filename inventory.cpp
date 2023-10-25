#include "SDL2/SDL.h"

#include "graphics.h"

namespace player {
    void inventoryevent(SDL_Event event, bool &inventory, int &curblock) {
        if(event.key.keysym.sym == SDLK_e) {
            inventory = !inventory;
        }
        if (inventory) {
            if (event.key.keysym.sym == SDLK_LEFT) {
                if (curblock == 10) {
                    curblock = 25;
                }
                else {
                    curblock -= 1;
                }
            }
            else if (event.key.keysym.sym == SDLK_RIGHT) {
                if (curblock == 25) {
                    curblock = 10;
                }
                else {
                    curblock += 1;
                }
            }
        }
    }
}

namespace overlay {
    void inventory(SDL_Renderer* renderer, int width, int height, bool inventory, int curblock) {
        // Define variables
        int colorr = 0;
        int colorg = 0;
        int colorb = 0;

        if (inventory) {
            // Render bg
            graphics::getColor(8, colorr, colorg, colorb);
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_Rect bgrect = {25, 25, width - 50, height - 50};
            SDL_RenderFillRect(renderer, &bgrect);

            // Current block preview
            graphics::getColor(curblock - 10, colorr, colorg, colorb);
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_Rect previewrect = {50, 50, 50, 50};
            SDL_RenderFillRect(renderer, &previewrect);
        }
    }
}
