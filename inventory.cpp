#include "SDL2/SDL.h"
#include <SDL_ttf.h>

#include "files.h"
#include "graphics.h"
#include "addional.h"

const int width = 1280;
const int height = 800;

// Color
SDL_Rect selclrect = {50, 50, 20, 80};
SDL_Rect selcrrect = {170, 50, 20, 80};
SDL_Rect colorrect = {80, 50, 80, 80};
SDL_Rect colortextrect = {80, 140, 80, 40};

// Background color
SDL_Rect selbclrect = {width - 190, 50, 20, 80};
SDL_Rect selbcrrect = {width - 70, 50, 20, 80};
SDL_Rect bgcolorrect = {width - 160, 50, 80, 80};
SDL_Rect bgcolortextrect = {width - 160, 140, 80, 40};

// Preview
SDL_Rect previewrect = {width / 2 - 50, 50, 100, 100};
SDL_Rect previewtextrect = {width / 2 - 50, 160, 100, 40};

// Buttons
SDL_Rect saverect = {50, height - 160, 200, 50};
SDL_Rect loadrect = {50, height - 100, 200, 50};
SDL_Rect exitrect = {width - 250, height - 100, 200, 50};

namespace player {
    void inventoryevent(SDL_Event event, bool &inventory) {
        if(event.key.keysym.sym == SDLK_e)
            inventory = !inventory;
    }
    void mouseinvchooser(SDL_Event event, bool inventory, bool &running, int worldmap[250][250], int mapwidth, int mapheight, int &curblock, int &bgcolor, int mousex, int mousey) {
        if (event.type == SDL_MOUSEBUTTONDOWN && inventory) {
            // Color
            if (mousex >= selclrect.x && mousex <= selclrect.x + selclrect.w &&
                mousey >= selclrect.y && mousey <= selclrect.y + selclrect.h) {
                if (curblock == 10)
                    curblock = 25;
                else
                    curblock -= 1;
            }
            else if (mousex >= selcrrect.x && mousex <= selcrrect.x + selcrrect.w &&
                mousey >= selcrrect.y && mousey <= selcrrect.y + selcrrect.h) {
                if (curblock == 25)
                    curblock = 10;
                else
                    curblock += 1;
            }

            // BG Color
            if (mousex >= selbclrect.x && mousex <= selbclrect.x + selbclrect.w &&
                mousey >= selbclrect.y && mousey <= selbclrect.y + selbclrect.h) {
                if (bgcolor == 0)
                    bgcolor = 15;
                else
                    bgcolor -= 1;
            }
            else if (mousex >= selbcrrect.x && mousex <= selbcrrect.x + selbcrrect.w &&
                mousey >= selbcrrect.y && mousey <= selbcrrect.y + selbcrrect.h) {
                if (bgcolor == 15)
                    bgcolor = 0;
                else
                    bgcolor += 1;
            }

            // Bottom bar
            if (mousex >= saverect.x && mousex <= saverect.x + saverect.w &&
                mousey >= saverect.y && mousey <= saverect.y + saverect.h)
                files::savemapevent(event, worldmap, mapwidth, mapheight);
            else if (mousex >= loadrect.x && mousex <= loadrect.x + loadrect.w &&
                mousey >= loadrect.y && mousey <= loadrect.y + loadrect.h)
                files::loadmapevent(event, worldmap, mapwidth, mapheight);
            else if (mousex >= exitrect.x && mousex <= exitrect.x + exitrect.w &&
                mousey >= exitrect.y && mousey <= exitrect.y + exitrect.h)
                running = false;
        }
    }
}

namespace overlay {
    void inventory(SDL_Renderer* renderer, TTF_Font* font, bool inventory, int curblock, int bgcolor, int mousex, int mousey) {
        // Define variables
        int colorr, colorg, colorb = 0;
        SDL_Color textcolor = {255, 255, 255};

        if (inventory) {
            // Render bg
            graphics::getColor(8, colorr, colorg, colorb);
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_Rect bgrect = {25, 25, width - 50, height - 50};
            SDL_RenderFillRect(renderer, &bgrect);

            // Color
            draw::drawButton(renderer, selclrect, 15, 7, mousex, mousey);
            draw::drawButton(renderer, selcrrect, 15, 7, mousex, mousey);
            draw::drawPreview(renderer, colorrect, curblock - 10);
            draw::drawText(renderer, font, colortextrect, "Block", textcolor);

            // BG Color
            draw::drawButton(renderer, selbclrect, 15, 7, mousex, mousey);
            draw::drawButton(renderer, selbcrrect, 15, 7, mousex, mousey);
            draw::drawPreview(renderer, bgcolorrect, bgcolor);
            draw::drawText(renderer, font, bgcolortextrect, "BG", textcolor);

            // Preview
            draw::drawPreview(renderer, previewrect, curblock - 10);
            draw::drawText(renderer, font, previewtextrect, "Preview", textcolor);

            // Save button
            draw::drawButton(renderer, saverect, 2, 3, mousex, mousey);
            draw::drawText(renderer, font, saverect, "save", textcolor);

            // Load button
            draw::drawButton(renderer, loadrect, 9, 1, mousex, mousey);
            draw::drawText(renderer, font, loadrect, "load", textcolor);

            // Exit button
            draw::drawButton(renderer, exitrect, 9, 1, mousex, mousey);
            draw::drawText(renderer, font, exitrect, "exit", textcolor);
        }
    }
}
