

pushd $SOURCES_DIR/$KERNEL_HEADERS_NAME
    make mrproper
    ARCH=${CONFIG_ARCHITECTURE} make headers

    cp -rv usr/include/* $TOOLS_LOCATION/include

    find $TOOLS_LOCATION/include \( -name '.*' -o -name '.*.cmd' \) -exec rm -vf {} \;
    rm -v $TOOLS_LOCATION/include/Makefile
popd