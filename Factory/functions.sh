#!/bin/bash
# Thank you stackoverflow: https://stackoverflow.com/a/18139746

function download_package {
    pushd $1
        sha256=""
        if [ -e $2 ]; then
            sha256="$(sha256sum $2 | cut -f1 -d' ')"
        fi

        if [ "$sha256" != $3 ]; then
            rm -rf $2
            curl -LO $4
        else
            echo "Skipped downloading $5"
        fi
    popd
}

function extract_package {
    pushd $1
        extension="$(echo "$2" | awk -F'[.]' '{print $NF}')"
        if [ "$extension" == "xz" ]; then
            xz -cd "$3/$2" | tar -xvf - 
        elif [ "$extension" == "gz" ]; then
            gzip -cd "$3/$2" | tar -xvf -
        else
            echo "Unkown extension, possibly corrupt, stopping"
            exit 1
        fi
    popd
}