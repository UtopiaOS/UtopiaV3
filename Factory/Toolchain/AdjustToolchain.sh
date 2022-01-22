export SPECFILE=`dirname $(${TARGET_TRIPLE}-gcc -print-libgcc-file-name)`/specs
${TARGET_TRIPLE}-gcc -dumpspecs > specs

case $(uname -m) in
    x86_64) sed -i "s|\/Core/Binaries\/linkerx86_64|${TOOLS_LOCATION}\/Core\/Binaries\/linkerx86_64|g" specs
            ;;
esac


mv -v specs ${SPECFILE}
unset SPECFILE

# test everything is ok

echo 'int main(){}' > dummy.c
${TARGET_TRIPLE}-gcc dummy.c
interpreter_path=$(${TARGET_TRIPLE}-readelf -l a.out | grep Requesting | awk '{split($0,a," "); print a[4]}' | sed 's/]//')

expected=$TOOLS_LOCATION/Core/Binaries/linker${CONFIG_ARCHITECTURE}

if [ $interpreter_path != $expected ]; then
    echo "Could not verify that the interpreter path got path critical"
    exit 1
fi

rm -v a.out dummy.c