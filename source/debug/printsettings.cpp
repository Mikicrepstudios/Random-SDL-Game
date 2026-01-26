#include <iostream>

#include "settings.h"

namespace debug {
    void PrintSettings(game::Game &game) {
        /**
         * @brief Prints all values from game::Settings struct
         * @param settings Game settings
         */
        std::cout << "DEBUG - SETTINGS" << std::endl;
        std::cout << "Cur preset: " << game.settings.curPreset << std::endl
                  << "Block color: " << game.settings.blockColor << std::endl
                  << "Block texture ID: " << game.settings.blockTextureId << std::endl
                  << "BG Color: " << game.settings.bgColor << std::endl
                  << "Player Color: " << game.settings.playerColor << std::endl
                  << "Can player place: " << game.settings.canPlayerPlace << std::endl
                  << "Place solid blocks: " << game.settings.placeSolidBlocks << std::endl
                  << "Inventory: " << game.settings.inventory << std::endl
                  << "Color picker ID: " << game.settings.colorPickerId << std::endl
                  << "Color picker: " << game.settings.colorPicker << std::endl
                  << "Texture picker: " << game.settings.texturePicker << std::endl
                  << "Dialogue ID: " << game.settings.dialogueId << std::endl
                  << "Dialogue: " << game.settings.dialogue << std::endl
                  << "Cheats ID: " << game.settings.cheatsId << std::endl
                  << "Cheats: " << game.settings.cheats << std::endl
                  << "Game info: " << game.settings.gameInfo << std::endl;
    }
}