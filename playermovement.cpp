#include "SDL2/SDL.h"

namespace player {
    void playermovement(SDL_Event event, int worldmap[250][250], int mapwidth, int mapheight, int playerspeed, int &playerx, int &playery) {
        if(event.key.keysym.sym == SDLK_w) {
            if (worldmap[playerx][playery - playerspeed] == 0 && playery != 0) {
                worldmap[playerx][playery] = 0;
                playery -= playerspeed;
            }
        }
        else if(event.key.keysym.sym == SDLK_a) {
            if (worldmap[playerx - playerspeed][playery] == 0 && playerx != 0) {
                worldmap[playerx][playery] = 0;
                playerx -= playerspeed;
            }
        }
        else if(event.key.keysym.sym == SDLK_s && playery != mapheight) {
            if (worldmap[playerx][playery + playerspeed] == 0) {
                worldmap[playerx][playery] = 0;
                playery += playerspeed;
            }
        }
        else if(event.key.keysym.sym == SDLK_d && playerx != mapwidth) {
            if (worldmap[playerx + playerspeed][playery] == 0) {
                worldmap[playerx][playery] = 0;
                playerx += playerspeed;
            }
        }
    }
}
