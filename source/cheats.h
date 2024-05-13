#pragma once

namespace cheats {
    extern void camTp(SDL_Event event, bool &target, bool &highlight, int curHoverX, int curHoverY, int &targetX, int &targetY);
    extern void playerTp(SDL_Event event, Block worldMap[250][250], bool &target, bool &highlight, int curHoverX, int curHoverY, int &targetX, int &targetY);
}
