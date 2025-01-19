#include <iostream>

#include "settings.h"

namespace debug {
    void printGame(game::Game game) {
        std::cout << game.terminalmode << std::endl
                  << game.cliInput << std::endl
                  << game.curHoverX << std::endl
                  << game.curHoverY << std::endl;
    }
}