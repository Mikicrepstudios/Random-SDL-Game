#pragma once

#include <string>

#include "settings.h"

namespace commands {
    extern void Executor(std::string command, game::SDL_Settings sdlSettings, game::Settings settings, game::Map &map);

    // Utils
    extern int Echo(std::string args);
    extern int GetBlockType(std::string args, int argsCount, game::Map map);
    extern int GetBlockColor(std::string args, int argsCount, game::Map map);
    extern int IsBlockSolid(std::string args, int argsCount, game::Map map);

    // World
    extern int Place(std::string args, int argsCount, game::Settings settings, game::Map &map);
}