#!/bin/bash

find_string_in_process() { # DO NOT USE
    local PID="$1"
    local SEARCH_STRING="$2"

    if [ -z "$PID" ] || [ -z "$SEARCH_STRING" ]; then
        echo "Usage: vas_search.sh <PID> <STRING>"
        return 1
    fi

    if [ ! -r "/proc/$PID/maps" ] || [ ! -r "/proc/$PID/mem" ]; then
        echo "Cannot access /proc/$PID/maps or /proc/$PID/mem. Please run with appropriate permissions."
        return 1
    fi

    grep r /proc/$PID/maps | while read -r line; do
        start=$(echo "$line" | awk '{print $1}' | cut -d'-' -f1)
        end=$(echo "$line" | awk '{print $1}' | cut -d'-' -f2)

        start_dec=$((0x$start))
        end_dec=$((0x$end))

        dd if=/proc/$PID/mem bs=1 skip=$start_dec count=$((end_dec-start_dec)) 2>/dev/null | grep -oba "$SEARCH_STRING" | while read -r content; do
            byte_offset=$(echo "$content" | cut -d':' -f1)

            address=$((start_dec+byte_offset))
            echo "Found '$SEARCH_STRING' at address: 0x$(printf '%x' $address)"
        done
    done
}

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <pid> <string>"
    exit 1
fi

PID=$1
STRING=$2
MEM_FILE="/proc/$PID/mem"

grep rw-p /proc/$PID/maps | while read -r line; do
    START_ADDR=$(echo $line | awk '{print $1}' | awk -F- '{print $1}')
    END_ADDR=$(echo $line | awk '{print $1}' | awk -F- '{print $2}')

    START_ADDR_DEC=$((0x$START_ADDR))
    END_ADDR_DEC=$((0x$END_ADDR))

    dd if=$MEM_FILE bs=1 skip=$START_ADDR_DEC count=$(($END_ADDR_DEC - $START_ADDR_DEC)) 2>/dev/null | \
    grep -oba "$STRING" | while read -r match; do
        OFFSET=$(echo $match | awk -F: '{print $1}')
        printf "Found string \"%s\" at address: 0x%x\n" "$STRING" "$((0x$START_ADDR + $OFFSET))"
    done

done
