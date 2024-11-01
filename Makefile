# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra

# Source files
SOURCES = server.c client.c monocypher.c shared.c random.c config.c

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

# Build each binary
$(BINS): %: %.c shared.c
	$(CC) $(CFLAGS) -o $@$(EXE_EXT) $< shared.c random.c monocypher.c config.c $(LIBS)

# Clean up build artifacts
clean:
	rm -f $(BINS)$(EXE_EXT) *.o

.PHONY: all clean
