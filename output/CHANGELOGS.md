## Added:
- Confirm dialogue on exit
- Confirm dialogue on clear
- Fullscreen on F11
- PRESETS!

## Removed:

## Changed/Fixed:
- Now you draw and erase while mouse is held instead of on clicking only
- colorpicker.cpp now uses functions from draw namespace
- Window can be now resized
- On hovering on white color cursor turns black
- Colors arent checked anymore each frame a lot of times, its now cached and they use structs now (basically less cpu usage)
- colors.cpp organized a lot better
- Fixed memory leak caused by texts
- Settings struct
- Player struct
- Camera struct
- Dialogues struct
- Game code is not way more readable and editable
- Removed colorR, colorG, colorB from most of code, you will not be forgotten
- Fixed makefile for windows32 compiling

## Notes:
- I wanted to make massive update before release, so here it is, a lot of stuff are under the hood but there are also a lot stuff that will be visible in actual game. 38 total changed files