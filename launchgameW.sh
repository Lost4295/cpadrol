#!/bin/bash
 
cd final
#check if cfiles exists
FILE=main.exe
if [ -f "$FILE" ]; then
    echo "file $FILE exists."
    ./main.exe
else
    echo "file $FILE does not exist. Run the script to compile the game."
fi