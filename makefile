run: output/sdlgaem
	$<

output/sdlgaem: $(wildcard source/*.cpp) $(wildcard source/*/*.cpp) $(wildcard source/*/*/*.cpp)
	g++ $^ -o $@ -lSDL2 -lSDL2_image -lSDL2_ttf -I/usr/include/SDL2 -I./source/headers

windows: $(wildcard source/*.cpp) $(wildcard source/*/*.cpp) $(wildcard source/*/*/*.cpp)
	echo "You must have "sdl2mingw" folder. More info in README.md"
	x86_64-w64-mingw32-g++ $^ -o output/sdlgaemx86_64.exe -I./source/headers -Isdl2mingw/include/SDL2 -Lsdl2mingw/lib -static-libstdc++ -static-libgcc -lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2

windows32: $(wildcard source/*.cpp) $(wildcard source/*/*.cpp) $(wildcard source/*/*/*.cpp)
	echo "You must have "sdl2mingw32" folder. More info in README.md"
	i686-w64-mingw32-g++ $^ -o output/sdlgaemx86.exe -I./source/headers -Isdl2mingw32/include/SDL2 -Lsdl2mingw32/lib -static-libstdc++ -static-libgcc -lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2

clean:
	rm output/sdlgaem
	rm output/sdlgaemx86_64.exe
	rm output/sdlgaemx86.exe
