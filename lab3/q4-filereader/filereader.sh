#!/bin/bash
echo enter x,y

read -p 'X: ' x
read -p 'Y: ' y

readonly input="./testfile.txt"
counter=0

while IFS= read -r line; do
	((counter++))

        if [ $counter -ge $x ] && [ $counter -le $y ] ; then
               echo "$line"
        fi

done < "$input"
