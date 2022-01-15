#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

CROSSTOOLS="$DIR/Cross"

UTOPIA_BUILDER=""$(echo $MACHTYPE | \
    sed "s/$(echo $MACHTYPE | cut -d- -f2)/cross/")""

ARCH="x86_64"
CPU="x86-64"
UTOPIA_TARGET="$ARCH-pc-linux-utopia"

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

GCC_VERSION="11.2.0"
GCC_NAME="gcc-$GCC_VERSION"
GCC_PKG="$GCC_NAME.tar.xz"

MPFR_VERSION="4.1.0"
MPFR_NAME="mpfr-$MPFR_VERSION"
MPFR_PKG="$MPFR_NAME.tar.xz"

GMP_VERSION="6.2.1"
GMP_NAME="gmp-$GMP_VERSION"
GMP_PKG="$GMP_NAME.tar.xz"

MPC_VERSION="1.2.1"
MPC_NAME="mpc-$MPC_VERSION"
MPC_PKG="$MPC_NAME.tar.gz"

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

## Since this is phase two, we go directly to building, except in a new directory

pushd "$DIR/Tarballs/"
    if [ -d "$GCC_NAME" ]; then
        rm -rf "${GCC_NAME}"
    else
        echo "Critical! There was no $GCC_NAME in Tarballs, are you sure you built phase one?"
        exit 1
    fi

    echo "Cleanly extracting GCC"

    tar -xvf "$GCC_PKG"

    echo "Extracting the GCC boostrap"

    echo "Extracting MPFR"

    pushd "$GCC_NAME"
        xz -cd ../${MPFR_PKG} | tar -xvf -
        mv ${MPFR_NAME} "mpfr"
    popd

    echo "Extracting gmp"

    pushd "$GCC_NAME"
        xz -cd ../${GMP_PKG} | tar -xvf -
        mv ${GMP_NAME} "gmp"
    popd

    echo "Extracting mpc"

    pushd "$GCC_NAME"
        gzip -cd ../${MPC_PKG} | tar -xvf -
        mv ${MPC_NAME} "mpc"
    popd
popd

# == Patch gcc ==
pushd "$DIR/Tarballs/$GCC_NAME"
    patches="$DIR/Patches/gnu/gcc"
    patch -Np0 -i "$DIR/Patches/gnu/gcc/0044-linux_decisions.patch"
    patch -Np0 -i "$DIR/Patches/gnu/gcc/0045-config.gcc-linux-common-and-utopia.patch"
    patch -Np0 -i "$DIR/Patches/gnu/gcc/0046-add_mt_utopia.patch"
    patch -Np0 -i "$DIR/Patches/gnu/gcc/0047-configure_support_utopia.patch"
    patch -Np0 -i "$DIR/Patches/gnu/gcc/0048-configure_ac_support_utopia.patch"
    patch -Np0 -i "$DIR/Patches/gnu/gcc/0049-config-sub-utopia.patch"
    patch -Np0 -i "$DIR/Patches/gnu/gcc/0050-utopia-basic-definition.patch"
    patch -Np0 -i "$DIR/Patches/gnu/gcc/0051-utopia-driver.patch"
    patch -Np0 -i "$DIR/Patches/gnu/gcc/0052-linux-common-use-linux-decisions.patch"
    patch -Np1 -i $patches/0001-posix_memalign.patch
    patch -Np1 -i $patches/0002-gcc-poison-system-directories.patch
    patch -Np1 -i $patches/0003-Turn-on-Wl-z-relro-z-now-by-default.patch
    patch -Np1 -i $patches/0004-Turn-on-D_FORTIFY_SOURCE-2-by-default-for-C-C-ObjC-O.patch
    patch -Np1 -i $patches/0006-Enable-Wformat-and-Wformat-security-by-default.patch
    patch -Np1 -i $patches/0007-Enable-Wtrampolines-by-default.patch
    patch -Np1 -i $patches/0009-Ensure-that-msgfmt-doesn-t-encounter-problems-during.patch
    patch -Np1 -i $patches/0010-Don-t-declare-asprintf-if-defined-as-a-macro.patch
    patch -Np1 -i $patches/0011-libiberty-copy-PIC-objects-during-build-process.patch
    patch -Np1 -i $patches/0012-libitm-disable-FORTIFY.patch
    patch -Np1 -i $patches/0013-libgcc_s.patch
    patch -Np1 -i $patches/0014-nopie.patch
    patch -Np1 -i $patches/0015-libffi-use-__linux__-instead-of-__gnu_linux__-for-mu.patch
    patch -Np1 -i $patches/0016-dlang-update-zlib-binding.patch
    patch -Np1 -i $patches/0017-dlang-fix-fcntl-on-mips-add-libucontext-dep.patch
    patch -Np1 -i $patches/0018-ada-fix-shared-linking.patch
    patch -Np1 -i $patches/0019-build-fix-CXXFLAGS_FOR_BUILD-passing.patch
    patch -Np1 -i $patches/0020-add-fortify-headers-paths.patch
    patch -Np1 -i $patches/0023-Pure-64-bit-MIPS.patch
    patch -Np1 -i $patches/0024-use-pure-64-bit-configuration-where-appropriate.patch
    patch -Np1 -i $patches/0025-always-build-libgcc_eh.a.patch
    patch -Np1 -i $patches/0027-ada-musl-support-fixes.patch
    patch -Np1 -i $patches/0028-gcc-go-Use-_off_t-type-instead-of-_loff_t.patch
    patch -Np1 -i $patches/0029-gcc-go-Don-t-include-sys-user.h.patch
    patch -Np1 -i $patches/0030-gcc-go-Fix-ucontext_t-on-PPC64.patch
    patch -Np1 -i $patches/0031-gcc-go-Fix-handling-of-signal-34-on-musl.patch
    patch -Np1 -i $patches/0032-gcc-go-Use-int64-type-as-offset-argument-for-mmap.patch
    patch -Np1 -i $patches/0034-gcc-go-signal-34-is-special-on-musl-libc.patch
    patch -Np1 -i $patches/0035-gcc-go-Prefer-_off_t-over-_off64_t.patch
    patch -Np1 -i $patches/0036-gcc-go-undef-SETCONTEXT_CLOBBERS_TLS-in-proc.c.patch
    patch -Np1 -i $patches/0037-gcc-go-link-to-libucontext.patch
    patch -Np1 -i $patches/0038-gcc-go-Disable-printing-of-unaccessible-ppc64-struct.patch
    patch -Np1 -i $patches/0041-Use-generic-errstr.go-implementation-on-musl.patch
    patch -Np1 -i $patches/0042-Disable-ssp-on-nostdlib-nodefaultlibs-and-ffreestand.patch
    patch -Np1 -i $patches/0043-configure-Add-enable-autolink-libatomic-use-in-LINK_.patch
    patch -Np1 -i $patches/0022-DP-Use-push-state-pop-state-for-gold-as-well-when-li.patch
popd

mkdir -p $DIR/Build/gccTwo

pushd "$DIR/Build/gccTwo"
    AR=ar \
    LDFLAGS="-Wl,-rpath,$CROSSTOOLS/lib" \
    $DIR/Tarballs/$GCC_NAME/configure \
    --prefix=$CROSSTOOLS --build=${UTOPIA_BUILDER} --host=${UTOPIA_BUILDER} --target=${UTOPIA_TARGET} --disable-multilib \
    --with-sysroot=${CROSSTOOLS} --disable-nls --enable-shared --enable-languages=c,c++ --enable-threads=posix --enable-clocale=generic \
    --enable-libstdcxx-time --enable-fully-dynamic-string --disable-symvers --disable-libsanitizer --disable-lto-plugin --disable-libssp

    buildstep gcc/stageTwo/make make AS_FOR_TARGET="${UTOPIA_TARGET}-as" LD_FOR_TARGET="${UTOPIA_TARGET}-ld"

    buildstep gcc/stageTwo/install make install
popd