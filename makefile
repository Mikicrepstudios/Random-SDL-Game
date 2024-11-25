# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -I/usr/include/SDL2 -I./miki-framework/ -I./source/headers -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# Paths
SRC_DIR := source
FRAMEWORK_DIR := miki-framework
OBJ_DIR := output/objects
OUT_DIR := output
APP := $(OUT_DIR)/app

# Find source files (recursive with wildcards)
SRCS := $(wildcard $(SRC_DIR)/**/**/*.cpp) $(wildcard $(FRAMEWORK_DIR)/**/**/*.cpp)
OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Create output folders automatically
$(shell mkdir -p $(OUT_DIR) $(OBJ_DIR) $(OBJ_DIR)/$(SRC_DIR) $(OBJ_DIR)/$(FRAMEWORK_DIR))

# Rules
.PHONY: all run clean windows windows32

all: $(APP)

run: $(APP)
	$<

# Link all object files
$(APP): $(OBJS)
	$(CXX) $^ -o $@ $(CXXFLAGS)

# Rule to compile each object
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build for Windows 64-bit
windows: $(SRCS)
	@echo "You must have 'sdl2mingw' folder."
	x86_64-w64-mingw32-g++ $^ -o $(OUT_DIR)/app-x86_64.exe -I./miki-framework/ -I./source/headers -Isdl2mingw/include/SDL2 -Lsdl2mingw/lib -static-libstdc++ -static-libgcc -lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2

# Build for Windows 32-bit
windows32: $(SRCS)
	@echo "You must have 'sdl2mingw32' folder."
	i686-w64-mingw32-g++ $^ -o $(OUT_DIR)/app-x86.exe -I./miki-framework/ -I./source/headers -Isdl2mingw32/include/SDL2 -Lsdl2mingw32/lib -static-libstdc++ -static-libgcc -lmingw32 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2

# Clean output directories
clean:
	rm -rf $(OUT_DIR)/*
	rm -rf $(OBJ_DIR)/*

# -fsanitize=address - memory thingie
# -g - include debug symbols