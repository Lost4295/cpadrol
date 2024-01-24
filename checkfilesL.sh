#!/bin/bash

ok=1
cd final
#check if cfiles exists
FILE=cfiles
if [ -d "$FILE" ]; then
    echo "$FILE exists."
else
    echo "$FILE does not exist."
    ok=0
fi
#check if fonts exists
FILE=fonts
if [ -d "$FILE" ]; then
    echo "$FILE exists."
else
    echo "$FILE does not exist."
    ok=0
fi
#check if images exists
FILE=images
if [ -d "$FILE" ]; then
    echo "$FILE exists."
else
    echo "$FILE does not exist."
    ok=0
fi
#check if music exists
FILE=music
if [ -d "$FILE" ]; then
    echo "$FILE exists."
else
    echo "$FILE does not exist."
    ok=0
fi

if [ $ok -eq 1 ]; then
    echo "All files are here."
else
    echo "Some files are missing. Please reinstall the game."
fi


cd ../