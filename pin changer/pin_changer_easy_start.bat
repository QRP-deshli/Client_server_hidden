@echo off
gcc -Wall -Wextra -Iinclude -I../src   -o pin_changer pin_changer.c ../src/monocypher.c ../src/config.c ../src/pin.c
.\pin_changer.exe
