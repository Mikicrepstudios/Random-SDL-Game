#include <iostream>

#include "settings.h"

namespace debug {
    void PrintPlayer(game::Player player) {
        std::cout << "DEBUG - PLAYER" << std::endl;
        std::cout << "X: " << player.x << std::endl
                  << "Y: " << player.y << std::endl
                  << "Color: " << player.color << std::endl
                  << "Speed: " << player.speed << std::endl;
    }
}