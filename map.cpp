namespace gamemap {
    void ClearMap(int worldMap[250][250], int mapWidth, int mapHeight) {
        for (int x = 0; x < mapWidth; x++) {
            for (int y = 0; y < mapHeight; y++) {
                worldMap[x][y] = 0;
            }
        }
    }
}
