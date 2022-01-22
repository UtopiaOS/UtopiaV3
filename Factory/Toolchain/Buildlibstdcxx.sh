

pushd $SOURCES_DIR/$GCC_NAME
    if [ ! -f .patched_find_on_host ]; then
        patch -Np1 -i $DIR/Patches/gnu/gcc/0057-fix_find_fenv_on_host.patch
        touch .patched_find_on_host
    fi
popd

BUILD_LOCATION="$BUILD_DIR/toolchain/libstdcxx"

mkdir -p $BUILD_LOCATION

pushd "$BUILD_LOCATION"
    if [ ! -f ".built_$PHASE" ]; then
        $SOURCES_DIR/$GCC_NAME/libstdc++-v3/configure \
        --target=${TARGET_TRIPLE} \
        --build=${BUILDER} \
        --host=${TARGET_TRIPLE} \
        --prefix=${TOOLS_LOCATION} \
        --disable-multilib \
        --disable-nls \
        --disable-libstdcxx-threads \
        --disable-libstdcxx-pch \
        --with-gxx-include-dir=${TOOLS_LOCATION}/${TARGET_TRIPLE}/include/c++/11.2.0

        make 

        make install
    fi

popd