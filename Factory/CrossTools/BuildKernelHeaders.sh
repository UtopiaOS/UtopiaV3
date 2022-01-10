#!/usr/bin/env bash

# === CONFIGURATION AND SETUP === 

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo $DIR

PREFIX="$DIR/Local/kernel-headers/"
BUILD="$DIR/../Build"
CROSSTOOLS="$DIR/Cross"


SHA256SUM="sha256sum"
AWK="awk"
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

KERNEL_HEADERS_VERSION="5.15.13"
KERNEL_HEADERS_MAJOR="$(echo $KERNEL_HEADERS_VERSION | awk '{split($0,a,"."); print a[1]}')" 
KERNEL_HEADERS_SHA256SUM="0a131b6a2f9f5ee37ecb332b5459ab35a87f0bf2d4ec923988d0663646cf156a"
KERNEL_HEADERS_PKG="linux-$KERNEL_HEADERS_VERSION.tar.xz"
KERNEL_HEADERS_URL="https://cdn.kernel.org/pub/linux/kernel/v$KERNEL_HEADERS_MAJOR.x/$KERNEL_HEADERS_PKG"

buildstep() {
    NAME=$1
    shift
    "$@" 2>&1 | "$SED" $'s|^|\e[34m['"${NAME}"$']\e[39m |'
}

# === Lets check if we have all the proper dependencies === 

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


# === TODO: Make it so a user can reuse the toolchain ===


# === DOWNLOAD AND PATCH ===

pushd "$DIR/Tarballs"
    sha256=""
    if [ -e "$KERNEL_HEADERS_PKG" ]; then
        sha256="$($SHA256SUM ${KERNEL_HEADERS_PKG} | cut -f1 -d' ')"
        echo "kernel-headers sha256='$sha256'"
    fi

    if [ "$sha256" != "$KERNEL_HEADERS_SHA256SUM" ]; then
        rm -f "$KERNEL_HEADERS_PKG"
        curl -LO "$KERNEL_HEADERS_URL"
    else
        echo "Skipped downloading the kernel headers"
    fi

    if [ -d "$KERNEL_HEADERS_NAME" ]; then
        # Drop the previous source and the build dir
        rm -rf "${KERNEL_HEADERS_NAME}"
        rm -rf "$DIR/Build/kernel-headers"
    fi
    
    echo "Extracting Kernel headers..."
    tar -xvf "$KERNEL_HEADERS_PKG"
popd



mkdir -p "$CROSSTOOLS"

# === COMPILE AND INSTALL ===

## TODO: Stop hardcoding x86 here

pushd "$DIR/Tarballs/linux-$KERNEL_HEADERS_VERSION"
    buildstep "kernel-headers/clean" make mrproper
    make ARCH=x86 headers
    mkdir -pv "$CROSSTOOLS/x86_64-pc-linux-musl/include"
    cp -rv usr/include/* "$CROSSTOOLS/x86_64-pc-linux-musl/include"
popd


pushd "$CROSSTOOLS/x86_64-pc-linux-musl/include"
    find . -name '.*.cmd' -exec rm -vf {} \;
    rm -v . Makefile
popd