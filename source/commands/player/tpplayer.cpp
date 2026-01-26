#include <iostream>
#include <string>
#include <sstream>

#include "settings.h"

namespace commands {
    int TpPlayer(std::string args, int argsCount, game::Game &game) {
        /**
         * @brief Function for TpPlayer command
         * @param args Command arguments
         * @param argsCount Amount of command arguments
         * @param player Game player
         */
        if(argsCount == 0) {
            return 2;
        }

        std::string* argsArray = new std::string[argsCount];
        std::stringstream ss(args);

        std::string curWord;
        int index = 0;
        while (ss >> curWord) {
            if (index < argsCount) {
                argsArray[index++] = curWord;
            }
        }

        int x = std::stoi(argsArray[0]);
        int y = std::stoi(argsArray[1]);

        // run
        if(argsCount == 2) {
            game.map.map[game.player.x][game.player.y].type = 0; // Delete old player from map
            game.player.x = x;
            game.player.y = y;

            return 0;
        }

        if(argsCount > 2) {
            return 3;
        }

        return -69;
    }
}