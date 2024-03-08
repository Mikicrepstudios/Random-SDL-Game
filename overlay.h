#pragma once

namespace overlay {
    extern void inventory(SDL_Renderer* renderer, TTF_Font* font, bool inventory, int curblock, int bgcolor, int mousex, int mousey);
    extern void mouse(SDL_Renderer* renderer, bool inventory, int worldmap[250][250], int mapwidth, int mapheight, int curhoverx, int curhovery, int camoffsetx, int camoffsety, int camscale);
}
