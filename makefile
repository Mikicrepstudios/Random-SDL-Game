# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -I/usr/include/SDL2 -I./miki-framework/ -I./source/headers
LDFLAGS := -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# Paths
SRC_DIR := source
FRAMEWORK_DIR := miki-framework
OBJ_DIR := output/objects
OUT_DIR := output
APP := $(OUT_DIR)/app

# Recursive wildcard function
rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

# Find all .cpp source files recursively
SRCS := $(call rwildcard,$(SRC_DIR)/,*.cpp) $(call rwildcard,$(FRAMEWORK_DIR)/,*.cpp)
OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Ensure output directories exist
$(shell mkdir -p $(OUT_DIR) $(OBJ_DIR))

# Rules
.PHONY: all run clean windows windows32

all: $(APP)

# Open app
open:
	./output/app

run: $(APP)
	$<

# Link all object files
$(APP): $(OBJS)
	$(CXX) $^ -o $@ $(CXXFLAGS) $(LDFLAGS)

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
	rm -rf $(OBJ_DIR)/*

# -fsanitize=address - memory thingie
# -g - include debug symbols