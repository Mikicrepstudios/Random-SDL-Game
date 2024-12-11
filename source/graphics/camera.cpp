#include "SDL.h"

#include "mf/core.h"

#include "settings.h"

namespace game {
	void CameraControls(core::MF_Window &window, game::Settings settings, game::Camera &cam) {
		/**
		 * @brief This function controls camera movement and other related stuff
		 * @param window Game window
		 * @param settings Game settings
		 * @param cam Game camera
		 */
		
		if (!settings.inventory) {
			if(window.event.key.keysym.sym == SDLK_UP && cam.offSetY != 0) {
				cam.offSetY += 1;
			}
			else if(window.event.key.keysym.sym == SDLK_LEFT && cam.offSetX != 0) {
				cam.offSetX += 1;
			}
			else if(window.event.key.keysym.sym == SDLK_DOWN) {
				cam.offSetY -= 1;
			}
			else if(window.event.key.keysym.sym == SDLK_RIGHT) {
				cam.offSetX -= 1;
			}
		}
		// Cam zoom out
		if(window.event.key.keysym.sym == SDLK_o && cam.scale != 10) {
			cam.scale -= 1;
		}
		// Cam zoom in
		else if(window.event.key.keysym.sym == SDLK_i) {
			cam.scale += 1;
		}
		// Reset zoom
		else if(window.event.key.keysym.sym == SDLK_p) {
			cam.scale = 50;
		}
    }
}
