#!/usr/bin/env bash

# === CONFIGURATION AND SETUP === 

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo $DIR

PREFIX="$DIR/Local/gcc/"
CROSSTOOLS="$DIR/CrossTools"
ARCH="x86_64"
CPU="x86-64"
UTOPIA_TARGET="$ARCH-pc-linux-utopia"

UTOPA_BUILDER=""$(echo $MACHTYPE | \
    sed "s/$(echo $MACHTYPE | cut -d- -f2)/cross/")""


SHA256SUM="sha256sum"
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

# == GCC == 

GCC_VERSION="11.2.0"
GCC_NAME="gcc-$GCC_VERSION"
GCC_PKG="$GCC_NAME.tar.xz"
GCC_URL="https://ftp.gnu.org/gnu/gcc/$GCC_NAME/$GCC_PKG"
GCC_SHA256SUM="d08edc536b54c372a1010ff6619dd274c0f1603aa49212ba20f7aa2cda36fa8b"

# == MPFR ==

MPFR_VERSION="4.1.0"
MPFR_NAME="mpfr-$MPFR_VERSION"
MPFR_PKG="$MPFR_NAME.tar.xz"
MPFR_URL="https://www.mpfr.org/$MPFR_NAME/$MPFR_PKG"
MPFR_SHA256SUM="0c98a3f1732ff6ca4ea690552079da9c597872d30e96ec28414ee23c95558a7f"

# == GMP ==

GMP_VERSION="6.2.1"
GMP_NAME="gmp-$GMP_VERSION"
GMP_PKG="$GMP_NAME.tar.xz"
GMP_URL="https://ftp.gnu.org/gnu/gmp/$GMP_PKG"
GMP_SHA256SUM="fd4829912cddd12f84181c3451cc752be224643e87fac497b69edddadc49b4f2"

# == MPC ==

MPC_VERSION="1.2.1"
MPC_NAME="mpc-$MPC_VERSION"
MPC_PKG="$MPC_NAME.tar.gz"
MPC_URL="https://ftp.gnu.org/gnu/mpc/$MPC_PKG"
MPC_SHA256SUM="17503d2c395dfcf106b622dc142683c1199431d095367c6aacba6eec30340459"

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

# == DOWNLOAD PHASE == 

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
    tar -xvf "$GCC_PKG"
popd

pushd "$DIR/Tarballs"
    sha256=""
    if [ -e "$MPFR_PKG" ]; then
        sha256="$($SHA256SUM ${MPFR_PKG} | cut -f1 -d' ')"
        echo "mpfr sha256='$sha256'"
    fi

    if [ "$sha256" != "$MPFR_SHA256SUM" ]; then
        rm -rf "${MPFR_PKG}"
        curl -LO "${MPFR_URL}"
    else
        echo "Skipping downloading mpfr"
    fi

    # We want to put this inside the GCC folder
    pushd $GCC_NAME
        # we do mv to change the name to mpfr, here we check for either name, in case mv failed
        if [ -d "mpfr" || -d "$MPFR_NAME"];  then
            rm -rf "${MPFR_NAME}"
            rm -rf "mpfr"
        fi

        echo "Extracting mpfr to the GCC directory"
        xz -cd ../${MPFR_PKG} | tar -xvf -
        mv ${MPFR_NAME} "mpfr"
    popd

    # clear our sha256sum we calculated before
    sha256=""
    if [ -e "$GMP_PKG" ]; then
        sha256="$($SHA256SUM ${GMP_PKG} | cut -f1 -d' ')"
        echo "gmp sha256='$sha256'"
    fi

    if [ "$sha256" != "$GMP_SHA256SUM" ]; then
        rm -rf "${GMP_PKG}"
        curl -LO "${GMP_URL}"
    else
        echo "Skipping downloading gmp"
    fi

    # We want to put this inside the GCC folder
    pushd $GCC_NAME
        # we do mv to change the name to gmp, here we check for either name, in case mv failed
        if [ -d "gmp" || -d "$GMP_NAME" ];  then
            rm -rf "${GMP_NAME}"
            rm -rf "gmp"
        fi

        echo "Extracting gmp to the GCC directory"
        xz -cd ../${GMP_PKG} | tar -xvf -
        mv ${GMP_NAME} "gmp"
    popd

    # reset the sha256 for the last time
    sha256=""
    if [ -e "$MPC_PKG" ]; then
        sha256="$($SHA256SUM ${MPC_PKG} | cut -f1 -d' ')"
        echo "mpc sha256='$sha256'"
    fi

    if [ "$sha256" != "$MPC_SHA256SUM" ]; then
        rm -rf "${MPC_PKG}"
        curl -LO "${MPC_URL}"
    else
        echo "Skipping downloading MPC"
    fi

    pushd $GCC_NAME
        if [ -d "mpc" || -d "$MPC_NAME" ]; then
            rm -rf "${MPC_NAME}"
            rm -rf "mpc"
        fi

        echo "Extracting mpc to the GCC directory"
        gzip -cd ../${MPC_PKG} | tar -xvf -
        mv ${MPC_NAME} "mpc"
    popd
popd

# == PATCHES ==

pushd "$DIR/Tarballs/$GCC_NAME"
    buildstep patching echo "Patching GCC"
    patch -Np1 -i "$DIR/Patches/gnu/gcc/config.addutopia.patch"
    patch -Np0 -i "$DIR/Patches/gnu/gcc/gcc_valid_utopia.patch"
    patch -Np1 -i "$DIR/Patches/gnu/gcc/utopiahighlevel.patch"
    patch -Np1 -i "$DIR/Patches/gnu/gcc/utopialinuxcommon.patch"
popd


# == BUILD ==

mkdir -p $DIR/Build/gcc

pushd "$DIR/Build/gcc"
    buildstep gcc/configure $DIR/Tarballs/$GCC_NAME/configure --prefix=${CROSSTOOLS} --build=${UTOPIA_BUILDER} \
    --host=${UTOPIA_BUILDER} --target=${UTOPIA_TARGET} \
    --with-sysroot=${CROSSTOOLS}/${UTOPIA_TARGET} --disable-nls \
    --with-newlib --disable-libitm --disable-libvtv --disable-libssp --disable-shared  \
    --disable-libgomp --without-headers --disable-threads --disable-multilib \
    --disable-libatomic --disable-libstdcxx  --enable-languages=c --disable-libquadmath \
    --disable-libsanitizer --with-arch=${CPU} \
    --disable-decimal-float --enable-clocale=generic

    buildstep gcc/libgcc make all-gcc all-target-libgcc

    buildstep gcc/install make install-gcc install-target-libgcc
popd