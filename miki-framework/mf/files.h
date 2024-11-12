#pragma once

#include <string>

/**
 * @brief This struct contains all basic functions for file managment
 */
namespace files {
    bool CheckFile(std::string path);
    int CreateFile(std::string path);
    int DeleteFile(std::string path);

    std::string ReadFile(std::string path);
    void AppendFile(std::string path, std::string content);
    void WriteFile(std::string path, std::string content);
}