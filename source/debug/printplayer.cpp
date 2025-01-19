#include <iostream>

#include "settings.h"

namespace debug {
    void PrintPlayer(game::Player player) {
        std::cout << player.x << std::endl
                  << player.y << std::endl
                  << player.color << std::endl
                  << player.speed << std::endl;
    }
}