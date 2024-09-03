#include "SDL2/SDL.h"

#include "settings.h"

namespace events {
	void Camera(game::SDL_Settings sdlSettings, game::Settings settings, game::Camera &cam) {
		if (!settings.inventory) {
			if(sdlSettings.event.key.keysym.sym == SDLK_UP && cam.offSetY != 0) {
				cam.offSetY += 1;
			}
			else if(sdlSettings.event.key.keysym.sym == SDLK_LEFT && cam.offSetX != 0) {
				cam.offSetX += 1;
			}
			else if(sdlSettings.event.key.keysym.sym == SDLK_DOWN) {
				cam.offSetY -= 1;
			}
			else if(sdlSettings.event.key.keysym.sym == SDLK_RIGHT) {
				cam.offSetX -= 1;
			}
		}
		// Cam zoom out
		if(sdlSettings.event.key.keysym.sym == SDLK_o && cam.scale != 10) {
			cam.scale -= 1;
		}
		// Cam zoom in
		else if(sdlSettings.event.key.keysym.sym == SDLK_i) {
			cam.scale += 1;
		}
		// Reset zoom
		else if(sdlSettings.event.key.keysym.sym == SDLK_p) {
			cam.scale = 50;
		}
    }
}
