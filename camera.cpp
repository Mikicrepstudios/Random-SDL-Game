#include "SDL2/SDL.h"

namespace events {
    void camera(SDL_Event event, bool inventory, int &camoffsetx, int &camoffsety, int &camscale) {
        if (!inventory) {
            if(event.key.keysym.sym == SDLK_UP) {
                camoffsety += 1;
            }
            else if(event.key.keysym.sym == SDLK_LEFT) {
                camoffsetx += 1;
            }
            else if(event.key.keysym.sym == SDLK_DOWN) {
                camoffsety -= 1;
            }
            else if(event.key.keysym.sym == SDLK_RIGHT) {
                camoffsetx -= 1;
            }
        }
        if(event.key.keysym.sym == SDLK_o) {
            if (camscale != 1) {
                camscale -= 1;
            }
        }
        else if(event.key.keysym.sym == SDLK_p) {
            camscale += 1;
        }
    }
}
