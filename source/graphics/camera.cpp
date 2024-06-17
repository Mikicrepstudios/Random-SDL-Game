#include "SDL2/SDL.h"

namespace events {
	void Camera(SDL_Event event, bool inventory, int &camOffSetX, int &camOffSetY, int &camScale) {
		if (!inventory) {
			if(event.key.keysym.sym == SDLK_UP && camOffSetY != 0) {
				camOffSetY += 1;
			}
			else if(event.key.keysym.sym == SDLK_LEFT && camOffSetX != 0) {
				camOffSetX += 1;
			}
			else if(event.key.keysym.sym == SDLK_DOWN) {
				camOffSetY -= 1;
			}
			else if(event.key.keysym.sym == SDLK_RIGHT) {
				camOffSetX -= 1;
			}
		}
		// Cam zoom out
		if(event.key.keysym.sym == SDLK_o && camScale != 10) {
			camScale -= 1;
		}
		// Cam zoom in
		else if(event.key.keysym.sym == SDLK_i) {
			camScale += 1;
		}
		// Reset zoom
		else if(event.key.keysym.sym == SDLK_p) {
			camScale = 50;
		}
    }
}
