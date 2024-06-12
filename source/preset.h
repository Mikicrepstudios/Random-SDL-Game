#ifndef PRESET_H_INCLUDED
#define PRESET_H_INCLUDED

struct Preset {
	int index;

	int blockColor;

	Preset() : index(0), blockColor(0) {}

	Preset(int _index, int _blockColor) : index(_index), blockColor(_blockColor) {}
};

#endif
