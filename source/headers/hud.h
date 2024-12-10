#pragma once

#include "mf/core.h"

#include <string>
#include "settings.h"

/**
 * @brief Namespace that contains all HUD related stuff
 */
namespace hud {
    void GameInfo(core::MF_Window &window, game::Settings settings, game::Camera cam, game::Player player);
}