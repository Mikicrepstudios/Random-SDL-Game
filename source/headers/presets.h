#ifndef PRESETS_H_INCLUDED
#define PRESETS_H_INCLUDED

struct Preset {
	int blockColor;

	Preset() : blockColor(0) {}

	Preset(int _blockColor) : blockColor(_blockColor) {}
};

#endif
