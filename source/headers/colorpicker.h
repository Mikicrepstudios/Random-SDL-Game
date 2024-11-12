#pragma once

#include "mf/core.h"

#include "settings.h"

namespace colorpicker {
    void Overlay(core::MF_Window &window, game::Settings settings);
	int Event(core::MF_Window &window, game::Settings &settings);
}