# a kludge to find boost libs properly and keep this away from the makefile.
# and also to keep the msys installation minimal (/usr/lib points to /msys/1.0)
if [ -e /usr/lib/libboost_program_options.a ] ; then 
	echo /usr/lib/libboost_program_options.a \
		 /usr/lib/libboost_filesystem.a  \
		 /usr/lib/libboost_system.a 
else
	echo /c/mingw/lib/libboost_program_options.a \
		 /c/mingw/lib/libboost_filesystem.a \
		 /c/mingw/lib/libboost_system.a
fi
