#!/bin/bash

architectures="arm riscv native"

if [[ $# -eq 0 ]] || [[ ! $architectures == *$1* ]]; then
    echo "Usage: buildAll.sh arm|riscv|native"
    exit 1
elif [ "$1" = "riscv" ] && [ $# -lt 2 ]; then
    echo "Usage for RISCV: buildAll.sh riscv device|spike"
    exit 1
fi

ignore_directories=("arm" "riscv" "native" "test")

# Loop through subdirectories
for subdir in $(dirname "$0")/*/; do
    # Get the name of the directory without the path
    dir_name=$(basename "$subdir")
    echo $dir_name
    if [[ ! $(echo ${ignore_directories[@]} | fgrep -w $dir_name) ]]; then
        make -C $subdir clean ARCH=$1 PLATFORM=$2
        make -C $subdir ARCH=$1 PLATFORM=$2
        cp $subdir/main.elf $(dirname "$0")/$dir_name.elf
        echo
    fi
done
