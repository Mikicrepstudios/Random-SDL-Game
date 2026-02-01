#pragma once

#include <string>

#include "mf/core.h"

#include "game.h"
#include "settings.h"

/**
 * @brief Namespace that contains all in game commands
 */
namespace commands {
    void Executor(std::string command, core::MF_Window &window, game::Game &game, game::Preset preset[10]);

    // Utils
    int Echo(std::string args);
    int GetBGColor(int bgColor);
    int GetBlockType(std::string args, int argsCount, game::Game &game);
    int GetBlockColor(std::string args, int argsCount, game::Game &game);
    int GetBlockTexture(std::string args, int argsCount, game::Game &game);
    int GetCamPos(game::Game &game);
    int GetPlayerColor(game::Game &game);
    int GetPlayerPos(game::Game &game);
    int IsBlockSolid(std::string args, int argsCount, game::Game &game);
    int TerminalMode(std::string args, int argsCount, bool &terminalmode);

    // Player
    int SetBGColor(std::string args, int argsCount, game::Game &game);
    int SetBlockColor(std::string args, int argsCount, game::Game &game, game::Preset preset[10]);
    int SetBlockTexture(std::string args, int argsCount, game::Game &game, game::Preset preset[10]);
    int SetPlayerColor(std::string args, int argsCount, game::Game &game);
    int TpCam(std::string args, int argsCount, game::Game &game);
    int TpPlayer(std::string args, int argsCount, game::Game &game);

    // World
    int Destroy(std::string args, int argsCount, game::Game &game);
    int Place(std::string args, int argsCount, game::Game &game);
}