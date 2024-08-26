#pragma once

namespace cheats {
	extern void CamTp(SDL_Event event, bool &target, bool &highlight, int curHoverX, int curHoverY, int &targetX, int &targetY);
	extern void PlayerTp(SDL_Event event, Block worldMap[250][250], bool &target, bool &highlight, int curHoverX, int curHoverY, int &targetX, int &targetY);
}
