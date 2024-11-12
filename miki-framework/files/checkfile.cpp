#include <fstream>
#include <string>

namespace files {
    bool CheckFile(std::string path) {
        /**
         * @brief Checks does file exist, returns 1 if it does and 0 if it doesnt
         * @param path Path to the file you want to check
         */
        
        std::ifstream file(path);
        if(file) return 1;
        else return 0;
    }
}