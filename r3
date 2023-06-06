#!/bin/bash  
  
dir="$1"  
  
if [ ! -d "$dir" ]; then  
 echo "Please provide a valid directory as an argument."  
 exit 1  
fi  
  
find "$dir" -type f -name "*.*" | sed 's/.*\.//' | sort -u
