#pragma once

namespace player {
    extern void playermovement(SDL_Event event, int worldmap[250][250], int playerspeed, int &playerx, int &playery);
    extern void mouseevent(SDL_Event event, int worldmap[250][250], int curhoverx, int curhovery, int &curblock, int camoffsetx, int camoffsety);
    void inventoryevent(SDL_Event event, bool &inventory, int &curblock);
}
