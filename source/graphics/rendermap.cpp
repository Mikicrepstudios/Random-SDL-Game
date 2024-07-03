#include "SDL2/SDL.h"

#include "block.h"
#include "graphics.h"

namespace game {
	void RenderMap(SDL_Renderer* renderer, Block worldMap[250][250], int width, int height, int mapWidth, int mapHeight, int camOffSetX, int camOffSetY, int camScale) {
		int maxXBlocks = width / camScale;
		int maxYBlocks = height / camScale;
		for(int x = 0; x <= maxXBlocks; x++) {
			for(int y = 0; y <= maxYBlocks; y++) {
				if((camOffSetX - width / camScale) > -mapWidth) {
				// Get current block id and define variables
				int objectId = worldMap[x - camOffSetX][y - camOffSetY].type;
				int colorId = worldMap[x - camOffSetX][y - camOffSetY].color;
				int colorR, colorG, colorB = 0;

				// Get color data
				if (objectId != 0) {
					graphics::GetColor(colorId, colorR, colorG, colorB);

					// Do rendering
					SDL_SetRenderDrawColor(renderer, colorR, colorG, colorB, 255);
					SDL_Rect curRect = {x * camScale, y * camScale, camScale, camScale};
					SDL_RenderFillRect(renderer, &curRect);
				}
				}
			}
		}
	}
}
