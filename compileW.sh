#!/bin/bash
 
cd final/cfiles
gcc replayer.c -o ../replayer
gcc bn.c -o ../menu -lncursesw
gcc Puissance4réseauC.c -o ../p4c -lSDL2_net -lSDL2 
gcc Puissance4réseauS.c -o ../p4s -lSDL2_net -lSDL2 -lws2_32
gcc menu.c -ISDL2/include -LSDL2/lib -w -o ../main -lmingw32 -lSDL2main -lSDL2  -lSDL2_ttf -lSDL2_mixer -lSDL2_net -lSDL2_image -lws2_32 -fstack-protector-all
cd ../../
