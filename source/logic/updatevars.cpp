#include "presets.h"
#include "settings.h"

namespace logic {
    void updateVars(game::Settings &settings, settings::Preset preset[10]) {
        settings.blockColor = preset[settings.curPreset].blockColor;
    }
}