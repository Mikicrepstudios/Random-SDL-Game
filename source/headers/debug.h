#include "settings.h"

/**
 * @brief Namespace that contains all functions that could be used for debugging
 */
namespace debug {
    void PrintGame(game::Game game);
    void PrintSettings(game::Settings settings);
    void PrintMap(game::Map map);
    void PrintPlayer(game::Player player);
    void PrintCamera(game::Camera camera);
}