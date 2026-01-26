#include "settings.h"

/**
 * @brief Namespace that contains all functions that could be used for debugging
 */
namespace debug {
    void PrintGame(game::Game &game);
    void PrintSettings(game::Game &game);
    void PrintMap(game::Game &game);
    void PrintPlayer(game::Game &game);
    void PrintCamera(game::Game &game);
}