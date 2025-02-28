#include <iostream>
#include "SDL2/SDL.h"

#include "mf/core.h"
#include "mf/colors.h"
#include "mf/graphics.h"

#include "block.h"
#include "settings.h"
#include "textures.h"

namespace game {
	void RenderMap(core::MF_Window &window, game::Settings settings, game::Map map, game::Camera cam, textures::BlockTextures blockTextures[32]) {
		/**
		 * @brief This function renders all blocks
		 * @param window Game window
		 * @param settings Game settings
		 * @param map Game map
		 * @param cam Game camera
		 * @param blockTextures Block textures
		 */
		
		int maxXBlocks = window.width / cam.scale;
		int maxYBlocks = window.height / cam.scale;
		for(int x = 0; x <= maxXBlocks; x++) {
			for(int y = 0; y <= maxYBlocks; y++) {
				if(x - cam.offSetX < map.width - 1 && y - cam.offSetY < map.height - 1) {
					// Get current block id and define variables
					int objectId = map.map[x - cam.offSetX][y - cam.offSetY].type;
					int colorId = map.map[x - cam.offSetX][y - cam.offSetY].color;
					int textureId = map.map[x - cam.offSetX][y - cam.offSetY].texture;

					// Get color data
					if(objectId != 0) {
						SDL_Rect curRect = {x * cam.scale, y * cam.scale, cam.scale, cam.scale};
						draw::DrawRect(window.renderer, curRect, colors::colorID[colorId - 1]);

						if(textureId != 0)
							SDL_RenderCopy(window.renderer, blockTextures[textureId].texture, NULL, &curRect);

						if(objectId == 1) draw::DrawRect(window.renderer, curRect, colors::colorID[settings.playerColor - 1]);
					}
				}
			}
		}
	}
}
