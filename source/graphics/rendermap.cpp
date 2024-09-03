#include "SDL2/SDL.h"

#include "draw.h"
#include "block.h"
#include "settings.h"

namespace game {
	void RenderMap(game::SDL_Settings sdlSettings, game::Map map, game::Camera cam) {
		int maxXBlocks = sdlSettings.width / cam.scale;
		int maxYBlocks = sdlSettings.height / cam.scale;
		for(int x = 0; x <= maxXBlocks; x++) {
			for(int y = 0; y <= maxYBlocks; y++) {
				if((cam.offSetX - sdlSettings.width / cam.scale) > -map.width) {
				// Get current block id and define variables
				int objectId = map.map[x - cam.offSetX][y - cam.offSetY].type;
				int colorId = map.map[x - cam.offSetX][y - cam.offSetY].color;
				int colorR, colorG, colorB = 0;

				// Get color data
				if (objectId != 0) {
					SDL_Rect curRect = {x * cam.scale, y * cam.scale, cam.scale, cam.scale};
					draw::DrawRect(sdlSettings.renderer, curRect, colorId);
				}
				}
			}
		}
	}
}
