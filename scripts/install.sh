#!/bin/bash

DIR="$1"
shift

mkdir -p "$DIR"

for SOURCE in "$@" ; do
    BASE=$(basename "$SOURCE")
    echo cp "./$BASE" "$DIR$BASE"
    cp "./$BASE" "$DIR$BASE"
done
