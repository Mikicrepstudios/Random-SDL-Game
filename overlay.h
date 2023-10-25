#pragma once

namespace overlay {
    extern void inventory(SDL_Renderer* renderer, int width, int height, bool inventory, int curblock);
    extern void mouse(SDL_Renderer* renderer, int worldmap[250][250], int curhoverx, int curhovery);
}
