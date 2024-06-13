#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED

// Type
/*
0 - air
1 - player
2 - block
*/

struct Block {
	int type;
	int color;

	Block() : type(0), color(0) {}

	Block(int _type, int _color)
	: type(_type), color(_color) {}
};

#endif
