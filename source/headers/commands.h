#pragma once

#include <string>

#include "presets.h"
#include "settings.h"

namespace commands {
    extern void Executor(std::string command, game::SDL_Settings sdlSettings, game::Settings settings, game::Map &map, game::Preset preset[10]);

    // Utils
    extern int Echo(std::string args);
    extern int GetBGColor(int bgColor);
    extern int GetBlockType(std::string args, int argsCount, game::Map map);
    extern int GetBlockColor(std::string args, int argsCount, game::Map map);
    extern int IsBlockSolid(std::string args, int argsCount, game::Map map);

    // Player
    extern int SetBlockColor(std::string args, int argsCount, game::Settings &settings, game::Preset preset[10]);

    // World
    extern int Destroy(std::string args, int argsCount, game::Map &map);
    extern int Place(std::string args, int argsCount, game::Settings settings, game::Map &map);
}