# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Iinclude -I$(HEAD_DIR) # Add -I$(HEAD_DIR) to search in the src directory for headers

# Source directory
SRC_DIR = ../src

# Header dir
HEAD_DIR = ../src/include

# Source directory of client
SRC_DIR_CLI = ../src/client

# Source files from src directory
SOURCES = $(SRC_DIR)/monocypher.c $(SRC_DIR)/config.c $(SRC_DIR_CLI)/pin.c \
			$(SRC_DIR)/txt_reader.c
# Output binary names
BINS = pin_changer

# Targets
all: $(BINS)

# Build repiner binary
pin_changer: pin_changer.c $(SOURCES)
	$(CC) $(CFLAGS) -o $@$(EXE_EXT) pin_changer.c $(SOURCES) $(LIBS)

# Clean up build artifacts
clean:
	rm -f $(BINS)$(EXE_EXT) *.o

.PHONY: all clean
