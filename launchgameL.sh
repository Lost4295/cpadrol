#!/bin/bash
 
cd final
#check if cfiles exists
FILE=main
if [ -f "$FILE" ]; then
    echo "file $FILE exists."
    ./main
else
    echo "file $FILE does not exist. Run the script to compile the game."
fi