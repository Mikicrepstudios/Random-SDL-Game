#include <iostream>
#include <string>
#include <sstream>

#include "settings.h"

namespace commands {
    int GetBlockColor(std::string args, int argsCount, game::Map map) {
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
            std::cout << map.map[x][y].color << std::endl;

            return 0;
        }

        if(argsCount > 2) {
            std::cout << "Too much arguments" << std::endl;
            return 1;
        }

        return -69;
    }
}