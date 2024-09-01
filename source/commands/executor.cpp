#include <iostream>
#include <string>

#include "commands.h"

namespace commands {
    void Executor(std::string command) {
        int status = 0;
        size_t pos = command.find(' ');

        std::string commandId = command.substr(0, pos);

        if(commandId == "echo") status = Echo(command);

        else std::cout << "Error executing command : Not found";
    }
}