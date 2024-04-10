#include "block.h"

namespace gamemap {
    void ClearMap(Block worldMap[250][250], int mapWidth, int mapHeight) {
        for (int x = 0; x < mapWidth; x++) {
            for (int y = 0; y < mapHeight; y++) {
                worldMap[x][y] = Block(0, 0);
            }
        }
    }
}
