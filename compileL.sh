#!/bin/bash
 
cd final/cfiles
gcc replayer.c -o ../replayer
gcc bn.c -o ../menu -lncurses
gcc Puissance4réseauC.c -o ../p4c -lSDL2_net -lSDL2
gcc Puissance4réseauS.c -o ../p4s -lSDL2_net -lSDL2
gcc menu.c $(sdl2-config --cflags --libs)  -lSDL2_ttf -lSDL2_mixer -lSDL2_net -lSDL2_image -o ../main
cd ../

