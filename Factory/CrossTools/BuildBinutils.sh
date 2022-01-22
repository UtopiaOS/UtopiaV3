#!/usr/bin/env bash

SHA256SUM="sha256sum"
AWK="awk"
REALPATH="realpath"
MAKE="make"
NPROC="nproc"
INSTALL="install"
SED="sed"

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

# === Patch phase ===
pushd "$SOURCES_DIR/$BINUTILS_NAME"
    buildstep patching echo "Patching binutils"
    if [[ ! -f ".patched" ]]; then
        patch -Np0 -i "$DIR/Patches/gnu/binutils/0001-guess_utopia_libc.patch"
        patch -Np0 -i "$DIR/Patches/gnu/binutils/0002-utopia_valid_os.patch"
        touch .patched
    else 
        echo "Already patched binutils!"
    fi
popd

mkdir -p $BUILD_DIR/binutils

# === Build phase ===
pushd "$BUILD_DIR/binutils"
    if [ ! -f ".built_$PHASE" ]; then
        unset CFLAGS
        unset CXXFLAGS
        $SOURCES_DIR/$BINUTILS_NAME/configure --prefix=${CROSS_LOCATION} \
        --target=${TARGET_TRIPLE} \
        --with-sysroot="${CROSS_LOCATION}/${TARGET_TRIPLE}" \
        --disable-nls \
        --disable-multilib \
        --disable-werror \
        --enable-deterministic-archives \
        --disable-compressed-debug-sections

        buildstep binutils/configure_host make configure-host

        buildstep binutils/make make
        buildstep binutils/install make install
        touch .built_$PHASE
    else
        echo "Binutils has been built already"
    fi
popd