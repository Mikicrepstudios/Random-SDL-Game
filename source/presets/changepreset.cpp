#include "SDL2/SDL.h"

namespace controls {
	void presetChooser(SDL_Event event, int &preset) {
		if(event.key.keysym.sym == SDLK_1) {
			preset = 1;
		}
		else if(event.key.keysym.sym == SDLK_2) {
			preset = 2;
		}
		else if(event.key.keysym.sym == SDLK_3) {
			preset = 3;
		}
		else if(event.key.keysym.sym == SDLK_4) {
			preset = 4;
		}
		else if(event.key.keysym.sym == SDLK_5) {
			preset = 5;
		}
		else if(event.key.keysym.sym == SDLK_6) {
			preset = 6;
		}
		else if(event.key.keysym.sym == SDLK_7) {
			preset = 7;
		}
		else if(event.key.keysym.sym == SDLK_8) {
			preset = 8;
		}
		else if(event.key.keysym.sym == SDLK_9) {
			preset = 9;
		}
		else if(event.key.keysym.sym == SDLK_0) {
			preset = 10;
		}
	}
}
