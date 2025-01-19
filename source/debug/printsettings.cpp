#include <iostream>

#include "settings.h"

namespace debug {
    void PrintSettings(game::Settings settings) {
        std::cout << settings.curPreset << std::endl
                  << settings.blockColor << std::endl
                  << settings.blockTextureId << std::endl
                  << settings.bgColor << std::endl
                  << settings.playerColor << std::endl
                  << settings.canPlayerPlace << std::endl
                  << settings.placeSolidBlocks << std::endl
                  << settings.inventory << std::endl
                  << settings.colorPickerId << std::endl
                  << settings.colorPicker << std::endl
                  << settings.texturePicker << std::endl
                  << settings.dialogueId << std::endl
                  << settings.dialogue << std::endl
                  << settings.cheatsId << std::endl
                  << settings.cheats << std::endl
                  << settings.gameInfo << std::endl;
    }
}