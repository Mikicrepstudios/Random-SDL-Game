## Added:
-- Build 56
- Added namespace mouse
- Game icon

-- Build 57
- Added namespace colorpicker
- Added bool cheats and int cheatsId to namespace settings
- Added SDL_Event in game::SDL_Settings
- Added more stuff to gameInfo

## Removed:
-- Build 56
- Shortened function arguments

-- Build 57
- Removed namespace overlay and overlay.h
- Removed struct game::cheats
- Shortened function arguments

## Changed/Fixed:
-- Build 56
- Renamed MouseInvChooser() to Chooser(), InventoryEvent() to Event(), InventoryOverlay() to Overlay()
- Moved all inventory functions to inventory namespace
- Moved overlay::Mouse() to mouse::Overlay() and player::MouseEvent() to mouse::Event()

-- Build 57
- Moved overlay:ColorPicker() to colorpicker:Overlay() and player::ColorPickerEvent() to colorpicker:Event()
- Renamed hud.cpp to gameinfo.cpp
- Rewrote gameInfo code

## Notes:


###### Changelogs are cleared every release