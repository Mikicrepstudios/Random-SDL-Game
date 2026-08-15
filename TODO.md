## TODO List
### This list conatins stuff that I might add in game - Feel free to [suggest stuff](https://github.com/Mikicrepstudios/Random-SDL-Game/blob/master/CONTRIBUTING.md)

### Some stuff for me

### In progress:
- Create inventory items database
- Seperate blocks and inventory items, inventory item should contain variable block which will be block id which will corespond to block that will be placed on use of inventory item, if its NULL then item isnt placeable

### Planned:
- Add a proper GameState enum: MENU, PLAYING, PAUSED, INVENTORY, DIALOGUE
- Replace raw int block/item IDs with enums or structs like enum BlockType { AIR, PLAYER, WOOD, STONE }, maybe same for item IDS (?)

### Thinking:


/* ARCHIVED
### In progress:
- - Remove saves/ in load game save selector

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
*/

### Explanations:
- In progress - Is currently being implemented
- Planned     - Will probably implement
- Thinking    - Has chance to be implemented
