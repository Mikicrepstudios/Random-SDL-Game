#include <iostream>

#include "settings.h"

namespace debug {
    void PrintGame(game::Game game) {
        std::cout << "Terminal Mode: " << game.terminalmode << std::endl
                  << "CLI input: " << game.cliInput << std::endl
                  << "Cur hover X: " << game.curHoverX << std::endl
                  << "Cur hover Y: " << game.curHoverY << std::endl;
    }
}