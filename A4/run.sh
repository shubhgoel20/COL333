#!/bin/bash
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <>.bif <>.dat"
    exit 1
fi
arg1="$1"
arg2="$2"
./a4 $arg1 $arg2