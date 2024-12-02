#include <iostream>
#include <string>
#include <sstream>

#include "settings.h"

namespace commands {
    int GetBlockType(std::string args, int argsCount, game::Map map) {
        /**
         * @brief Function for GetBlockType command
         * @param args Command arguments
         * @param argsCount Amount of command arguments
         * @param map Game map
         */
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

        // x y
        if(argsCount == 2) {
            int type = map.map[x][y].type;

            if(type == 0) std::cout << "Air" << std::endl;
            else if(type == 1) std::cout << "Player" << std::endl;
            else if(type == 2) std::cout << "Block" << std::endl;
            else std::cout << "Unknown block type" << std::endl;

            return 0;
        }

        if(argsCount > 2) {
            std::cout << "Too much arguments" << std::endl;
            return 1;
        }

        return -69;
    }
}