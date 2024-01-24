#!/bin/bash
 
cd final
ok=1
#check if cfiles exists
FILE=cfiles
if [ -d "$FILE" ]; then
    echo "directory $FILE exists."
else
    echo "directory $FILE does not exist."
    ok=0
fi
#check if fonts exists
FILE=fonts
if [ -d "$FILE" ]; then
    echo "directory $FILE exists."
else
    echo "directory $FILE does not exist."
    ok=0
fi
#check if images exists
FILE=images
if [ -d "$FILE" ]; then
    echo "directory $FILE exists."
else
    echo "directory $FILE does not exist."
    ok=0
fi
#check if music exists
FILE=music
if [ -d "$FILE" ]; then
    echo "directory $FILE exists."
else
    echo "directory $FILE does not exist."
    ok=0
fi

#check if dlls are all here
FILE=SDL2.dll
if [ -f "$FILE" ]; then
    echo "file $FILE exists."
else 
    echo "file $FILE does not exist."
    ok=0
fi
FILE=SDL2_image.dll
if [ -f "$FILE" ]; then
    echo "file $FILE exists."
else 
    echo "file $FILE does not exist."
    ok=0
fi
FILE=SDL2_mixer.dll
if [ -f "$FILE" ]; then
    echo "file $FILE exists."
else 
    echo "file $FILE does not exist."
    ok=0
fi
FILE=SDL2_net.dll
if [ -f "$FILE" ]; then
    echo "file $FILE exists."
else 
    echo "file $FILE does not exist."
    ok=0
fi
FILE=SDL2_ttf.dll
if [ -f "$FILE" ]; then
    echo "file $FILE exists."
else 
    echo "file $FILE does not exist."
    ok=0
fi
FILE=libncursesw6.dll
if [ -f "$FILE" ]; then
    echo "file $FILE exists."
else 
    echo "file $FILE does not exist."
    ok=0
fi
FILE=SDL2
if [ -d "$FILE" ]; then
    echo "directory $FILE exists."
else 
    echo "directory $FILE does not exist."
    ok=0
fi

if [ $ok -eq 1 ]; then
    echo "All files and directories are here."
else
    echo "Some files are missing. Please reinstall the game."
fi


cd ../
