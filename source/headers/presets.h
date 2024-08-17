#ifndef PRESETS_H_INCLUDED
#define PRESETS_H_INCLUDED

/*struct Preset {
	int blockColor;

	Preset() : blockColor(0) {}

	Preset(int _blockColor) : blockColor(_blockColor) {}
};*/ // i would assume that this is safe to remove, i will keep it just to be sure even tho i know its 99.99% useless

namespace settings {
	struct Preset {
		int blockColor;
	};
}
// Comments for future me : presets will be array; begone block color variable! we dont like you. 
// maybe im too harsh on it but it should really be gone, we will miss it (it will have no impact on actual game lol)
// why anyone would even check preset.h header file, do not read this. this will be gone in few builds anyways, you can leave now

// Anyways i did this i will just keep comments for fun and remove in next build, if anyone even checks this tell me

#endif