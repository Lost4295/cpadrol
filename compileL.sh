#!/bin/bash
 
cd final/cfiles
gcc replayer.c -o ../replayer -w
gcc bn.c -o ../menu -lncurses -w
gcc Puissance4réseauC.c -o ../p4c -lSDL2_net -lSDL2 -w
gcc Puissance4réseauS.c -o ../p4s -lSDL2_net -lSDL2 -w
gcc menu.c $(sdl2-config --cflags --libs)  -lSDL2_ttf -w -lSDL2_mixer -lSDL2_net -lSDL2_image -o ../main
cd ../../

