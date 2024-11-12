#include <iostream>
#include "SDL2/SDL.h"
#include <SDL_ttf.h>

#include "mf/core.h"
#include "mf/colors.h"

namespace draw {
	void SetCol(SDL_Renderer* renderer, int bgColor) {
		SDL_SetRenderDrawColor(renderer, colors::colorID[bgColor - 1].r, colors::colorID[bgColor - 1].g, colors::colorID[bgColor - 1].b, 255);
	}
}
