#include <iostream>

#include "settings.h"

namespace debug {
    void PrintSettings(game::Settings settings) {
        std::cout << "DEBUG - SETTINGS" << std::endl;
        std::cout << "Cur preset: " << settings.curPreset << std::endl
                  << "Block color: " << settings.blockColor << std::endl
                  << "Block texture ID: " << settings.blockTextureId << std::endl
                  << "BG Color: " << settings.bgColor << std::endl
                  << "Player Color: " << settings.playerColor << std::endl
                  << "Can player place: " << settings.canPlayerPlace << std::endl
                  << "Place solid blocks: " << settings.placeSolidBlocks << std::endl
                  << "Inventory: " << settings.inventory << std::endl
                  << "Color picker ID: " << settings.colorPickerId << std::endl
                  << "Color picker: " << settings.colorPicker << std::endl
                  << "Texture picker: " << settings.texturePicker << std::endl
                  << "Dialogue ID: " << settings.dialogueId << std::endl
                  << "Dialogue: " << settings.dialogue << std::endl
                  << "Cheats ID: " << settings.cheatsId << std::endl
                  << "Cheats: " << settings.cheats << std::endl
                  << "Game info: " << settings.gameInfo << std::endl;
    }
}