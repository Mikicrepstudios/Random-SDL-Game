#include <iostream>
#include <string>
#include <sstream>

#include "settings.h"

namespace commands {
    int Place(std::string args, int argsCount, game::Settings settings, game::Map &map) {
        /**
         * @brief Function for Place command
         * @param args Command arguments
         * @param argsCount Amount of command arguments
         * @param settings Game settings
         * @param map Game map
         */
        // Empty command
        if(argsCount == 0 || argsCount == 1) {
            std::cout << "Not enough arguments" << std::endl;
            
            return 1;
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

        // place x y
        if(argsCount == 2) {
            map.map[x][y].type = 2;
            map.map[x][y].color = settings.blockColor;
            std::cout << "Successfuly placed block" << std::endl;

            return 0;
        }

        int blockColor = std::stoi(argsArray[2]);

        if(argsCount == 3) {
            map.map[x][y].type = 2;
            map.map[x][y].color = blockColor;
            std::cout << "Successfuly placed block" << std::endl;

            return 0;
        }

        if(argsCount > 3) {
            std::cout << "Too many arguments" << std::endl;
            
            return 1;
        }

        return -69;
    }
}