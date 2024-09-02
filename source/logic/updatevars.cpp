#include "presets.h"
#include "settings.h"

namespace logic {
    void UpdateVars(game::Settings &settings, game::Player &player, game::Preset preset[10]) {
        settings.blockColor = preset[settings.curPreset].blockColor;
        player.color = settings.playerColor;
    }
}