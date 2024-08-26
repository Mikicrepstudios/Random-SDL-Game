## Added:
-- Build 51
- New struct Map
- Current block color is now part of settings struct

-- Build 52
- Added demo save text on demo save
- New struct SDL_Settings

-- Build 54
- New struct Cheats
- More stuff in SDL_Settings struct
- Now clicking outside of color picker closes picker
- New struct dialogueRects
- Demo save text now got black border

-- Build 55
- Renderer is now part of game::sdlSettings struct
- Added game.colorPicker and game.colorPickerId to fix old messy implementation of color picker
- Pressing button which causes dialogue for 2nd time closes dialogue (flip flop switch)

## Removed:
-- Build 51
- Removed release text

-- Build 52
- Removed preset selector arrows

-- Build 53
- Shortened function arguments

-- Build 54
- Shortened function arguments

-- Build 55
- Removed colorPick, bgColorPick, playerColorPick from game::settings
- Inventory is no longer dropping fps to 15 (Removed optimization)

## Changed/Fixed:
-- Build 52
- Renamed confirmRect to yesRect
- background.png is no longer loaded on each frame, less cpu (and hdd) usage, and possible prevention of memory leak

-- Build 53
- initRects for inventoryRects now looks better
- fixed typo in BUGS.md

-- Build 54
- Code readers SHOULD no longer have headache when looking at mouseinvchooser function. 3 screens are no longer required to show whole function.
- Fixed quick color pick
- Color picker function is no longer void, now returns int value
- Renamed settings::Preset to game::Preset
- Added settings.playerColor to modify easier player color value
- Makefile scans for files even deeper
- Demo save stars now got stars on fullhd resouluton

-- Build 55
- Rewrote colorPicker
- All functions now start with big letter
- ConfirmDialogue() is now bool
- Dialogue results are now handled way better

## Notes:


###### Changelogs are cleared every release