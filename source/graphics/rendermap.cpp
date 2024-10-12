#include "SDL2/SDL.h"

#include "draw.h"
#include "block.h"
#include "settings.h"
#include "textures.h"

namespace game {
	void RenderMap(game::SDL_Settings sdlSettings, game::Settings settings, game::Map map, game::Camera cam, textures::BlockTextures blockTextures[32]) {
		int maxXBlocks = sdlSettings.width / cam.scale;
		int maxYBlocks = sdlSettings.height / cam.scale;
		for(int x = 0; x <= maxXBlocks; x++) {
			for(int y = 0; y <= maxYBlocks; y++) {
				if((cam.offSetX - sdlSettings.width / cam.scale) > -map.width) {
				// Get current block id and define variables
				int objectId = map.map[x - cam.offSetX][y - cam.offSetY].type;
				int colorId = map.map[x - cam.offSetX][y - cam.offSetY].color;
				int textureId = map.map[x - cam.offSetX][y - cam.offSetY].texture;
				int colorR, colorG, colorB = 0;

				// Get color data
				if(objectId != 0) {
					SDL_Rect curRect = {x * cam.scale, y * cam.scale, cam.scale, cam.scale};
					draw::DrawRect(sdlSettings.renderer, curRect, colorId);

					if(textureId != 0)
						SDL_RenderCopy(sdlSettings.renderer, blockTextures[textureId].texture, NULL, &curRect);

					if(objectId == 1) draw::DrawRect(sdlSettings.renderer, curRect, settings.playerColor);
				}
				}
			}
		}
	}
}
