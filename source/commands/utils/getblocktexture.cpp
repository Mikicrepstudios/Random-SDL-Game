#include <iostream>
#include <string>
#include <sstream>

#include "settings.h"

namespace commands {
    int GetBlockTexture(std::string args, int argsCount, game::Map map) {
        /**
         * @brief Function for GetBlockTexture command
         * @param args Command arguments
         * @param argsCount Amount of command arguments
         * @param map Game map
         */
        if(argsCount == 0 || argsCount == 1) {
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

        // x y
        if(argsCount == 2) {
            std::cout << map.map[x][y].texture << std::endl;

            return 0;
        }

        if(argsCount > 2) {
            return 3;
        }

        return -69;
    }
}