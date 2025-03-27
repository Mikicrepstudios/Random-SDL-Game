#pragma once

#include <string>

#include "mf/core.h"

#include "game.h"
#include "settings.h"

/**
 * @brief Namespace that contains all in game commands
 */
namespace commands {
    void Executor(std::string command, core::MF_Window &window, game::Game &game, game::Settings &settings, game::Map &map, game::Player &player, game::Camera &cam, game::Preset preset[10]);

    // Utils
    int Echo(std::string args);
    int GetBGColor(int bgColor);
    int GetBlockType(std::string args, int argsCount, game::Map map);
    int GetBlockColor(std::string args, int argsCount, game::Map map);
    int GetBlockTexture(std::string args, int argsCount, game::Map map);
    int GetCamPos(game::Camera cam);
    int GetPlayerColor(game::Player player);
    int GetPlayerPos(game::Player player);
    int IsBlockSolid(std::string args, int argsCount, game::Map map);
    int TerminalMode(std::string args, int argsCount, bool &terminalmode);

    // Player
    int SetBGColor(std::string args, int argsCount, game::Settings &settings);
    int SetBlockColor(std::string args, int argsCount, game::Settings &settings, game::Preset preset[10]);
    int SetBlockTexture(std::string args, int argsCount, game::Settings &settings, game::Preset preset[10]);
    int SetPlayerColor(std::string args, int argsCount, game::Settings &settings);
    int TpCam(std::string args, int argsCount, game::Camera &cam);
    int TpPlayer(std::string args, int argsCount, game::Map &map, game::Player &player);

    // World
    int Destroy(std::string args, int argsCount, game::Map &map);
    int Place(std::string args, int argsCount, game::Settings settings, game::Map &map);
}