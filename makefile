run: output/sdlgaem
	$<

output/sdlgaem: $(wildcard source/*.cpp)
	g++ $^ -o $@ -lSDL2 -lSDL2_image -lSDL2_ttf -I/usr/include/SDL2

clean:
	rm output/sdlgaem
