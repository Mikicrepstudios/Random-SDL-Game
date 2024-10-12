#include "settings.h"

namespace gamemap {
	void ClearMap(game::Map &map) {
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
