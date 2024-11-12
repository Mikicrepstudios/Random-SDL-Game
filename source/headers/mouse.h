#pragma once

#include "mf/core.h"

#include "presets.h"
#include "settings.h"

namespace mouse {
    // Function for placing blocks
    extern void Event(core::MF_Window &window, game::Game game, game::Settings &settings, game::Map &map, game::Camera &cam, game::Preset preset[10]);

    // Function for highlighting current mouse position
    extern void Overlay(core::MF_Window &window, game::Game game, game::Settings settings, game::Map map, game::Camera cam);
}