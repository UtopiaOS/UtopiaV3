SPECFILE=`dirname $(${TARGET_TRIPLE}-gcc -print-libgcc-file-name)`/specs &&
${TARGET_TRIPLE}-gcc -dumpspecs > tempspecfile

case $(uname -m) in
    x86_64) sed -i "s|\/Core\/Binaries\/linker${CONFIG_ARCHITECTURE}|${TOOLS_LOCATION}\/Core\/Binaries\/linker${CONFIG_ARCHITECTURE}|g" tempspecfile
            ;;
esac

mv -vf tempspecfile $SPECFILE &&
unset SPECFILE

echo 'int main(){}' > dummy.c
${TARGET_TRIPLE}-gcc dummy.c
interpreter_path=$(${TARGET_TRIPLE}-readelf -l a.out | grep Requesting | awk '{split($0,a," "); print a[4]}' | sed 's/]//')

expected=$TOOLS_LOCATION/Core/Binaries/linker${CONFIG_ARCHITECTURE}

if [ $interpreter_path != $expected ]; then
    echo "Could not verify that the interpreter path got path critical"
    exit 1
fi

GCC_INCLUDEDIR=`dirname $(${TARGET_TRIPLE}-gcc -print-libgcc-file-name)`/include &&
find ${GCC_INCLUDEDIR}/* -maxdepth 0 -xtype d -exec rm -rvf '{}' \; &&
rm -vf `grep -l "DO NOT EDIT THIS FILE" ${GCC_INCLUDEDIR}/*` &&
unset GCC_INCLUDEDIR


rm -v a.out dummy.c