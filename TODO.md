## TODO List
### This list conatins stuff that I might add in game - Feel free to [suggest stuff](https://github.com/Mikicrepstudios/Random-SDL-Game/blob/master/CONTRIBUTING.md)

### Some stuff for me
... also test windows support after that because it is cancer, last time i had save function inside of non main file it did stack overflow error

how i can make it not instantly click other subdialogues when clicking dialogue? Currently it just checks window.mouse.isdown which is always true and if i try to disable it will be reenabled next frame


### In progress:
- - Multiple save slots
- - Save as some other slot than default
- - Fix instant click when selecting "Exit with saving"

### Planned:
- Code
- - Add in inventory initrects texts for cropped stuff like BG or Exit
- - Seperate placing and destroying blocks into its own function and reference it in playercontrols.cpp and cheats like place and destroy
- - After updating to newer MF version, add to changelogs that button hover effect is now weaker
- Core
- - Colors for block textures
- - Autosave - save after n of placed blocks OR n time (not decided)
- - Way to save and load while inside of game
- - Different world map sizes?
- - Blocks with gravity
- Commands
- Files
- Graphics
- - Change hover to be either transparent or just darker color of block
- - Hotbar drawn at bottom for block presets
- - Replace "solid" flip flop switch to allow other kind of blocks too instead of it only, rename to "Block type"

### Thinking:
- - Shadows
- - Undo/Redo
- - Custom color pallete
- - Seperate build/play mode?

### Explanations:
- In progress - Is currently being implemented
- Planned     - Will probably implement
- Thinking    - Has chance to be implemented
