#include <fstream>
#include <iostream>
#include <string>

namespace files {
    std::string ReadFile(std::string path) {
        /**
         * @brief Reads contents of file and returns them as std::string
         * @param path Path of file you want to read
         */
        
        std::ifstream file(path);

        if (file) {
            std::string content((std::istreambuf_iterator<char>(file)),
                                std::istreambuf_iterator<char>());
            file.close();
            return content;
        } else {
            std::cerr << "Unable to open file: " << path << std::endl;
        }

        return NULL;
    }
}