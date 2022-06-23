#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PARENT="$(realpath $DIR/..)"

source "$PARENT/3rd/bash_framework/lib/oo-bootstrap.sh"

import util/type
import util/log

namespace run

Log::AddOutput run DEBUG

if [ ! -e $PARENT/Build ]; then
    Log "Build directory doesn't exist, creating it..."
    mkdir $PARENT/Build
fi


if [ ! -e $PARENT/Resources ]; then
    Log "Resources directory doesn't exist, creating it..."
    mkdir $PARENT/Resources
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

if [ ! -e $PARENT/Resources/$BUSYBOX_NAME ]; then
    Log "Extracting busybox..."
    tar -xjf $PARENT/Resources/$BUSYBOX_PKG -C $PARENT/Resources
    mv $PARENT/Resources/$BUSYBOX_NAME $PARENT/Resources/busybox
fi