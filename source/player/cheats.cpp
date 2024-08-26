#include "SDL2/SDL.h"

#include "block.h"

namespace cheats {
	void CamTp(SDL_Event event, bool &target, bool &highlight, int curHoverX, int curHoverY, int &targetX, int &targetY) {
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			targetX = curHoverX;
			targetY = curHoverY;

			target = false;
			highlight = false;
		}
	}
	void PlayerTp(SDL_Event event, Block worldMap[250][250], bool &target, bool &highlight, int curHoverX, int curHoverY, int &targetX, int &targetY) {
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			worldMap[targetX][targetY].type = 0;
			targetX = curHoverX;
			targetY = curHoverY;

			target = false;
			highlight = false;
		}
	}
}
