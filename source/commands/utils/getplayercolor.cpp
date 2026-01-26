#include <iostream>

#include "settings.h"

namespace commands {
    int GetPlayerColor(game::Game &game) {
        /**
         * @brief Function for GetPlayerColor command
         * @param player Game player
         */
        
        std::cout << "Player color: " << game.player.color << std::endl;

        return 0;
    }
}