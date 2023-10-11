#!/bin/bash

green='\033[0;32m'
clear='\033[0m'

string="$1"

search_core() {
    pid="$1"
    echo "$(ps -p $pid -o comm= 2>/dev/null)($pid)"
    gcore "$pid" > /dev/null 2>&1
    if grep -q "$string" core."$pid" > /dev/null 2>&1; then
        echo -e "${green}$(ps -p $pid -o comm=) $pid CONTAINS STRING '$string'${clear}"
    fi
    rm core."$pid" > /dev/null 2>&1
}

chunk() {
    local list=("$@")
    local chunk_size=$1
    shift

    for i in $(seq 0 $chunk_size $((${#list[@]} - 1))); do
        echo "${list[@]:$i:$chunk_size}"
    done
}

pids=($(ps axo pid))
num_cores=$(nproc)
chunk_size=$(( (${#pids[@]} + num_cores - 1) / num_cores ))

IFS=$'\n' read -rd '' -a chunks <<< "$(chunk $chunk_size "${pids[@]}")"

for pid_chunk in "${chunks[@]}"; do
    {
        for pid in $pid_chunk; do
            search_core "$pid"
        done
    } &
done

wait
