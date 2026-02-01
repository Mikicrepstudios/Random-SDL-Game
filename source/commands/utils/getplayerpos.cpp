#include <iostream>

#include "settings.h"

namespace commands {
    int GetPlayerPos(game::Game &game) {
        /**
         * @brief Function for GetPlayerPos command
         * @param player Game player
         */
        
        std::cout << "Player X: " << game.player.x << " Player Y: " << game.player.y << std::endl;

        return 0;
    }
}