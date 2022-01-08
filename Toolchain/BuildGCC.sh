#!/usr/bin/env bash

# === CONFIGURATION AND SETUP === 

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo $DIR

PREFIX="$DIR/Local/gcc/"
CROSSTOLS="$DIR/CrossTools"


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

GCC_VERSION="11.2.0"
GCC_NAME="gcc-$GCC_VERSION"
GCC_PKG="$GCC_NAME.tar.xz"
GCC_URL="https://ftp.gnu.org/gnu/gcc/$GCC_NAME/$GCC_PKG"
GCC_SHA256SUM="d08edc536b54c372a1010ff6619dd274c0f1603aa49212ba20f7aa2cda36fa8b"

buildstep() {
    NAME=$1
    shift
    "$@" 2>&1 | "$SED" $'s|^|\e[34m['"${NAME}"$']\e[39m |'
}

# == Dependency checking ==

buildstep dependencies echo "Checking whether 'make' is availible..."
if ! command -v ${MAKE:-make} >/dev/null; then
    buildstep dependencies echo "Please make sure to install GNU Make (for the '${MAKE:-make}' tool)."
    exit 1
fi

buildstep dependencies echo "Checking whether 'patch' is availible..."
if ! command -v ${PATCH:-patch} >/dev/null; then
    buildstep dependencies echo "Please make sure to install GNU patch (for the '${PATCH:-patch}' tool)."
    exit 1
fi

buildstep dependencies echo "Checking whether your C compiler works..."
if ! ${CC:-cc} -o /dev/null -xc - >/dev/null <<'PROGRAM'
int main() {}
PROGRAM
then
    buildstep dependencies echo "Please make sure to install a working C compiler"
    exit 1
fi

buildstep dependencies echo "Checking whether your C++ compiler works..."
if ! ${CXX:-c++} -o /dev/null -xc - >/dev/null <<'PROGRAM'
int main() {}
PROGRAM
then
    buildstep dependencies echo "Please make sure to install a working C++ compiler."
    exit 1
fi

pushd "$DIR/Tarballs"
    sha256=""
    if [ -e "$GCC_PKG" ]; then
        sha256="$($SHA256SUM ${GCC_PKG} | cut -f1 -d' ')"
        echo "gcc sha256='$sha256'"
    fi

    if [ "$sha256" != "$GCC_SHA256SUM" ]; then
        rm -rf "$GCC_PKG"
        curl -LO "$GCC_URL"
    else
        echo "Skipped downloading gcc"
    fi

    if [ -d "$GCC_NAME" ]; then
        rm -rf "${GCC_NAME}"
        rm -rf "$DIR/Build/gcc"
    fi

    echo "Extracting gcc..."
    tar -xvf "$KERNEL_HEADERS_PKG"
popd

pushd "$DIR/Tarballs/$GCC_NAME"
    buildstep patching echo "Patching GCC"
    patch -Np0 -i "$DIR/Patches/gnu/gcc/config.addutopia.patch"
    patch -Np0 -i "$DIR/Patches/gnu/gcc/utopiahighlevel.patch"
    patch -Np0 -i "$DIR/Patches/gnu/gcc/utopialinuxcommon.patch"
popd

