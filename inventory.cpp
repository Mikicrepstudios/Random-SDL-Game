#include "SDL2/SDL.h"

#include "graphics.h"

namespace player {
    void inventoryevent(SDL_Event event, bool &inventory) {
        if(event.key.keysym.sym == SDLK_e) {
            inventory = !inventory;
        }
    }
    void mouseinvchooser(SDL_Event event, bool inventory, int &curblock, int &bgcolor, int mousex, int mousey) {
        // Rects
        SDL_Rect selclrect = {50, 55, 20, 80};
        SDL_Rect selcrrect = {170, 55, 20, 80};
        SDL_Rect selbclrect = {50, 155, 20, 80};
        SDL_Rect selbcrrect = {170, 155, 20, 80};

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            // Color
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

            // BG Color
            if (mousex >= selbclrect.x && mousex <= selbclrect.x + selbclrect.w &&
                mousey >= selbclrect.y && mousey <= selbclrect.y + selbclrect.h) {
                if (bgcolor == 0) {
                    bgcolor = 15;
                }
                else {
                    bgcolor -= 1;
                }
            }
            else if (mousex >= selbcrrect.x && mousex <= selbcrrect.x + selbcrrect.w &&
                mousey >= selbcrrect.y && mousey <= selbcrrect.y + selbcrrect.h) {
                if (bgcolor == 15) {
                    bgcolor = 0;
                }
                else {
                    bgcolor += 1;
                }
            }
        }
    }
}

namespace overlay {
    void inventory(SDL_Renderer* renderer, int width, int height, bool inventory, int curblock, int bgcolor, int mousex, int mousey) {
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
            SDL_Rect selclrect = {50, 55, 20, 80};
            if (mousex >= selclrect.x && mousex <= selclrect.x + selclrect.w &&
                mousey >= selclrect.y && mousey <= selclrect.y + selclrect.h) {
                graphics::getColor(7, colorr, colorg, colorb);
            }
            else {
                graphics::getColor(15, colorr, colorg, colorb);
            }
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_RenderFillRect(renderer, &selclrect);
            // Color right
            SDL_Rect selcrrect = {170, 55, 20, 80};
            if (mousex >= selcrrect.x && mousex <= selcrrect.x + selcrrect.w &&
                mousey >= selcrrect.y && mousey <= selcrrect.y + selcrrect.h) {
                graphics::getColor(7, colorr, colorg, colorb);
            }
            else {
                graphics::getColor(15, colorr, colorg, colorb);
            }
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_RenderFillRect(renderer, &selcrrect);
            // Color preview
            graphics::getColor(curblock - 10, colorr, colorg, colorb);
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_Rect colorrect = {80, 55, 80, 80};
            SDL_RenderFillRect(renderer, &colorrect);

            // BG Color left
            SDL_Rect selbclrect = {50, 155, 20, 80};
            if (mousex >= selbclrect.x && mousex <= selbclrect.x + selbclrect.w &&
                mousey >= selbclrect.y && mousey <= selbclrect.y + selbclrect.h) {
                graphics::getColor(7, colorr, colorg, colorb);
            }
            else {
                graphics::getColor(15, colorr, colorg, colorb);
            }
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_RenderFillRect(renderer, &selbclrect);
            // BG Color right
            SDL_Rect selbcrrect = {170, 155, 20, 80};
            if (mousex >= selbcrrect.x && mousex <= selbcrrect.x + selbcrrect.w &&
                mousey >= selbcrrect.y && mousey <= selbcrrect.y + selbcrrect.h) {
                graphics::getColor(7, colorr, colorg, colorb);
            }
            else {
                graphics::getColor(15, colorr, colorg, colorb);
            }
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_RenderFillRect(renderer, &selbcrrect);
            // BG Color preview
            graphics::getColor(bgcolor, colorr, colorg, colorb);
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_Rect bgcolorrect = {80, 155, 80, 80};
            SDL_RenderFillRect(renderer, &bgcolorrect);

            // Current block preview
            graphics::getColor(curblock - 10, colorr, colorg, colorb);
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_Rect previewrect = {width / 2 - 50, 50, 100, 100};
            SDL_RenderFillRect(renderer, &previewrect);
        }
    }
}
