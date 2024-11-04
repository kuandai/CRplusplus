# Compiler
CXX = g++

# Compiler flags
CFLAGS = -Wall -Wextra -std=c++20 -O2 -s

# Linker flags
LDFLAGS = -lSDL2 -lGLEW -lGL -ldl -lGLU -lglfw -lX11 -lpthread

# Binary name
BIN_NAME = game

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
INCLUDE_DIR = include
SHADER_DIR = shader
SHADER_INCLUDE_DIR = $(INCLUDE_DIR)/shader

# Include flags
INCLUDEFLAGS = $(shell find $(INCLUDE_DIR) -type d -printf '-I%p ')

# Find all source files in the SRC_DIR, replace extension .c or .cpp with .o and prefix with BUILD_DIR/
SRC = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC)))

# Shader files
SHADERS = $(wildcard $(SHADER_DIR)/*.glsl)
SHADER_HEADERS = $(SHADERS:$(SHADER_DIR)/%.glsl=$(SHADER_INCLUDE_DIR)/%.h)

# Output binary
OUT = $(BIN_DIR)/$(BIN_NAME)

# Default target
all: pack

# Compilation target
compile: shaders $(OUT)

# Package binary into archive
pack: | compile
	cp -r textures $(BIN_DIR)
	rm -f $(BIN_DIR)/$(BIN_NAME).zip
	zip $(BIN_DIR)/$(BIN_NAME).zip $$(find $(BIN_DIR) -type f)

# Link the target binary from the object files
$(OUT): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CFLAGS) $(INCLUDEFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CFLAGS) $(INCLUDEFLAGS) -c $< -o $@

# Build and run the program
test: all
	$(BIN_DIR)/$(BIN_NAME)

# Clean up build and bin directories
clean: cleanshaders shaders
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	mkdir $(BUILD_DIR) $(BIN_DIR)

# Shader targets
shaders: $(SHADER_HEADERS)

$(SHADER_INCLUDE_DIR)/%.h: $(SHADER_DIR)/%.glsl
	@mkdir -p $(SHADER_INCLUDE_DIR)
	xxd -i $< $@ && sed -i 's/unsigned/const/g' $@

# Clean shader headers
cleanshaders:
	rm -rf $(SHADER_INCLUDE_DIR)
	mkdir $(SHADER_INCLUDE_DIR)

# Phony targets
.PHONY: all clean test shaders pack