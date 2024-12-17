# Random SDL Game
#### Sandbox game where you place blocks, based on [Mikicrep Framework](https://miki.macakom.net/projects/mf)
- by Mikicrep

You can also make pixelart in it if you want.

[Project page](https://miki.macakom.net/projects/rsg)

## Compatibility
- Supports **linux** and **windows** (shh linux is given bigger love)
- Made for **ultra** low-end devices

## Requirements
For linux compiling:

```pacman -S --needed base-devel sdl2 sdl2_ttf sdl2_image```

For windows compiling:

```pacman -S --needed mingw-w64-gcc```

Also you must get `mingw-w64-sdl2` from AUR for windows compiling

Theese commands only work on arch based linux distros, for other ones you gotta find corresponding packages

## Compiling and running
1. Clone this repo
2. Open terminal and run `make`
- You can move `output` folder out of repo folder and delete repo folder
- You can delete .md and .dll files too but do not delete anything else
- Game will open automatically after successful compilation
### Windows x86_64 (64bit)
1. Clone this repo
2. Download `sdl2mingw.zip` [here](https://drive.google.com/drive/folders/1uVe7oKMzCTTUeMJuHL7vqK0O2FK_pIPs?usp=drive_link)
3. Extract `sdl2mingw.zip` in root folder of repo (make sure you put folder not contents of folder)
4. Open terminal and run `make windows`
5. Move contents from `x86_64dll` folder to output folder
- You can move `output` folder out of repo folder and delete repo folder
- You can delete .md files too but do not delete anything else
- Game will not open automatically after successful compilation
- Works under [WINE](https://www.winehq.org/) too
### Windows x86 (32bit)
1. Clone this repo
2. Download `sdl2mingw32.zip` [here](https://drive.google.com/drive/folders/1uVe7oKMzCTTUeMJuHL7vqK0O2FK_pIPs?usp=drive_link)
3. Extract `sdl2mingw32.zip` in root folder of repo (make sure you put folder not contents of folder)
4. Open terminal and run `make windows32`
5. Move contents from `x86dll` folder to output folder
- You can move `output` folder out of repo folder and delete repo folder
- You can delete .md files too but do not delete anything else
- Game will not open automatically after successful compilation
- Works under [WINE](https://www.winehq.org/) too

*Compiling is recommended unless you get game from [releases](https://github.com/Mikicrepstudios/Random-SDL-Game/releases) page*

## Documentation
You can find documentation [here](https://miki.macakom.net/projects/docs/rsg/index)

## Bug reporting && Suggestions && Contributing
Please read [CONTRIBUTING.md](https://github.com/Mikicrepstudios/Random-SDL-Game/blob/master/CONTRIBUTING.md)

## Support us
You can support us by giving us star on github <3

### Runs on potato.


