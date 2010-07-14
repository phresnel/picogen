# a kludge to add machine specific linker options
if [ -e /c/mingw ]; then
	echo -Wl,--subsystem,console
else
	echo
fi