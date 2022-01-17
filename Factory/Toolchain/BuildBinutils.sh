

CC="${TARGET_TRIPLE}-gcc"
CXX="${TARGET_TRIPLE}-g++"
AR="${TARGET_TRIPLE}-ar"
AS="${TARGET_TRIPLE}-as"
RANLIB="${TARGET_TRIPLE}-ranlib"
LD="${TARGET_TRIPLE}-ld"
STRIP="${TARGET_TRIPLE}-strip"

case $(uname -m) in
        x86_64) ln -sv lib $TOOLS_LOCATION/lib64 ;;
esac

BUILD_LOCATION="$BUILD_DIR/toolchain/binutils"

mkdir -p $BUILD_LOCATION

pushd "$BUILD_LOCATION"
    if [ ! -f ".built_$PHASE" ]; then
        $SOURCES_DIR/$BINUTILS_NAME/configure --prefix=${TOOLS_LOCATION} \
        --with-lib-path=${TOOLS_LOCATION/lib} \
        --build=${BUILDER} \
        --host=${TARGET_TRIPLE} \
        --target=${TARGET_TRIPLE} \
        --disable-nls \
        --disable-werror \
        --with-sysroot

        make

        make install

        make -C ld clean
        make -C ld LIB_PATH=/usr/lib:/lib:/Core/Libraries
        cp -v ld/ld-new $TOOLS_LOCATION/bin
        touch .built_$PHASE
    fi
popd
