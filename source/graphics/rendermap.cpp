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
		 * @brief Renders all blocks visible on screen based on camera position and zoom.
		 * 
		 * @param window          Game window with SDL renderer and window size info.
		 * @param settings        Game settings, including player color and other configs.
		 * @param map             2D map of block data to render.
		 * @param cam             Camera controlling zoom (scale) and offset in the world.
		 * @param blockTextures   Array of textures for each block type.
		 */
		
		// Calculate how many blocks fit on screen based on camera scale
		int maxXBlocks = (window.width + cam.scale - 1) / cam.scale;
		int maxYBlocks = (window.height + cam.scale - 1) / cam.scale;

		for (int x = 0; x < maxXBlocks; x++) {
			for (int y = 0; y < maxYBlocks; y++) {
				// World-space coordinates (adjusted by camera offset)
				int worldX = x + cam.offSetX;
				int worldY = y + cam.offSetY;

				// Skip out-of-bounds map access
				if (worldX < 0 || worldY < 0 || worldX >= map.width || worldY >= map.height)
					continue;

				// Get block data at world position
				int objectId = map.map[worldX][worldY].type;       // Type of block (0 = air, 1 = player, etc.)
				int colorId  = map.map[worldX][worldY].color;      // Color index for the block (1-based)
				int textureId = map.map[worldX][worldY].texture;   // Texture ID for this block

				if (objectId != 0) {
					// Define screen-space rectangle where the block should be rendered
					SDL_Rect curRect = {
						x * cam.scale,  // Screen X
						y * cam.scale,  // Screen Y
						cam.scale,      // Width
						cam.scale       // Height
					};

					// Draw colored rectangle background
					if (colorId > 0 && colorId <= 32) {
						draw::DrawRect(window.renderer, curRect, colors::colorID[colorId - 1]);
					}

					// Draw texture overlay if available
					if (textureId != 0)
						SDL_RenderCopy(window.renderer, blockTextures[textureId].texture, NULL, &curRect);

					// Special player rendering overlay
					if (objectId == 1)
						draw::DrawRect(window.renderer, curRect, colors::colorID[settings.playerColor - 1]);
				}
			}
		}
	}
}
