# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Iinclude -I$(SRC_DIR)  # Add -I$(SRC_DIR) to search in the src directory for headers

# Source directory
SRC_DIR = src

# Source files from src directory
SOURCES = $(SRC_DIR)/monocypher.c $(SRC_DIR)/addition.c \
		  $(SRC_DIR)/random.c $(SRC_DIR)/config.c $(SRC_DIR)/crypto.c \
		  $(SRC_DIR)/network.c 

# Output binary names
BINS = server client

# Libraries
ifeq ($(OS), Windows_NT)
    LIBS = -lws2_32 -lm
    EXE_EXT = .exe
else
    LIBS = -lm -lbsd
    EXE_EXT =
endif

# Targets
all: $(BINS)

# Build server binary
server: server.c $(SOURCES)
	$(CC) $(CFLAGS) -o $@$(EXE_EXT) server.c $(SOURCES) $(LIBS)

# Build client binary
client: client.c $(SOURCES) $(SRC_DIR)/pin.c
	$(CC) $(CFLAGS) -o $@$(EXE_EXT) client.c $(SOURCES) $(SRC_DIR)/pin.c $(LIBS)

# Clean up build artifacts
clean:
	rm -f $(BINS)$(EXE_EXT) *.o

.PHONY: all clean
