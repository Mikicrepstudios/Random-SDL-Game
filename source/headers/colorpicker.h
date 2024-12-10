#pragma once

#include "mf/core.h"

#include "settings.h"

/**
 * @brief Namespace that contains all color picker related stuff
 */
namespace colorpicker {
    void Overlay(core::MF_Window &window, game::Settings settings);
	int Event(core::MF_Window &window, game::Settings &settings);
}