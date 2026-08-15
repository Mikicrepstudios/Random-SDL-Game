#pragma once

#include "settings.h"

namespace map {
void ClearMap(game::Game &game);
void GenerateMap(game::Game &game);

void PlaceBlock(game::Game &game, game::Game::Database::Blocks &block, int x,
                int y);
void DestroyBlock(game::Game &game, int x, int y);
} // namespace map