#include <iostream>

#include "settings.h"

namespace debug {
    void PrintPlayer(game::Game &game) {
        /**
         * @brief Prints all values from game::Player struct
         * @param player Game player
         */
        std::cout << "DEBUG - PLAYER" << std::endl;
        std::cout << "X: " << game.player.x << std::endl
                  << "Y: " << game.player.y << std::endl
                  << "Color: " << game.player.color << std::endl
                  << "Speed: " << game.player.speed << std::endl;
    }
}