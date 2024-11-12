#include <cstdio>
#include <string>

namespace files {
    int DeleteFile(std::string path) {
        /**
         * @brief This function deletes specified file
         * @param path Path of file that you want to delete
         */
        
        if (std::remove(path.c_str()) == 0) return 0;
        else return 1;

        return -69; // Uh oh something happened
    }
}