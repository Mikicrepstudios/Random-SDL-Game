#pragma once

#include "presets.h"
#include "settings.h"

/**
 * @brief Namespace that contains all game logic related stuff
 */
namespace logic {
    void UpdateVars(game::Settings &settings, game::Player &player, game::Camera &cam, game::Preset preset[10]);
}