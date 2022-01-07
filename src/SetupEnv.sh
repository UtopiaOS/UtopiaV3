#!/bin/bash

if [ "$(uname)" != "Linux" ]
then
    echo "Currently Utopia can only be built on Linux!"
    exit 1
fi

POSITIONAL_ARGUMENTS=()

setupenv__build-toolchain() {
    echo "hello world!"
}


setupenv() {
    local cmdname=$1; shift
    echo $cmdname
    if type "setupenv__$cmdname" >/dev/null 2>&1; then
        "setupenv__$cmdname" "$@"
    else
        exit 1
    fi
}


get_top_dir() {
    git rev-parse --show-toplevel
}

init_env() {
    if [ ! -d "$UTOPIAOS_SOURCE_DIR" ]; then
        UTOPIAOS_SOURCE_DIR="$(get_top_dir)"
        export UTOPIAOS_SOURCE_DIR
    fi

    BUILD_DIR="$UTOPIAOS_SOURCE_DIR/Build"
}

# make sure we actually *did* get passed a valid function name
if declare -f "$1" >/dev/null 2>&1; then
  # invoke that function, passing arguments through
  "$@" # same as "$1" "$2" "$3" ... for full argument list
  setupenv
else
  echo "Function $1 not recognized" >&2
  exit 1
fi


