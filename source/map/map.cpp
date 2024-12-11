#include "settings.h"

namespace game {
	void ClearMap(game::Map &map) {
		/**
		 * @brief This function clears map
		 * @param map Game map which will be cleared
		 */

		for (int x = 0; x < map.width; x++) {
			for (int y = 0; y < map.height; y++) {
				map.map[x][y].type = 0;
				map.map[x][y].color = 0;
				map.map[x][y].texture = 0;
				
				map.map[x][y].isSolid = false;
			}
		}
	}
}
