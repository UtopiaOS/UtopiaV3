#!/usr/bin/env bash

SHA256SUM="sha256sum"
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

# == Patch to support Utopia  and copy over mpfr, gmp and mpc==

pushd "$SOURCES_DIR/$GCC_NAME"
    if [ ! -f ".support_patch" ]; then
        buildstep patching echo "Patching GCC"
        patch -Np0 -i "$DIR/CrossTools/Patches/gnu/gcc/0044-linux_decisions.patch"
        patch -Np0 -i "$DIR/CrossTools/Patches/gnu/gcc/0045-config.gcc-linux-common-and-utopia.patch"
        patch -Np0 -i "$DIR/CrossTools/Patches/gnu/gcc/0046-add_mt_utopia.patch"
        patch -Np0 -i "$DIR/CrossTools/Patches/gnu/gcc/0047-configure_support_utopia.patch"
        patch -Np0 -i "$DIR/CrossTools/Patches/gnu/gcc/0048-configure_ac_support_utopia.patch"
        patch -Np0 -i "$DIR/CrossTools/Patches/gnu/gcc/0049-config-sub-utopia.patch"
        patch -Np0 -i "$DIR/CrossTools/Patches/gnu/gcc/0050-utopia-basic-definition.patch"
        patch -Np0 -i "$DIR/CrossTools/Patches/gnu/gcc/0051-utopia-driver.patch"
        patch -Np0 -i "$DIR/CrossTools/Patches/gnu/gcc/0052-linux-common-use-linux-decisions.patch"
        touch .support_patch
    else
        echo "GCC was already patched to support Utopia"
    fi

    if [ ! -d "mpc" ]; then
        cp -vr $SOURCES_DIR/$MPC_NAME "mpc"
    else
        echo "mpc was already copied"
    fi

    if [ ! -d "mpfr" ]; then
        cp -vr $SOURCES_DIR/$MPFR_NAME "mpfr"
    else
        echo "mpfr was already copied"
    fi

    if [ ! -d "gmp" ]; then
        cp -vr $SOURCES_DIR/$GMP_NAME "gmp"
    else
        echo "gmp was already copied"
    fi

popd



# == BUILD ==

mkdir -p $BUILD_DIR/gcc/passone

pushd "$BUILD_DIR/gcc/passone"
    CFLAGS='-g0 -O0' \
    CXXFLAGS='-g0 -O0' \
    $SOURCES_DIR/$GCC_NAME/configure \
    --prefix=${CROSS_LOCATION} --build=${BUILDER} \
    --host=${BUILDER} --target=${TARGET_TRIPLE} \
    --with-sysroot=${CROSS_LOCATION}/${TARGET_TRIPLE} --disable-nls \
    --with-newlib --disable-libitm --disable-libvtv --disable-libssp --disable-shared  \
    --disable-libgomp --without-headers --disable-threads --disable-multilib \
    --disable-libatomic --disable-libstdcxx  --enable-languages=c --disable-libquadmath \
    --disable-libsanitizer --with-arch=${CPU} \
    --disable-decimal-float --enable-clocale=generic

    make all-gcc all-target-libgcc

    buildstep gcc/install make install-gcc install-target-libgcc
popd