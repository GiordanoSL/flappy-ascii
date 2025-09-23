# Define the project name, which will be the name of the executable.
TARGET = flappy_bird

# Define the directories for source and header files.
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

# Find all C source files (.c) in the source directory.
# The `wildcard` function is used to find all files that match the pattern.
SRC = $(wildcard $(SRC_DIR)/*.c)

# Create a list of object files (.o) from the source files.
# The `patsubst` function substitutes the file extension.
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))

# Define the compiler and flags.
CC = gcc
CFLAGS = -Wall -g -lpthread -I$(INCLUDE_DIR) -O3

# The default target. This is the first target executed when you run `make`.
# It depends on the $(TARGET) executable.
all: $(TARGET)

# Rule to link the object files into the final executable.
# The $(OBJ) are prerequisites, so this rule is only executed after they are built.
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@

# Rule to compile each source file into an object file.
# The $< is an automatic variable that represents the first prerequisite (the .c file).
# The $@ is an automatic variable that represents the target (the .o file).
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean up the build directory.
# `rm -f` removes the specified files without prompting for confirmation.
clean:
	rm -f $(TARGET) $(OBJ)

run:
	./$(TARGET)