# Compiler Option, Set to "gcc" for C compilation, "g++" for C++ compilation
CC = g++

# Target to build to (name.exe)
TARGET = example

# Important directories for header and source files for compilation
INC_DIR = ./include
SRC_DIR = ./src

# Source files
SRC = $(SRC_DIR)/main.cpp $(SRC_DIR)/Socket.cpp

# Compiler Flags
# 	-g 			Adds debug information to final build
#	-Wall 	Turns on most warnings for compilation
CFLAGS = -Wall -g -I$(INC_DIR)

# Compiler Flags for release, excludes debug information
CFLAGS_R = -Wall -I$(INC_DIR)

# Set default make to "debug"
default = debug

# Compile for debug, allow -g flag
debug: $(OBJ)
	$(CC) -o ./build/$(TARGET) $(SRC) $(CFLAGS)

# Compiles for final release, does not allow -g flag
release: $(OBJ)
	$(CC) -o ./build/$(TARGET) $(SRC) $(CFLAGS_R)

# Cleans object files
clean:
	rm -f $@
