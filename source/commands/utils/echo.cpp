#include <iostream>
#include <string>

namespace commands {
    int Echo(std::string command) {
        // Find the position of the first space
        size_t pos = command.find(' ');

        // Check if a space is found
        if (pos != std::string::npos) {
            // Print everything after the first space
            std::cout << command.substr(pos + 1) << std::endl;
        } else {
            // If no space is found, there's only one word, so there's nothing to print
            return 1;
        }
        return 0;
    }
}