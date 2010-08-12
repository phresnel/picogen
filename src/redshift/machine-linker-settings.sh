# a kludge to add machine specific linker options
if [ -e /c/mingw ]; then
	echo -Wl,--subsystem,console
fi

cd ../scripts > /dev/null
if  ./gcc-version-at-least.sh 4 5 ; then
        echo -enable-stdcall-fixup  \
         -Wl,--enable-auto-import,--enable-runtime-pseudo-reloc-v2,--no-keep-memory,--reduce-memory-overheads
fi
cd -  > /dev/null