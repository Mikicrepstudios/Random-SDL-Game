#include "SDL2/SDL.h"
#include <SDL_ttf.h>

#include "files.h"
#include "graphics.h"

SDL_Rect selclrect = {50, 50, 20, 80};
SDL_Rect selcrrect = {170, 50, 20, 80};
SDL_Rect colorrect = {80, 50, 80, 80};
SDL_Rect colortextrect = {80, 140, 80, 40};

SDL_Rect selbclrect = {1280 - 190, 50, 20, 80};
SDL_Rect selbcrrect = {1280 - 70, 50, 20, 80};
SDL_Rect bgcolorrect = {1280 - 160, 50, 80, 80};
SDL_Rect bgcolortextrect = {1280 - 160, 140, 80, 40};

SDL_Rect saverect = {50, 800 - 160, 200, 50};
SDL_Rect loadrect = {50, 800 - 100, 200, 50};
SDL_Rect exitrect = {1280 - 250, 800 - 100, 200, 50};

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
    void inventory(SDL_Renderer* renderer, TTF_Font* font, int width, int height, bool inventory, int curblock, int bgcolor, int mousex, int mousey) {
        // Define variables
        int colorr, colorg, colorb = 0;
        SDL_Color textcolor = {255, 255, 255};

        if (inventory) {
            // Rects

            SDL_Rect previewrect = {width / 2 - 50, 50, 100, 100};

            // Render bg
            graphics::getColor(8, colorr, colorg, colorb);
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_Rect bgrect = {25, 25, width - 50, height - 50};
            SDL_RenderFillRect(renderer, &bgrect);

            // Selector arrows
            // Color left
            if (mousex >= selclrect.x && mousex <= selclrect.x + selclrect.w &&
                mousey >= selclrect.y && mousey <= selclrect.y + selclrect.h)
                graphics::getColor(7, colorr, colorg, colorb);
            else
                graphics::getColor(15, colorr, colorg, colorb);
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_RenderFillRect(renderer, &selclrect);
            // Color right
            if (mousex >= selcrrect.x && mousex <= selcrrect.x + selcrrect.w &&
                mousey >= selcrrect.y && mousey <= selcrrect.y + selcrrect.h)
                graphics::getColor(7, colorr, colorg, colorb);
            else
                graphics::getColor(15, colorr, colorg, colorb);
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_RenderFillRect(renderer, &selcrrect);
            // Color preview
            graphics::getColor(curblock - 10, colorr, colorg, colorb);
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_RenderFillRect(renderer, &colorrect);
            // Color text
            SDL_Surface* colorsurface = TTF_RenderText_Solid(font, "Block", textcolor);
            SDL_Texture* colortexture = SDL_CreateTextureFromSurface(renderer, colorsurface);
            SDL_RenderCopy(renderer, colortexture, NULL, &colortextrect);

            // BG Color left
            if (mousex >= selbclrect.x && mousex <= selbclrect.x + selbclrect.w &&
                mousey >= selbclrect.y && mousey <= selbclrect.y + selbclrect.h)
                graphics::getColor(7, colorr, colorg, colorb);
            else
                graphics::getColor(15, colorr, colorg, colorb);
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_RenderFillRect(renderer, &selbclrect);
            // BG Color right
            if (mousex >= selbcrrect.x && mousex <= selbcrrect.x + selbcrrect.w &&
                mousey >= selbcrrect.y && mousey <= selbcrrect.y + selbcrrect.h)
                graphics::getColor(7, colorr, colorg, colorb);
            else
                graphics::getColor(15, colorr, colorg, colorb);
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_RenderFillRect(renderer, &selbcrrect);
            // BG Color preview
            graphics::getColor(bgcolor, colorr, colorg, colorb);
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_RenderFillRect(renderer, &bgcolorrect);
            // BG Color text
            SDL_Surface* bgcolorsurface = TTF_RenderText_Solid(font, "BG", textcolor);
            SDL_Texture* bgcolortexture = SDL_CreateTextureFromSurface(renderer, bgcolorsurface);
            SDL_RenderCopy(renderer, bgcolortexture, NULL, &bgcolortextrect);

            // Current block preview
            graphics::getColor(curblock - 10, colorr, colorg, colorb);
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_RenderFillRect(renderer, &previewrect);

            // Save button
            if (mousex >= saverect.x && mousex <= saverect.x + saverect.w &&
                mousey >= saverect.y && mousey <= saverect.y + saverect.h)
                graphics::getColor(3, colorr, colorg, colorb);
            else
                graphics::getColor(2, colorr, colorg, colorb);
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_RenderFillRect(renderer, &saverect);

            SDL_Surface* savesurface = TTF_RenderText_Solid(font, "save", textcolor);
            SDL_Texture* savetexture = SDL_CreateTextureFromSurface(renderer, savesurface);
            SDL_RenderCopy(renderer, savetexture, NULL, &saverect);
            // Load button
            if (mousex >= loadrect.x && mousex <= loadrect.x + loadrect.w &&
                mousey >= loadrect.y && mousey <= loadrect.y + loadrect.h)
                graphics::getColor(1, colorr, colorg, colorb);
            else
                graphics::getColor(9, colorr, colorg, colorb);
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_RenderFillRect(renderer, &loadrect);

            SDL_Surface* loadsurface = TTF_RenderText_Solid(font, "load", textcolor);
            SDL_Texture* loadtexture = SDL_CreateTextureFromSurface(renderer, loadsurface);
            SDL_RenderCopy(renderer, loadtexture, NULL, &loadrect);
            // Exit button
            if (mousex >= exitrect.x && mousex <= exitrect.x + exitrect.w &&
                mousey >= exitrect.y && mousey <= exitrect.y + exitrect.h)
                graphics::getColor(1, colorr, colorg, colorb);
            else
                graphics::getColor(9, colorr, colorg, colorb);
            SDL_SetRenderDrawColor(renderer, colorr, colorg, colorb, 255);
            SDL_RenderFillRect(renderer, &exitrect);

            SDL_Surface* exitsurface = TTF_RenderText_Solid(font, "exit", textcolor);
            SDL_Texture* exittexture = SDL_CreateTextureFromSurface(renderer, exitsurface);
            SDL_RenderCopy(renderer, exittexture, NULL, &exitrect);
        }
    }
}
