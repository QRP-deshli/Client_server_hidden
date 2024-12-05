@echo off
gcc -Wall -Wextra -Iinclude -I../src -I../src/client -o pin_changer pin_changer.c ../src/monocypher.c ../src/config.c ../src/client/pin.c ../src/txt_reader.c
.\pin_changer.exe
