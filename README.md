# cpadrol


Voici le projet de C de l'année 2023-2024 pour l'examen de la matière "Langage C avancé".


## Puissance 4 online

C'est un puissance 4 avec une fonctionnalité en ligne. Rien de plus. 


Sur celui-ci, vous pourrez :
    - Jouer à deux en ligne (logique)
    - Jouer à deux sur le même PC (en local)
    - Revoir une partie précédemment jouée contre quelqu'un
    - Le tout, avec une musique de fond (eh oui !)

Afin de pouvoir le build, il va falloir installer les librairies nécessaires. 

 - Lancez la commande 

    $ apt-get install libsdl2-dev libsdl2-net-dev libsdl2-ttf-dev libsdl2-mixer-dev libsdl2-image-dev

 afin de pouvoir installer les dépendances nécessaires au lancement du programme.

 Ensuite, il faut cloner le projet. 

    $ git clone https://github.com/Lost4295/cpadrol.git
    $ cd cpadrol/src

 Enfin, on peut lancer le fichier ! 

    $ gcc menu.c $(sdl2-config --cflags --libs) -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_net -w && ./a.out 


----------------------------


Sinon, vous pouvez télécharger les exécutables directement depuis la page https://github.com/Lost4295/cpadrol/releases.
