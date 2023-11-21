#include "SDL2/SDL.h"

#include "graphics.h"

namespace player {
    void inventoryevent(SDL_Event event, bool &inventory) {
        if(event.key.keysym.sym == SDLK_e) {
            inventory = !inventory;
        }
    }
    void mouseinvchooser(SDL_Event event, bool inventory, int &curblock, int mousex, int mousey) {
        // Rects
        SDL_Rect selclrect = {50, 55, 10, 40};
        SDL_Rect selcrrect = {120, 55, 10, 40};

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (mousex >= selclrect.x && mousex <= selclrect.x + selclrect.w &&
                mousey >= selclrect.y && mousey <= selclrect.y + selclrect.h) {
                if (curblock == 10) {
                    curblock = 25;
                }
                else {
                    curblock -= 1;
                }
            }
            else if (mousex >= selcrrect.x && mousex <= selcrrect.x + selcrrect.w &&
                mousey >= selcrrect.y && mousey <= selcrrect.y + selcrrect.h) {
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
        int colorr, colorg, colorb = 0;

        if (inventory) {
            // Render bg
            graphics::getColor(8, colorr, colorg, colorb);
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_Rect bgrect = {25, 25, width - 50, height - 50};
            SDL_RenderFillRect(renderer, &bgrect);

            // Selector arrows
            // Color left
            graphics::getColor(15, colorr, colorg, colorb);
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_Rect selclrect = {50, 55, 10, 40};
            SDL_RenderFillRect(renderer, &selclrect);

            // Color right
            graphics::getColor(15, colorr, colorg, colorb);
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_Rect selcrrect = {120, 55, 10, 40};
            SDL_RenderFillRect(renderer, &selcrrect);

            // Current block preview
            graphics::getColor(curblock - 10, colorr, colorg, colorb);
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_Rect previewrect = {65, 50, 50, 50};
            SDL_RenderFillRect(renderer, &previewrect);
        }
    }
}
