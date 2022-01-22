#!/bin/bash

# Patch mpfr
pushd $SOURCES_DIR/$MPFR_NAME
    if [ ! -f ".patched" ]; then
        patch -Np0 -i "$DIR/Patches/gnu/mpfr/0001-utopia_valid_os.patch"
        patch -Np0 -i "$DIR/Patches/gnu/mpfr/0002-guess_utopia_libc.patch"
        touch .patched
    fi
popd

# patch gmp
pushd $SOURCES_DIR/$GMP_NAME
    if [ ! -f ".patched" ]; then
        patch -Np0 -i "$DIR/Patches/gnu/gmp/0001-guess_utopia_libc.patch"
        patch -Np0 -i "$DIR/Patches/gnu/gmp/0002-utopia_valid_os.patch"
        touch .patched
    fi
popd

# patch mpc
pushd $SOURCES_DIR/$MPC_NAME
    if [ ! -f ".patched" ]; then
        patch -Np0 -i "$DIR/Patches/gnu/mpc/0001-guess_utopia_libc.patch"
        patch -Np0 -i "$DIR/Patches/gnu/mpc/0002-utopia_valid_os.patch"
        touch .patched
    fi
popd