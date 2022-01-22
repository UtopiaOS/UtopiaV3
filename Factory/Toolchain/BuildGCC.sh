#!/bin/bash
set -eo pipefail

CC="${TARGET_TRIPLE}-gcc"
CXX="${TARGET_TRIPLE}-g++"
AR="${TARGET_TRIPLE}-ar"
AS="${TARGET_TRIPLE}-as"
RANLIB="${TARGET_TRIPLE}-ranlib"
LD="${TARGET_TRIPLE}-ld"
STRIP="${TARGET_TRIPLE}-strip"


pushd $SOURCES_DIR/$GCC_NAME
    cat gcc/{limitx,glimits,limity}.h > \
        $(dirname $($TARGET_TRIPLE-gcc -print-libgcc-file-name))/include-fixed/limits.h

    for file in gcc/config/{linux,i386/linux{,64}}.h
    do
        cp -uv $file{,.orig}
        sed -e "s|\/Core\/Binaries\/linker|${TOOLS_LOCATION}&|g" \
            -e "s|\/usr|${TOOLS_LOCATION}|g" ${file}.orig > $file
    echo "
#undef STANDARD_STARTFILE_PREFIX_1
#undef STANDARD_STARTFILE_PREFIX_2
#define STANDARD_STARTFILE_PREFIX_1 \"${TOOLS_LOCATION}/lib\"
#define STANDARD_STARTFILE_PREFIX_2 \"\"" >> ${file}
    touch ${file}.orig
done
popd

pushd $SOURCES_DIR/$GCC_NAME
    if [ -f ".no_explicit_link" ]; then
        patch -Np0 "$DIR/Patches/gnu/gcc/0065-dont_explicity_link_libc.patch"
        touch .no_explicit_link
    fi
    if [ ! -f ".fixed_regression_cxx" ]; then
        patch -Np0 -i "$DIR/Patches/gnu/gcc/0055-Fix_regresion_nostdinc_makefile_in.patch"
        patch -Np0 -i "$DIR/Patches/gnu/gcc/0056-Fix_regresion_nostdinc_makefile_am.patch"
        touch .fixed_regression_cxx
    fi
popd

BUILD_LOCATION="$BUILD_DIR/toolchain/gcc"

mkdir -p $BUILD_LOCATION

pushd "$BUILD_LOCATION"
    if [ ! -f ".built_$PHASE" ]; then
        CFLAGS='-g0 -O0' \
        CXXFLAGS=$CFLAGS \
        $SOURCES_DIR/$GCC_NAME/configure \
        --target=${TARGET_TRIPLE} \
        --build=${BUILDER} \
        --host=${TARGET_TRIPLE} \
        --prefix=$TOOLS_LOCATION \
        --with-local-prefix=$TOOLS_LOCATION \
        --with-native-system-header-dir=$TOOLS_LOCATION/include \
        --enable-languages=c,c++ \
        --disable-libstdcxx-pch \
        --disable-multilib \
        --disable-bootstrap \
        --disable-libgomp \
        --disable-libquadmath \
        --disable-libssp \
        --disable-libvtv \
        --disable-symvers \
        --disable-libitm \
        --disable-libsanitizer

        PATH=/bin:/usr/bin:$CROSS_LOCATION/bin:$TOOLS_LOCATION/bin make

        make install
        ln -sfv gcc $TOOLS_LOCATION/bin/cc
        touch .built_$PHASE
    fi
popd
