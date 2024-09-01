#include <iostream>
#include <string>

namespace commands {
    int Echo(std::string args) {
        std::cout << args << std::endl;

        return 0;
    }
}