#!/bin/bash

find_string_in_process() {
    local PID="$1"
    local SEARCH_STRING="$2"

    if [ -z "$PID" ] || [ -z "$SEARCH_STRING" ]; then
        echo "Usage: find_string_in_process <PID> <STRING>"
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

find_string_in_process $1 $2
