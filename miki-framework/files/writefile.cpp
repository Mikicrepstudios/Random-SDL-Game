#include <fstream>
#include <string>

namespace files {
    void WriteFile(std::string path, std::string content) {
        /**
         * @brief This function writes text to file with overwriting existing one
         * @param path Path to the file
         * @param content Stuff that you want to write to the file
         */
        
        std::ofstream file(path);
        file << content;
        file.close();
    }
}