#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PARENT="$(realpath $DIR/..)"

source "$PARENT/3rd/bash_framework/lib/oo-bootstrap.sh"

import util/type
import util/log

namespace run

Log::AddOutput run DEBUG

pushd () {
	command pushd "$@" &> /dev/null
}

popd () {
	command popd "$@" &> /dev/null
}

AVAILABLE_CORES="$(nproc)"
# TODO: Stop hardcoding the arch
TARGET_SYSROOT_DIRECTORY="$PARENT/Build/x86_64/Root"

# Check that the "dev kit" is installed
needed_tools=("bc" "flex" "bison" "gawk" "autoconf" "cc" "cpp" "git" "wget" "curl" "clang" "clang++" "ninja" "make" "cmake")
for cmd in ${needed_tools[@]}; do
	Log "Checking for ${cmd}..."
	if ! command -v ${cmd} &> /dev/null
	then
		echo "The command ${cmd} isn't installed, please install it with your distros package manager"
		exit 1
	fi
done

# Find some general include files that we are going to need
declare -A needed_headers=( ["libiberty"]="libiberty/ansidecl.h" ["libncurses"]="curses.h" ["libelf"]="gelf.h" ["libudev"]="libudev.h" ["libpci"]="pci/pci.h")
for library in "${!needed_headers[@]}"; do
	Log "Looking for ${library}..."
	echo "#include <${needed_headers[$library]}>" | cpp -H -o /dev/null 2>&1 &> /dev/null
	if [ $? -ne 0 ]
	then
		echo "The library ${library} isn't installed as it doesn't provide a header file to include. Please install it"
		exit 1
	fi
done


if [ ! -e $PARENT/Build ]; then
    Log "Build directory doesn't exist, creating it..."
    mkdir -p $PARENT/Build
fi


if [ ! -e $PARENT/Resources ]; then
    Log "Resources directory doesn't exist, creating it..."
    mkdir -p $PARENT/Resources
fi

if [ ! -e $PARENT/Resources/linux ];  then
    if [ ! -e $PARENT/Resources/master.zip ]; then
        Log "Cloning the latest Utopia Linux kernel..."
        wget https://github.com/UtopiaOS/linux/archive/master.zip -O $PARENT/Resources/master.zip
    else
        Log "Found cached Utopia Linux kernel..."
    fi

    if [ -e $PARENT/Resources/linux ]; then
        Log "Kernel has already been extracted..."
    else
        pushd $PARENT/Resources
            Log "Extracting the kernel..."
            unzip master.zip
            # Delete the kernel zip file
            rm master.zip
        popd
        # TODO: Stop hardcoding the archive name
        mv $PARENT/Resources/linux-master $PARENT/Resources/linux
    fi
fi

if [ ! -e $PARENT/Build/kernel/x86_64/vmlinux ]; then
	# Pushd into the kernel directory
	pushd $PARENT/Resources/linux/src
    		Log "Building the kernel..."
    		mkdir -p $PARENT/Build/kernel
    		# TODO: Stop hardcoding the architecture
    		mkdir -p $PARENT/Build/kernel/x86_64
    		Log "Copying kernel configuration"
    		# TODO: Make the configuration... well, configurable
    		cp $PARENT/Resources/linux/Configs/BasicUtopia $PARENT/Build/kernel/x86_64/.config
    		WORKSPACE="$PARENT/Build/kernel/x86_64"
    		make O=$WORKSPACE -j$(nproc)
	popd
else
	Log "Found cached kernel build..."
fi

BUSYBOX_VERSION="1.35.0"
BUSYBOX_NAME="busybox-$BUSYBOX_VERSION"
BUSYBOX_PKG="$BUSYBOX_NAME.tar.bz2"
BUSYBOX_URL="https://busybox.net/downloads/$BUSYBOX_PKG"

# In the future, we don't use busybox :p
if [ ! -e $PARENT/Resources/$BUSYBOX_PKG ]; then
    Log "Downloading busybox..."
    wget $BUSYBOX_URL -O $PARENT/Resources/$BUSYBOX_PKG
else
    Log "Found cached busybox..."
fi

# If on the first run the resources were already renamed to busybox, the second run will result in duplicate source code
if [ ! -e $PARENT/Resources/$BUSYBOX_NAME ] && [ ! -e $PARENT/Resources/busybox ]; then
    Log "Extracting busybox..."
    tar -xjf $PARENT/Resources/$BUSYBOX_PKG -C $PARENT/Resources
    mv $PARENT/Resources/$BUSYBOX_NAME $PARENT/Resources/busybox
fi

if [ ! -e $PARENT/Build/busybox/x86_64/_install ]; then
	# We need glibc-static-devel on OpenSuse...
	pushd $PARENT/Resources/busybox
    		Log "Making necessary directories..."
    		mkdir -p $PARENT/Build/busybox/x86_64
    		Log "Copying busybox configuration"
    		cp $PARENT/Meta/configs/BusyBoxDefault $PARENT/Build/busybox/x86_64/.config
    		WORKSPACE="$PARENT/Build/busybox/x86_64"
		make O=$WORKSPACE -j$(nproc)
		LDFLAGS="--static" make O=$WORKSPACE -j$(nproc) install
	popd
else
	Log "Found cached busybox build..."
fi

# Make the legacy directory structure
if [ ! -e $TARGET_SYSROOT_DIRECTORY ]; then
    mkdir -p $TARGET_SYSROOT_DIRECTORY
    mkdir -p $TARGET_SYSROOT_DIRECTORY/{bin,sbin,etc,proc,sys,usr/{bin,sbin}}
fi

# Copy the resulting busybox binaries to the Root
Log "Copying busybox binaries to the Root..."
cp -nav $PARENT/Build/busybox/x86_64/_install/* $TARGET_SYSROOT_DIRECTORY

if [ ! -e $TARGET_SYSROOT_DIRECTORY/init ]; then
	# Make our temporary "init"
	cat << EOT >> $TARGET_SYSROOT_DIRECTORY/init
	#!/bin/sh

	mount -t proc none /proc
	mount -t sysfs none /sys

	echo -e "Welcome to Utopia\n"
	echo -e "Is not recommend to distribute this\n"

	exec /bin/sh
	EOT

	chmod +x $TARGET_SYSROOT_DIRECTORY/init
fi

Log "Building Utopia"
if [ ! -e $PARENT/Build/Utopia ]; then
	mkdir $PARENT/Build/Utopia
fi

cmake -GNinja -S "$PARENT/Meta/CMake/Quetza" -B $PARENT/Build/Utopia
CMAKE_BUILD_PARALLEL_LEVEL="${AVAILABLE_CORES}" cmake --build $PARENT/Build/Utopia


pushd $TARGET_SYSROOT_DIRECTORY
	Log "Compressing initramfs"
	# Create and compress our initramfs
	find . -print0 | cpio --null -ov --format=newc | gzip -9 > $PARENT/Build/rootfs.cpio.gz
popd
