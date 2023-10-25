#pragma once

namespace game {
    extern void rendermap(SDL_Renderer* renderer, int worldmap[250][250], int mapwidth, int mapheight);
}

namespace gamemap {
    extern void clearmap(int worldmap[250][250], int mapwidth, int mapheight);
}
