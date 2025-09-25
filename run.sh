#!/bin/sh

# Download the program
curl -L -o program.cpp https://bit.ly/mcpdsktp

# Compile manually (adjust paths if needed)
clang++ program.cpp -o program -I/usr/local/include -L/usr/local/lib -lSDL2 -lSDL2_ttf

# Run without Xorg
SDL_VIDEODRIVER=KMSDRM ./program
