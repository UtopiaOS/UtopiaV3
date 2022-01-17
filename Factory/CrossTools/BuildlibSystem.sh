#!/usr/bin/env bash

SHA256SUM="sha256sum"
REALPATH="realpath"
MAKE="make"
NPROC="nproc"
INSTALL="install"
SED="sed"

if [ -z "$MAKEJOBS" ]; then
    MAKEJOBS=$($NPROC)
fi

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


# == ACQUIRE SOURCE ==

# In reality we just cp the files from Core/Libraries/libSystem

cp -vr $ROOT/Core/Libraries/libSystem $SOURCES_DIR/libSystem

# == BUILD ==

mkdir -p $BUILD_DIR/libSystem

pushd "$BUILD_DIR/libSystem"
    if [ ! -f ".built_$PHASE" ]; then
        $SOURCES_DIR/libSystem/configure CROSS_COMPILE=${TARGET_TRIPLE}- \
        --prefix=/ \
        --target=${TARGET_TRIPLE}

        make
        DESTDIR=$CROSS_LOCATION make install
        touch .built_$PHASE
    else
        echo "libSystem already built"
    fi
popd

# == FIX BROKEN LINKS ==

pushd "$CROSS_LOCATION"
    if [ ! -f ".cleaned_$PHASE" ]; then
        mkdir -pv usr
        ln -sfv ../include ./usr/include

        rm -vf lib/ld-musl-${CONFIG_ARCHITECTURE}.so.1
        ln -sfv libSystem.so lib/ld-musl-${CONFIG_ARCHITECTURE}.so.1
        # For legacy applications and portability
        ln -sfv libSystem.so lib/libc.so

        ln -sfv ../lib/ld-musl-$CONFIG_ARCHITECTURE.so.1 bin/ldd

        mkdir -pv etc
        if [ -f $CROSS_LOCATION/etc/ld-musl-$CONFIG_ARCHITECTURE.path ]; then
            rm -rf $CROSS_LOCATION/etc/ld-musl-$CONFIG_ARCHITECTURE.path
        fi
        # cat didnt work very well here...
        touch $CROSS_LOCATION/etc/ld-musl-$CONFIG_ARCHITECTURE.path
        echo "$CROSS_LOCATION/lib" >> $CROSS_LOCATION/etc/ld-musl-$CONFIG_ARCHITECTURE.path
        echo "$TOOLS_LOCATION/lib" >> $CROSS_LOCATION/etc/ld-musl-$CONFIG_ARCHITECTURE.path

        touch .cleaned_$PHASE
    else
        echo "libSystem broken links have already been fixed"
    fi
popd