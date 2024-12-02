#include <iostream>
#include <string>

namespace commands {
    int Echo(std::string args) {
        /**
         * @brief Function for Echo command
         * @param args Text to print
         */
        std::cout << args << std::endl;

        return 0;
    }
}