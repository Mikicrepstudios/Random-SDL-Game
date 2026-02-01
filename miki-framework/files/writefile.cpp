#include <fstream>
#include <iostream>
#include <string>

namespace files {
    void WriteFile(std::string path, std::string content) {
        /**
         * @brief This function writes text to file with overwriting existing one
         * @param path Path to the file
         * @param content Stuff that you want to write to the file
         */
        
        std::ofstream file(path);
        if(!file.is_open()) {std::cout << "[MF] Error loading file: \"" << path << "\", did you enter correct path?" << std::endl; return;}
        file << content;
        file.close();
    }
}