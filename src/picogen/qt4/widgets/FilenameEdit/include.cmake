if (not ${FILENAMEEDIT})
        set (FILENAMEEDIT true)
        set (sources ${sources}
                ${pwd}/filenameedit.cc
        )
        set (forms ${forms}
                ${pwd}/filenameedit.ui
        )
        include_directories ("${pwd}/")
endif()