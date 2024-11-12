run: output/app
	$<

output/app: $(wildcard miki-framework/*.cpp) $(wildcard miki-framework/*/*.cpp) $(wildcard miki-framework/*/*/*.cpp) $(wildcard source/*.cpp) $(wildcard source/*/*.cpp) $(wildcard source/*/*/*.cpp)
	g++ $^ -o $@ -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -I/usr/include/SDL2 -I./miki-framework/ -I./source/headers

windows: $(wildcard miki-framework/*.cpp) $(wildcard miki-framework/*/*.cpp) $(wildcard miki-framework/*/*/*.cpp) $(wildcard source/*.cpp) $(wildcard source/*/*.cpp) $(wildcard source/*/*/*.cpp)
	echo "You must have "sdl2mingw" folder."
	x86_64-w64-mingw32-g++ $^ -o output/app-x86_64.exe -I./miki-framework/ -I./source/headers -Isdl2mingw/include/SDL2 -Lsdl2mingw/lib -static-libstdc++ -static-libgcc -lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2

windows32: $(wildcard miki-framework/*.cpp) $(wildcard miki-framework/*/*.cpp) $(wildcard miki-framework/*/*/*.cpp) $(wildcard source/*.cpp) $(wildcard source/*/*.cpp) $(wildcard source/*/*/*.cpp)
	echo "You must have "sdl2mingw32" folder."
	i686-w64-mingw32-g++ $^ -o output/app-x86.exe -I./miki-framework/ -I./source/headers -Isdl2mingw32/include/SDL2 -Lsdl2mingw32/lib -static-libstdc++ -static-libgcc -lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2

clean:
	rm output/app
	rm output/app-x86_64.exe
	rm output/app-x86.exe

# -fsanitize=address - memory thingie
# -g - include debug symbols