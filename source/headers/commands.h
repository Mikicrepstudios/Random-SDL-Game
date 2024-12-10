#pragma once

#include <string>

#include "mf/core.h"

#include "game.h"
#include "settings.h"

/**
 * @brief Namespace that contains all in game commands
 */
namespace commands {
    void Executor(std::string command, core::MF_Window &window, game::Settings &settings, game::Map &map, game::Preset preset[10]);

    // Utils
    int Echo(std::string args);
    int GetBGColor(int bgColor);
    int GetBlockType(std::string args, int argsCount, game::Map map);
    int GetBlockColor(std::string args, int argsCount, game::Map map);
    int IsBlockSolid(std::string args, int argsCount, game::Map map);

    // Player
    int SetBGColor(std::string args, int argsCount, game::Settings &settings);
    int SetBlockColor(std::string args, int argsCount, game::Settings &settings, game::Preset preset[10]);
    int SetPlayerColor(std::string args, int argsCount, game::Settings &settings);

    // World
    int Destroy(std::string args, int argsCount, game::Map &map);
    int Place(std::string args, int argsCount, game::Settings settings, game::Map &map);
}