#!/usr/bin/env bash

# === CONFIGURATION AND SETUP === 

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo $DIR

PREFIX="$DIR/Local/gcc/"
BUILD="$DIR/../Build"


SHA256SUM="sha256sum"
REALPATH="realpath"
MAKE="make"
NPROC="nproc"
INSTALL="install"
SED="sed"

if [ -z "$MAKEJOBS" ]; then
    MAKEJOBS=$($NPROC)
fi

if [ ! -d "$BUILD" ]; then
    mkdir -p "$BUILD"
fi

BUILD=$($REALPATH "$BUILD")

echo PREFIX is "$PREFIX"

mkdir -p "$DIR/Tarballs"

GCC_VERSION=