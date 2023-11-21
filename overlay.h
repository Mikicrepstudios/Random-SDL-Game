#pragma once

namespace overlay {
    extern void inventory(SDL_Renderer* renderer, int width, int height, bool inventory, int curblock);
    extern void mouse(SDL_Renderer* renderer, bool inventory, int worldmap[250][250], int mapwidth, int mapheight, int curhoverx, int curhovery, int camoffsetx, int camoffsety, int camscale);
}
