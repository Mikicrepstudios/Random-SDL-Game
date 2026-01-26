#pragma once

#include "mf/core.h"

#include <string>
#include "settings.h"

/**
 * @brief Namespace that contains all HUD related stuff
 */
namespace gui {
    void ColorPickerOverlay(core::MF_Window &window, game::Game &game);
	int ColorPickerEvent(core::MF_Window &window, game::Game &game);

    void GameInfo(core::MF_Window &window, game::Game &game);
}