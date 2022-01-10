#!/usr/bin/env bash


DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo "$DIR"

PREFIX="$DIR/CrossTools"
ARCH="x86_64"
UTOPIA_TARGET="$ARCH-pc-linux-musl"

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

echo PREFIX is "$PREFIX"

mkdir -p "$DIR/Tarballs"

BINUTILS_VERSION="2.37"
BINUTILS_SHA256SUM="820d9724f020a3e69cb337893a0b63c2db161dadcb0e06fc11dc29eb1e84a32c"
BINUTILS_NAME="binutils-$BINUTILS_VERSION"
BINUTILS_PKG="$BINUTILS_NAME.tar.xz"
BINUTILS_URL="https://ftp.gnu.org/gnu/binutils/$BINUTILS_PKG"

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


pushd "$DIR/Tarballs"
    sha256=""
    if [ -e "$BINUTILS_PKG" ]; then
        sha256="$($SHA256SUM ${BINUTILS_PKG} | cut -f1 -d' ')"
        echo "Binutils sha256='$sha256'"
    fi

    if [ "$sha256" != "$BINUTILS_SHA256SUM" ]; then
        rm -f "$BINUTILS_PKG"
        curl -LO "$BINUTILS_URL"
    else
        echo "Skipped downloading binutils"
    fi

    if [ -d "$BINUTILS_NAME" ]; then
        # Drop the previous source and the build dir
        rm -rf "${BINUTILS_NAME}"
        rm -rf "$DIR/Build/binutils"
    fi
    
    echo "Extracting binutils..."
    tar -xvf "$BINUTILS_PKG"
popd

# === Build phase ===
pushd "$DIR/Tarballs/$BINUTILS_NAME"
    buildstep binutils/configure ./configure --prefix=${PREFIX} \
    --target=${UTOPIA_TARGET} \
    --with-sysroot="${PREFIX}/${UTOPIA_TARGET}" \
    --disable-nls \
    --disable-multilib \
    --disable-werror \
    --enable-deterministic-archives \
    --disable-compressed-debug-sections

    buildstep binutils/configure_host make configure-host

    buildstep binutils/make make
    buildstep binutils/install make install
popd