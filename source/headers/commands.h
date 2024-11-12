#pragma once

#include <string>

#include "mf/core.h"

#include "presets.h"
#include "settings.h"

namespace commands {
    void Executor(std::string command, core::MF_Window &window, game::Settings &settings, game::Map &map, game::Preset preset[10]);

    // Utils
    extern int Echo(std::string args);
    extern int GetBGColor(int bgColor);
    extern int GetBlockType(std::string args, int argsCount, game::Map map);
    extern int GetBlockColor(std::string args, int argsCount, game::Map map);
    extern int IsBlockSolid(std::string args, int argsCount, game::Map map);

    // Player
    extern int SetBGColor(std::string args, int argsCount, game::Settings &settings);
    extern int SetBlockColor(std::string args, int argsCount, game::Settings &settings, game::Preset preset[10]);
    extern int SetPlayerColor(std::string args, int argsCount, game::Settings &settings);

    // World
    extern int Destroy(std::string args, int argsCount, game::Map &map);
    extern int Place(std::string args, int argsCount, game::Settings settings, game::Map &map);
}