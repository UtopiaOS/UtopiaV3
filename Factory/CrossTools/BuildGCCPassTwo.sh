#!/usr/bin/env bash
set -eo pipefail

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

# == Patch gcc ==
pushd "$SOURCES_DIR/$GCC_NAME"
    patches="$DIR/CrossTools/Patches/gnu/gcc"
    if [ ! -f ".support_patch" ]; then
        buildstep gcc/stageTwo/patch/0044 patch -Np0 -i "$patches/0044-linux_decisions.patch"
        buildstep gcc/stageTwo/patch/0045 patch -Np0 -i "$patches/0045-config.gcc-linux-common-and-utopia.patch"
        buildstep gcc/stageTwo/patch/0046 patch -Np0 -i "$patches/0046-add_mt_utopia.patch"
        buildstep gcc/stageTwo/patch/0047 patch -Np0 -i "$patches/0047-configure_support_utopia.patch"
        buildstep gcc/stageTwo/patch/0048 patch -Np0 -i "$patches/0048-configure_ac_support_utopia.patch"
        buildstep gcc/stageTwo/patch/0049 patch -Np0 -i "$patches/0049-config-sub-utopia.patch"
        buildstep gcc/stageTwo/patch/0050 patch -Np0 -i "$patches/0050-utopia-basic-definition.patch"
        buildstep gcc/stageTwo/patch/0051 patch -Np0 -i "$patches/0051-utopia-driver.patch"
        buildstep gcc/stageTwo/patch/0052 patch -Np0 -i "$patches/0052-linux-common-use-linux-decisions.patch"
        touch .support_patch
    else
        echo "Support patches for Utopia have already been applied"
    fi
    if [ ! -f ".musl_specific_patch" ]; then
        buildstep gcc/stageTwo/patch/0053 patch -Np0 -i "$patches/0053-utopia_is_generic.patch"
        buildstep gcc/stageTwo/patch/0001 patch -Np1 -i "$patches/0001-posix_memalign.patch"
        buildstep gcc/stageTwo/patch/0002 patch -Np1 -i "$patches/0002-gcc-poison-system-directories.patch"
        buildstep gcc/stageTwo/patch/0003 patch -Np1 -i "$patches/0003-Turn-on-Wl-z-relro-z-now-by-default.patch"
        buildstep gcc/stageTwo/patch/0004 patch -Np1 -i "$patches/0004-Turn-on-D_FORTIFY_SOURCE-2-by-default-for-C-C-ObjC-O.patch"
        buildstep gcc/stageTwo/patch/0006 patch -Np1 -i "$patches/0006-Enable-Wformat-and-Wformat-security-by-default.patch"
        buildstep gcc/stageTwo/patch/0007 patch -Np1 -i "$patches/0007-Enable-Wtrampolines-by-default.patch"
        buildstep gcc/stageTwo/patch/0009 patch -Np1 -i "$patches/0009-Ensure-that-msgfmt-doesn-t-encounter-problems-during.patch"
        buildstep gcc/stageTwo/patch/0010 patch -Np1 -i "$patches/0010-Don-t-declare-asprintf-if-defined-as-a-macro.patch"
        buildstep gcc/stageTwo/patch/0011 patch -Np1 -i "$patches/0011-libiberty-copy-PIC-objects-during-build-process.patch"
        buildstep gcc/stageTwo/patch/0012 patch -Np1 -i "$patches/0012-libitm-disable-FORTIFY.patch"
        buildstep gcc/stageTwo/patch/0013 patch -Np1 -i "$patches/0013-libgcc_s.patch"
        buildstep gcc/stageTwo/patch/0014 patch -Np1 -i "$patches/0014-nopie.patch"
        buildstep gcc/stageTwo/patch/0015 patch -Np1 -i "$patches/0015-libffi-use-__linux__-instead-of-__gnu_linux__-for-mu.patch"
        buildstep gcc/stageTwo/patch/0016 patch -Np1 -i "$patches/0016-dlang-update-zlib-binding.patch"
        buildstep gcc/stageTwo/patch/0017 patch -Np1 -i "$patches/0017-dlang-fix-fcntl-on-mips-add-libucontext-dep.patch"
        buildstep gcc/stageTwo/patch/0018 patch -Np1 -i "$patches/0018-ada-fix-shared-linking.patch"
        buildstep gcc/stageTwo/patch/0019 patch -Np1 -i "$patches/0019-build-fix-CXXFLAGS_FOR_BUILD-passing.patch"
        buildstep gcc/stageTwo/patch/0020 patch -Np1 -i "$patches/0020-add-fortify-headers-paths.patch"
        buildstep gcc/stageTwo/patch/0023 patch -Np1 -i "$patches/0023-Pure-64-bit-MIPS.patch"
        buildstep gcc/stageTwo/patch/0024 patch -Np1 -i "$patches/0024-use-pure-64-bit-configuration-where-appropriate.patch"
        buildstep gcc/stageTwo/patch/0025 patch -Np1 -i "$patches/0025-always-build-libgcc_eh.a.patch"
        buildstep gcc/stageTwo/patch/0027 patch -Np1 -i "$patches/0027-ada-musl-support-fixes.patch"
        buildstep gcc/stageTwo/patch/0028 patch -Np1 -i "$patches/0028-gcc-go-Use-_off_t-type-instead-of-_loff_t.patch"
        buildstep gcc/stageTwo/patch/0029 patch -Np1 -i "$patches/0029-gcc-go-Don-t-include-sys-user.h.patch"
        buildstep gcc/stageTwo/patch/0030 patch -Np1 -i "$patches/0030-gcc-go-Fix-ucontext_t-on-PPC64.patch"
        buildstep gcc/stageTwo/patch/0031 patch -Np1 -i "$patches/0031-gcc-go-Fix-handling-of-signal-34-on-musl.patch"
        buildstep gcc/stageTwo/patch/0032 patch -Np1 -i "$patches/0032-gcc-go-Use-int64-type-as-offset-argument-for-mmap.patch"
        buildstep gcc/stageTwo/patch/0034 patch -Np1 -i "$patches/0034-gcc-go-signal-34-is-special-on-musl-libc.patch"
        buildstep gcc/stageTwo/patch/0035 patch -Np1 -i "$patches/0035-gcc-go-Prefer-_off_t-over-_off64_t.patch"
        buildstep gcc/stageTwo/patch/0036 patch -Np1 -i "$patches/0036-gcc-go-undef-SETCONTEXT_CLOBBERS_TLS-in-proc.c.patch"
        buildstep gcc/stageTwo/patch/0037 patch -Np1 -i "$patches/0037-gcc-go-link-to-libucontext.patch"
        buildstep gcc/stageTwo/patch/0038 patch -Np1 -i "$patches/0038-gcc-go-Disable-printing-of-unaccessible-ppc64-struct.patch"
        buildstep gcc/stageTwo/patch/0041 patch -Np1 -i "$patches/0041-Use-generic-errstr.go-implementation-on-musl.patch"
        buildstep gcc/stageTwo/patch/0042 patch -Np1 -i "$patches/0042-Disable-ssp-on-nostdlib-nodefaultlibs-and-ffreestand.patch"
        buildstep gcc/stageTwo/patch/0043 patch -Np1 -i "$patches/0043-configure-Add-enable-autolink-libatomic-use-in-LINK_.patch"
        touch .musl_specific_patch
    else
        echo "Musl-c patches for GCC have already been applied"
    fi
popd

mkdir -p $BUILD_DIR/gcc/passtwo

pushd "$BUILD_DIR/gcc/passtwo"
    echo $PATH
    if [ ! -f ".built_$PHASE" ]; then
        export PATH=$CROSS_LOCATION/bin:/bin:/usr/bin
        AR=ar \
        LDFLAGS="-Wl,-rpath,$CROSS_LOCATION/lib" \
        $SOURCES_DIR/$GCC_NAME/configure \
        --prefix=$CROSS_LOCATION \
        --build=${BUILDER} \
        --host=${BUILDER} \
        --target=${TARGET_TRIPLE} \
        --disable-multilib \
        --with-sysroot=${CROSS_LOCATION} \
        --disable-nls \
        --enable-shared \
        --enable-languages=c,c++ \
        --enable-threads=posix \
        --enable-clocale=generic \
        --enable-libstdcxx-time \
        --enable-fully-dynamic-string \
        --disable-symvers \
        --disable-libsanitizer \
        --disable-lto-plugin \
        --disable-libssp

        make AS_FOR_TARGET="${TARGET_TRIPLE}-as" \
            LD_FOR_TARGET="${TARGET_TRIPLE}-ld"

        buildstep gcc/stageTwo/install make install
        touch .built_$PHASE
    else
        echo "GCC pass two on $PHASE already built"
    fi
popd