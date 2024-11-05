#!/bin/bash
if [ -z "$1" ]; then
    echo "Usage: $0 <filename>"
    exit 1
fi

gcc src/*.c -o goud
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

./goud "$1"
