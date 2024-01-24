#!/bin/bash
 
cd final
#check if cfiles exists
FILE=menu.exe
if [ -f "$FILE" ]; then
    echo "file $FILE exists."
    ./menu.exe
else
    echo "file $FILE does not exist. Run the script to compile the game."
fi