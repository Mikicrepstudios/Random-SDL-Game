#include <iostream>
#include <string>
#include <sstream>

#include "settings.h"

namespace commands {
    int Place(std::string args, int argsCount, game::Settings settings, game::Map &map) {
        // Empty command
        if(argsCount == 0 || argsCount == 1) return 1;

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

            return 0;
        }

        return -69;
    }
}