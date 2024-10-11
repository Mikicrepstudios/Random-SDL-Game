#pragma once

#include "presets.h"
#include "settings.h"

namespace logic {
    extern void UpdateVars(game::Settings &settings, game::Player &player, game::Camera &cam, game::Preset preset[10]);
}