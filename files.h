#pragma once

namespace files {
    extern void savemapevent(SDL_Event event, int worldmap[250][250], int mapwidth, int mapheight);
    extern void loadmapevent(SDL_Event event, int worldmap[250][250], int mapwidth, int mapheight);
}
