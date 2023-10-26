#pragma once

namespace events {
    extern void camera(SDL_Event event, bool inventory, int &camoffsetx, int &camoffsety, int &camscale);
}

namespace game {
    extern void rendermap(SDL_Renderer* renderer, int worldmap[250][250], int mapwidth, int mapheight, int camoffsetx, int camoffsety, int camscale);
}

namespace gamemap {
    extern void clearmap(int worldmap[250][250], int mapwidth, int mapheight);
}
