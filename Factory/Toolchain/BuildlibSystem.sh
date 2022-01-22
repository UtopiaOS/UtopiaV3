#!/usr/bin/env bash
BUILD_LOCATION="$BUILD_DIR/toolchain/libSystem"

mkdir -p "$BUILD_LOCATION"

pushd "$BUILD_LOCATION"
    if [ ! -f ".built_$PHASE" ]; then
        $SOURCES_DIR/libSystem/configure \
        CROSS_COMPILE=${TARGET_TRIPLE}- \
        --prefix=/ \
        --target=${TARGET_TRIPLE}

        make

        DESTDIR=$TOOLS_LOCATION make install
    fi
popd

pushd "$TOOLS_LOCATION"
    rm -fv $TOOLS_LOCATION/Core/Binaries/linker${CONFIG_ARCHITECTURE}
    ln -sfv ../../lib/libSystem.so $TOOLS_LOCATION/Core/Binaries/linker${CONFIG_ARCHITECTURE}

    mkdir -pv $TOOLS_LOCATION/etc
    if [ -f $TOOLS_LOCATION/etc/ld-musl-$CONFIG_ARCHITECTURE.path ]; then
        rm -rf $TOOLS_LOCATION/etc/ld-musl-$CONFIG_ARCHITECTURE.path
    fi

    touch $TOOLS_LOCATION/etc/ld-musl-$CONFIG_ARCHITECTURE.path
    echo "$TOOLS_LOCATION/lib" >> $TOOLS_LOCATION/etc/ld-musl-$CONFIG_ARCHITECTURE.path

popd