#pragma once

#include <string>

/**
 * @brief This namespace is used to play sounds
 */
namespace sfx {
    void PlayBGMusic(const std::string path);
    void PlaySound(const std::string& path);
}