#include "settings.h"

namespace inventory {
    void Event(SDL_Event event, game::Game &game) {
		/**
		 * @brief This function opens and closes inventory and its submenus
		 * @param event SDL_Event
		 * @param settings Game settings
		 */

		auto& settings = game.settings;
		
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