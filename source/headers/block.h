#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED

// Type
/*
0 - air
1 - player
2 - block
*/

struct Block {
	// Look
	int type = 0; // Check comments above
	int color = 0; // Base color of block
	int texture = 0; // Texture id of block

	// Properties
	bool isSolid = true; // Can player pass thru it?
};

#endif
