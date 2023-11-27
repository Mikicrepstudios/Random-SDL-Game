#pragma once

namespace player {
    extern void playermovement(SDL_Event event, int worldmap[250][250], int mapwidth, int mapheight, int playerspeed, int &playerx, int &playery);
    extern void mouseevent(SDL_Event event, bool inventory, int worldmap[250][250], int mapwidth, int mapheight, int curhoverx, int curhovery, int &curblock, int camoffsetx, int camoffsety);
    extern void inventoryevent(SDL_Event event, bool &inventory);
    extern void mouseinvchooser(SDL_Event event, bool inventory, int &curblock, int &bgcolor, int mousex, int mousey);
}
