#include "SDL.h"

#include "mf/core.h"

#include "settings.h"

namespace game {
	void CameraControls(core::MF_Window &window, game::Settings settings, game::Map map, game::Camera &cam) {
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
			// - because offset is negative
			else if(window.event.key.keysym.sym == SDLK_DOWN && cam.offSetY > -((map.height - 1) - window.height / cam.scale)) {
				cam.offSetY -= 1;
			}
			else if(window.event.key.keysym.sym == SDLK_RIGHT && cam.offSetX > -((map.width - 1) - window.width / cam.scale)) {
				cam.offSetX -= 1;
			}
		}
		// Cam zoom out
		if((window.event.key.keysym.sym == SDLK_o || window.event.key.keysym.sym == SDLK_KP_MINUS) && cam.scale != 10) {
			cam.scale -= 1;

			// Push camera back if it goes out of bounds
			while(cam.offSetX < -((map.width - 1) - window.width / cam.scale)) {
				cam.offSetX += 1;
			}
			while(cam.offSetY < -((map.height - 1) - window.height / cam.scale)) {
				cam.offSetY += 1;
			}
		}
		// Cam zoom in
		else if(window.event.key.keysym.sym == SDLK_i || window.event.key.keysym.sym == SDLK_KP_PLUS){
			cam.scale += 1;
		}
		// Reset zoom
		else if(window.event.key.keysym.sym == SDLK_p) {
			cam.scale = 50;

			// Push camera back if it goes out of bounds
			while(cam.offSetX < -((map.width - 1) - window.width / cam.scale)) {
				cam.offSetX += 1;
			}
			while(cam.offSetY < -((map.height - 1) - window.height / cam.scale)) {
				cam.offSetY += 1;
			}
		}
    }
}
