export PATH=/usr/i586-mingw32msvc/SDL-1.2.13/bin:$PATH
export PATH=/usr/i586-mingw32msvc/bin:$PATH
alias g++='i586-mingw32msvc-g++'

#sdl-config --static-libs
g++ \
        -DWINVER=0x0400 \
        -D__WIN95__ \
        -D__GNUWIN32__ \
        -DSTRICT \
        -DHAVE_W32API_H \
        -D__WXMSW__ \
        -D__WINDOWS__ \
        -DSTANDALONE \
        \
        -Wall -Wextra \
        -I/usr/i586-mingw32msvc/include \
        -I/usr/i586-mingw32msvc/SDL-1.2.13/include \
        -I./include \
        `sdl-config --cflags` \
        src/main.cc \
        \
        -L/usr/i586-mingw32msvc/lib\
        -L/usr/i586-mingw32msvc/SDL-1.2.13/lib \
        -lstdc++ \
        -lgcc \
        -lodbc32 \
        -lwsock32 \
        -lwinspool \
        -lwinmm \
        -lshell32 \
        -lcomctl32 \
        -lctl3d32 \
        -lodbc32 \
        -ladvapi32 \
        -lodbc32 \
        -lwsock32 \
        -lopengl32 \
        -lglu32 \
        -lole32 \
        -loleaut32 \
        -luuid \
        -static `sdl-config --static-libs` \
        -dynamic -mwindows
        
#-Wl,-Bstatic `/usr/i586-mingw32msvc/SDL-1.2.13/bin/sdl-config --static-libs` -Wl,-Bdynamic -mwindows
