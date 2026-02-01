#include <filesystem>
#include <string>
#include <iostream>
#include <vector>

namespace files {
    std::vector<std::string> lsDir(std::string path) {
        /**
         * @brief This function lists directory contents and returns vector containing all files
         * @param path Path of folder that you want to ls
         */

        std::vector<std::string> result;
        for (const auto & entry : std::filesystem::directory_iterator(path)) {
            result.push_back(entry.path().string());
        }
        return result;
    }
}