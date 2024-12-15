@echo off
gcc -Wall -Wextra -Iinclude -Isrc/include  -o client.exe client.c src/monocypher.c src/addition.c src/random.c src/config.c src/crypto.c src/network.c src/compress_decompress.c src/lzrw3-a.c src/txt_reader.c src/client/pin.c -lws2_32 -lm
.\client.exe
rem .\client.exe 9999 starting client with custom port
rem .\client.exe 9999 192.168.1.1 starting client with custom port and ip