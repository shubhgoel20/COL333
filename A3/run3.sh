#!/bin/bash
inputgraph="$1.graph"
line=$(head -n 1 $inputgraph)
# assign n the first integer in line
n=$(echo $line | cut -d' ' -f1)

l=1
u=$(($n+1))

while [ $l -lt $u ]
do
    
    mid=$(((($l+$u))/2))
    if [ $mid -eq $l ]; then
        break
    fi

    echo "checking $mid"
    # Run the C++ binary with the argument
    ./exe/cnf_build_2 $1 $mid
    timeout 40 ./MiniSat_v1.14_linux $1.satinput $1.satoutput
    outfile="$1.satoutput"
    line=$(head -n 1 $outfile)
    if [ "$line" == "SAT" ]; then
        l=$(($mid))
        ./exe/result_parse_2 $1 $n
    else
        u=$(($mid))
fi
done