#include "presets.h"
#include "settings.h"

namespace logic {
    void UpdateVars(game::Settings &settings, game::Player &player, game::Camera &cam, game::Preset preset[10]) {
        /**
         * @brief This function updates/links variables on every frame
         * @param settings Game settings
         * @param player Game player
         * @param cam Game camera
         * @param preset Game presets
         */
        
        settings.blockColor = preset[settings.curPreset].blockColor;
        player.color = settings.playerColor;

        // Check canPlayerPlace by seeing are any submenus/events active
        if(
            settings.inventory == true ||
            settings.colorPicker == true ||
            settings.dialogue == true ||
            settings.cheats == true
        ) settings.canPlayerPlace = false;
        else settings.canPlayerPlace = true;
        // Addional checks incase some value in above one is used in multiple ways
        if(
            settings.cheatsId == 3
        ) settings.canPlayerPlace = true;

        // Check if cursor should be highlighted
        if(
            settings.cheats == true
        ) cam.highlight = true;
        else cam.highlight = false;
    }
}