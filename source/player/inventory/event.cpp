#include "settings.h"

namespace inventory {
    void Event(SDL_Event event, game::Settings &settings) {
		if(event.key.keysym.sym == SDLK_e) {
			if (!settings.inventory) {
				settings.inventory = true;
			}
			else {
				// Before stopping inv exit every sub UI
				settings.colorPicker = false;
				settings.texturePicker = false;
				settings.inventory = false;
			}
		}
	}
}