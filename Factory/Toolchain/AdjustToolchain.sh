export SPECFILE=`dirname $(${TARGET_TRIPLE}-gcc -print-libgcc-file-name)`/specs
${TARGET_TRIPLE}-gcc -dumpspecs > specs

case $(uname -m) in
    x86_64) sed -i "s|\/lib\/ld-musl-x86_64.so.1|${TOOLS_LOCATION}\/lib\/ld-musl-x86_64.so.1|g" specs
            ;;
esac


mv -v specs ${SPECFILE}
unset SPECFILE

# test everything is ok

echo 'int main(){}' > dummy.c
${TARGET_TRIPLE}-gcc dummy.c
interpreter_path=$(${TARGET_TRIPLE}-readelf -l a.out | grep Requesting | awk '{split($0,a," "); print a[4]}' | sed 's/]//')

expected=$TOOLS_LOCATION/lib/ld-musl-${CONFIG_ARCHITECTURE}.so.1

if [ $interpreter_path != $expected ]; then
    echo "Could not verify that the interpreter path got path critical"
    exit 1
fi

rm -v a.out dummy.c