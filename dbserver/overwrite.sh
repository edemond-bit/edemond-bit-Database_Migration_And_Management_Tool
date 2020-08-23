#!/bin/bash
#
# Update the first line of the file with 
#
INPUTFILE=$1
OUTPUTFILE=temp99.txt
echo "ON" > $OUTPUTFILE
awk '(NR > 2) {$1=$1-1; print}' $INPUTFILE >> $OUTPUTFILE
