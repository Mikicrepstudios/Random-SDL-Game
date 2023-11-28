namespace gamemap {
    void clearmap(int worldmap[250][250], int mapwidth, int mapheight) {
        for (int x = 0; x < mapwidth; x++) {
            for (int y = 0; y < mapheight; y++) {
                worldmap[x][y] = 0;
            }
        }
    }
}
