main:
	g++ source/*.cpp -o output/sdlgaem -lSDL2 -lSDL2_image -lSDL2_ttf -I/usr/include/SDL2
	./output/sdlgaem

clean:
	rm output/sdlgaem
