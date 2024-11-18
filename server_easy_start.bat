@echo off
gcc -Wall -Wextra -Iinclude -Isrc   -o server.exe server.c src/monocypher.c src/addition.c src/random.c src/config.c src/crypto.c src/network.c -lws2_32 -lm
.\server.exe