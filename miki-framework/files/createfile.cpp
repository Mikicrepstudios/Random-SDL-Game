#include <fstream>
#include <iostream>
#include <string>

namespace files {
    int CreateFile(std::string path) {
        /**
         * @brief This function creates empty file
         * @param path Path of new file
         */
        
        std::ofstream file(path);

        if (file) {
            file.close();
            return 0;
        }
        else {
            std::cout << "[MF] Error creating file: \"" << path << "\", do you have sufficient permissions?" << std::endl;
            return 1;
        }

        return -69; // Uh oh something happened
    }
}