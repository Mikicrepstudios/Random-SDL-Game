#include "SDL2/SDL.h"

namespace player {
    void playermovement(SDL_Event event, int worldmap[250][250], int mapwidth, int mapheight, int playerspeed, int &playerx, int &playery) {
        if(event.key.keysym.sym == SDLK_w && worldmap[playerx][playery - playerspeed] == 0 && playery != 0) {
            worldmap[playerx][playery] = 0;
            playery -= playerspeed;
        }
        else if(event.key.keysym.sym == SDLK_a && worldmap[playerx - playerspeed][playery] == 0 && playerx != 0) {
            worldmap[playerx][playery] = 0;
            playerx -= playerspeed;
        }
        else if(event.key.keysym.sym == SDLK_s && playery != mapheight && worldmap[playerx][playery + playerspeed] == 0) {
            worldmap[playerx][playery] = 0;
            playery += playerspeed;
        }
        else if(event.key.keysym.sym == SDLK_d && playerx != mapwidth && worldmap[playerx + playerspeed][playery] == 0) {
            worldmap[playerx][playery] = 0;
            playerx += playerspeed;
        }
    }
}
