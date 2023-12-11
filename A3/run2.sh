#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Usage: $0 <test>"
    exit 1
fi

input="$1"

# Run the C++ binary with the argument
./exe/result_parse ${input} 