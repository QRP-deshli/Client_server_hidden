@echo off
gcc -Wall -Wextra -Iinclude -I../src/include  -o pin_changer pin_changer.c ../src/monocypher.c ../src/config.c ../src/client/pin.c ../src/txt_reader.c
.\pin_changer.exe
