#!/usr/bin/env bash


# === CONFIGURATION AND SETUP ===

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo $DIR

CROSSTOOLS="$DIR/Cross"
ARCH="x86_64"
TOOLS=$(realpath "$DIR/../Toolchain")/Tools
UTOPIA_TARGET="$ARCH-pc-linux-musl"

SHA256SUM="sha256sum"
REALPATH="realpath"
MAKE="make"
NPROC="nproc"
INSTALL="install"
SED="sed"

if [ -z "$MAKEJOBS" ]; then
    MAKEJOBS=$($NPROC)
fi

buildstep() {
    NAME=$1
    shift
    "$@" 2>&1 | "$SED" $'s|^|\e[34m['"${NAME}"$']\e[39m |'
}

echo PREFIX is "$PREFIX"

mkdir -p "$DIR/Tarballs"

# == MUSL SOURCE == 

MUSL_VERSION="1.2.2"
MUSL_NAME="musl-$MUSL_VERSION"
MUSL_PKG="$MUSL_NAME.tar.gz"
MUSL_URL="https://musl.libc.org/releases/$MUSL_PKG"
MUSL_SHA256SUM="9b969322012d796dc23dda27a35866034fa67d8fb67e0e2c45c913c3d43219dd"

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

# == DOWNLOAD PHASE ==

pushd "$DIR/Tarballs"
    sha256=""
    if [ -e "$MUSL_PKG" ]; then
        sha256="$($SHA256SUM ${MUSL_PKG} | cut -f1 -d ' ')"
        echo "musl sha256='$sha256'"
    fi

    if [ "$sha256" != "$MUSL_SHA256SUM" ]; then
        rm -rf "$MUSL_PKG"
        curl -LO "$MUSL_URL"
    else
        echo "Skipping downloading musl"
    fi

    echo "Extracting musl..."
    tar -xvf ${MUSL_PKG}
popd

# == BUILD ==

mkdir -p $DIR/Build/musl

pushd "$DIR/Build/musl"
    export PATH=$DIR/Cross/bin:/bin:/usr/bin
    buildstep musl/configure $DIR/Tarballs/$MUSL_NAME/configure CROSS_COMPILEs=${UTOPIA_TARGET}- \
    --prefix=/ \
    --target=${UTOPIA_TARGET}

    make
    DESTDIR=$CROSSTOOLS make install
popd

# == FIX BROKEN LINKS ==

pushd "$CROSSTOOLS"
    mkdir -v usr
    ln -sv ../include ./usr/include

    rm -vf lib/ld-musl-${ARCH}.so.1
    ln -sv libc.so lib/ld-musl-${ARCH}.so.1

    ln -sv ../lib/ld-musl-$ARCH.so.1 bin/ldd

    mkdir -pv etc
    if [ -f $CROSSTOOLS/etc/ld-musl-$ARCH.path ]; then
        rm -rf $CROSSTOOLS/etc/ld-musl-$ARCH.path
    fi
    # cat didnt work very well here...
    touch $CROSSTOOLS/etc/ld-musl-$ARCH.path
    echo "$CROSSTOOLS/lib" >> $CROSSTOOLS/etc/ld-musl-$ARCH.path
    echo "$TOOLS/lib" >> $CROSSTOOLS/etc/ld-musl-$ARCH.path
popd