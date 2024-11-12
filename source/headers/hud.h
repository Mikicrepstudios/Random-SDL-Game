#pragma once

#include "mf/core.h"

#include <string>
#include "settings.h"

namespace hud {
    void GameInfo(core::MF_Window &window, game::Settings settings, game::Camera cam, game::Player player);
}