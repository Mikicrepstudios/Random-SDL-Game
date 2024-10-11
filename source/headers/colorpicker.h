#pragma once

#include "settings.h"

namespace colorpicker {
    extern void Overlay(game::SDL_Settings sdlSettings, game::Settings settings);
	extern int Event(game::SDL_Settings sdlSettings, game::Settings &settings);
}