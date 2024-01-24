#!/bin/bash
 
cd final
#check if cfiles exists
FILE=menu
if [ -f "$FILE" ]; then
    echo "file $FILE exists."
    ./menu
else
    echo "file $FILE does not exist. Run the script to compile the game."
fi