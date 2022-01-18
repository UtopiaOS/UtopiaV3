#!/bin/bash
# 
# UtopiaBuilder -> Deploy Utopia stage 1 crosstools and toolchain
#
# Setup env script
#
#

CROSS_ENV_SIGNATURE=${CROSS_ENV_SIGNATURE:-"UNDEFINED"}
if [ $CROSS_ENV_SIGNATURE = "CROSS_ENV_SIGNATURE" ]; then
    echo "Cannot set build environment!"
    echo "Build environment already set!"
    exit 1
fi

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PARENT=$(realpath $DIR/..)
CONFIG="$DIR/config.json"

VALID_VENDORS=("trip")
VALID_ARCHITECTURES=("x86_64")

if [ ! -f "$CONFIG" ]; then
    echo "No configuration file has been detected! Please write your own or copy the example over"
    echo "$(realpath $DIR/../Meta/Build/example_config.json) as config.json"
    exit 1
fi

# TODO: Verify that vendors are valid
CONFIG_VENDOR=$(cat $CONFIG | jq -r ".vendor")

# TODO: Verify that the architecture is valid
CONFIG_ARCHITECTURE=$(cat $CONFIG | jq -r ".targets")

ALWAYS_REEXTRACT=$( [[ $(cat $CONFIG | jq -r ".reextract") == "true" ]] &&  printf %d "1" || printf %d "0")

echo $ALWAYS_REEXTRACT

# TODO: Make a function that converts targets architecture to CPU syntax
CPU="x86-64"

BUILDER=""$(echo $MACHTYPE | \
    sed "s/$(echo $MACHTYPE | cut -d- -f2)/cross/")""

TARGET_TRIPLE="$CONFIG_ARCHITECTURE-$CONFIG_VENDOR-linux-utopia"

CROSS_LOCATION="$PARENT/Build/CrossTools"
TOOLS_LOCATION="$PARENT/Build/Tools"
OPERATION_CREATION="$PARENT/Build/Creation"
TARBALLS_DIR="$OPERATION_CREATION/tarballs"
SOURCES_DIR="$OPERATION_CREATION/sources"
BUILD_DIR=$"$OPERATION_CREATION/build"
ROOT=$(realpath $DIR/../)
NAMES=( "KERNEL_HEADERS" "BINUTILS" "MPFR" "GMP" "MPC" "GCC" )

PHASE="crosstools"


export PATH=$CROSS_LOCATION/bin:/bin:/usr/bin:$TOOLS_LOCATION/bin

NPROC="nproc"

if [ -z "$MAKEJOBS" ]; then
    export MAKEJOBS=$($NPROC)
fi

echo "Making directories..."
mkdir -p $CROSS_LOCATION
mkdir -p $TOOLS_LOCATION
mkdir -p $OPERATION_CREATION/{tarballs,sources,build}

source $DIR/sources.sh
source $DIR/functions.sh

for name in "${NAMES[@]}"
do
    lpkg="${name}_PKG"
    lsha256sum="${name}_SHA256SUM"
    lurl="${name}_URL"
    lname="${name}_NAME"
    download_package $TARBALLS_DIR ${!lpkg} ${!lsha256sum} ${!lurl} ${!lname}
    if [ ! -d $SOURCES_DIR/${!lname} ] || [ "$ALWAYS_REEXTRACT" == "1" ]; then
        extract_package $SOURCES_DIR ${!lpkg} $TARBALLS_DIR
    else
        echo "${!lpkg} has already been extracted"
    fi
done


source $DIR/CrossTools/BuildKernelHeaders.sh
source $DIR/CrossTools/BuildBinutils.sh
source $DIR/CrossTools/BuildGCCPassOne.sh
source $DIR/CrossTools/BuildlibSystem.sh
source $DIR/CrossTools/BuildGCCPassTwo.sh

PHASE="toolchain"

source $DIR/Toolchain/BuildlibSystem.sh
source $DIR/Toolchain/AdjustToolchain.sh
source $DIR/Toolchain/BuildBinutils.sh
source $DIR/Toolchain/BuildGCC.sh