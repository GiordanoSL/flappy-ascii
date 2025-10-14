# Project name (without path)
TARGET = flappy_bird

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

# Source and object files
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))
EXEC = $(BUILD_DIR)/$(TARGET)

# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -lpthread -I$(INCLUDE_DIR) -O3

# Default target
all: $(EXEC)

# Link object files into the final executable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Run the program
run: $(EXEC)
	./$(EXEC)
