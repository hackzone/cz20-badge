#!/bin/bash

COUNT=0

echo "Connect badges to program them"
while [ 1 ]
do
    if [ $(st-info --probe | grep 'unknown device' | wc -l) -eq "0" ]; then
        echo "Programming"
        ./flash.sh && ((COUNT=COUNT+1)) && echo "\nFlashed: ${COUNT}" && echo -ne '\007'

        while [ $(st-info --probe | grep 'unknown device' | wc -l) -eq "0" ]
        do
            sleep 0.1
        done
    fi

    sleep 0.1
done