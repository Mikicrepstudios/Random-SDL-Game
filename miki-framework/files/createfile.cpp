#include <fstream>
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
        else return 1;

        return -69; // Uh oh something happened
    }
}