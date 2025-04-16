#include <iostream>

#include "settings.h"

namespace commands {
    int GetPlayerPos(game::Player player) {
        /**
         * @brief Function for GetPlayerPos command
         * @param player Game player
         */
        
        std::cout << "Player X: " << player.x << " Player Y: " << player.y << std::endl;

        return 0;
    }
}