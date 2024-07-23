run: output/sdlgaem
	$<

output/sdlgaem: $(wildcard source/*.cpp) $(wildcard source/*/*.cpp)
	g++ $^ -o $@ -lSDL2 -lSDL2_image -lSDL2_ttf -I/usr/include/SDL2 -I./source/headers

windows: $(wildcard source/*.cpp) $(wildcard source/*/*.cpp)
	x86_64-w64-mingw32-g++ $^ -o outputwindows/sdlgaem.exe -I./source/headers -Isdl2mingw/include/SDL2 -Lsdl2mingw/lib -static-libstdc++ -static-libgcc -lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2

clean:
	rm output/sdlgaem
	rm outputwindows/sdlgaem.exe
