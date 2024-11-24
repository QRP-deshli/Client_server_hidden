# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Iinclude -I$(SRC_DIR)  # Add -I$(SRC_DIR) to search in the src directory for headers

# Source directory
SRC_DIR = ../src

# Source files from src directory
SOURCES = $(SRC_DIR)/monocypher.c 

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
