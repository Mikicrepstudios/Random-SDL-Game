#pragma once

#include "presets.h"
#include "settings.h"

namespace mouse {
    // Function for placing blocks
    extern void Event(SDL_Event event, game::SDL_Settings sdlSettings, game::Settings &settings, game::Map &map, game::Camera &camera, game::Preset preset[10]);

    // Function for highlighting current mouse position
    extern void Overlay(game::SDL_Settings sdlSettings, game::Settings settings, game::Map map, game::Camera camera);
}