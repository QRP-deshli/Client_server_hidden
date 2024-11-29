@echo off
gcc -Wall -Wextra -Iinclude -Isrc -o server.exe server.c src/monocypher.c src/addition.c src/random.c src/config.c src/crypto.c src/network.c src/compress_decompress.c src/lzrw3-a.c -lws2_32 -lm
.\server.exe