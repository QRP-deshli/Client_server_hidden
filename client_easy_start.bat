@echo off
gcc -Wall -Wextra -Iinclude -Isrc -Isrc/client  -o client.exe client.c src/monocypher.c src/addition.c src/random.c src/config.c src/crypto.c src/network.c src/compress_decompress.c src/lzrw3-a.c src/txt_reader.c src/client/pin.c -lws2_32 -lm
.\client.exe