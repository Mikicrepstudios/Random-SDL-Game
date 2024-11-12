#include <fstream>
#include <string>

namespace files {
    void AppendFile(std::string path, std::string content) {
        /**
         * @brief This function writes text to file without overwriting existing one
         * @param path Path to the file
         * @param content Stuff that you want to write to the file
         */
        
        std::ofstream file(path, std::ios::app);
        file << content;
        file.close();
    }
}