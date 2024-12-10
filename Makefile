# Compiler
CC = gcc

# Source directory
SRC_DIR = src
CLIENT_DIR = src/client

# Compiler flags
CFLAGS_SERVER = -Wall -g -Wextra -Iinclude -I$(SRC_DIR)# Add -I$(SRC_DIR) to search in the src directory for headers

# Compiler flags
CFLAGS_CLIENT = -Wall -g -Wextra -Iinclude -I$(SRC_DIR) -I$(CLIENT_DIR) # Add -I$(CLIENT_DIR) to search in the src/client directory for headers

# Source files from src directory
SOURCES_SERVER = $(SRC_DIR)/monocypher.c $(SRC_DIR)/addition.c \
		  $(SRC_DIR)/random.c $(SRC_DIR)/config.c $(SRC_DIR)/crypto.c \
		  $(SRC_DIR)/network.c $(SRC_DIR)/compress_decompress.c \
		  $(SRC_DIR)/lzrw3-a.c 

# Source files from src directory and client
SOURCES_CLIENT = $(SRC_DIR)/monocypher.c $(SRC_DIR)/addition.c \
		  $(SRC_DIR)/random.c $(SRC_DIR)/config.c $(SRC_DIR)/crypto.c \
		  $(SRC_DIR)/network.c $(SRC_DIR)/compress_decompress.c \
		  $(SRC_DIR)/lzrw3-a.c $(SRC_DIR)/txt_reader.c \
		  $(CLIENT_DIR)/pin.c

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
server: server.c $(SOURCES_SERVER)
	$(CC) $(CFLAGS_SERVER) -o $@$(EXE_EXT) server.c $(SOURCES_SERVER) $(LIBS)

# Build client binary
client: client.c $(SOURCES_CLIENT) 
	$(CC) $(CFLAGS_CLIENT) -o $@$(EXE_EXT) client.c $(SOURCES_CLIENT) $(LIBS)

# Clean up build artifacts
clean:
	rm -f $(BINS)$(EXE_EXT) *.o

.PHONY: all clean
