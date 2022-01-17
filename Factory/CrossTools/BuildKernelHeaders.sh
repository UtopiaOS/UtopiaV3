#!/usr/bin/env bash
set -eo pipefail

## TODO: Stop hardcoding x86 here

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


# === COMPILE AND INSTALL ===

pushd $SOURCES_DIR/$KERNEL_HEADERS_NAME
    if [ ! -f ".built_$PHASE" ]; then
        buildstep "kernel-headers/clean" make mrproper
        make ARCH=x86 headers
        mkdir -pv "$CROSS_LOCATION/$TARGET_TRIPLE/include"
        cp -rv usr/include/* "$CROSS_LOCATION/$TARGET_TRIPLE/include"
        touch .built_$PHASE
    else
        echo "Kernel headers has been built already"
    fi
popd


pushd "$CROSS_LOCATION/$TARGET_TRIPLE/include"
    if [ ! -f ".cleaned_$PHASE" ]; then
        find . -name '.*.cmd' -exec rm -vf {} \;
        rm -v Makefile
        touch .cleaned_$PHASE
    else
        echo "Kernel headers has been cleaned already"
    fi
popd