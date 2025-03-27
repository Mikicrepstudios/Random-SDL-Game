#include <iostream>

#include "settings.h"

namespace commands {
    int GetPlayerColor(game::Player player) {
        /**
         * @brief Function for GetPlayerColor command
         * @param player Game player
         */
        
        std::cout << "Player color: " << player.color << std::endl;

        return 0;
    }
}