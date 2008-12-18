
cmake_minimum_required (VERSION 2.6)
project (picogen-wx) 
set (project_name picogen-wx)

#########################################################################################
## picogen source files #################################################################
set (CXX_SOURCEFILE_FOLDER ../src)
set (FILES )
macro (add_cxx_file filename)
    set (FILES ${FILES} ${CXX_SOURCEFILE_FOLDER}/${filename})
endmacro (add_cxx_file)
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #


#########################################################################################
## #defines #############################################################################
add_definitions ()
#########################################################################################


#########################################################################################
##  #####################################################################################

    
    #find_library (boost_serialization_lib libboost_serialization.a ../lib/)
    #find_library (boost_serialization_lib boost_serialization ../lib/)
    find_library (boost_serialization_lib libboost_serialization ../lib/ c:/mingw/lib)
    #message ("boost-serialization: ${boost_serialization_lib}")
    link_libraries(${boost_serialization_lib})
    
    #find_library (boost_regex_lib libboost_regex ../lib/)
    #message ("boost-regex_lib: ${boost_regex_lib}")
    #link_libraries(${boost_regex_lib})

    include_directories (BEFORE
		/c/boost/boost_1_37_0/
		/usr/include/boost.static/boost-1_37/
    )
    
    include_directories (BEFORE 
		../../quantum/include/
        ./ 
        ../include
        ../upstream/wx-gtk/
        ../upstream/wx-gtk/gtk2-unicode-release-static-2.6
        ../src 
        ../upstream/wxscintilla/src/
        ../upstream/wxscintilla/include/
        ../upstream/wxscintilla/src/scintilla/include/
        ../upstream/wxscintilla/src/scintilla/src/        
        ../upstream/wxscintilla/include/wx
        ../upstream/        
    )
    
    
    #link_libraries (/usr/lib/libboost_serialization-gcc41-mt-1_37.a)
    
    add_cxx_file (main.cc)
    add_cxx_file (MkheightmapWxDialog.cpp)
    add_cxx_file (auto/mkheightmap_wx_gui.cpp)
    
    add_definitions (-D__WX__=1 -DSCI_LEXER -DLINK_LEXERS )
        
    #########################################################################################
    #add_cxx_file (../../quantum/src/picossdf/picossdf.cc)
    #add_cxx_file (../../quantum/src/picosrc/misc/scripting/nano-c.cc)
    #add_cxx_file (../../quantum/src/picosrc/misc/scripting/picogen-c.cc)
    #add_cxx_file (../../quantum/src/picosrc/misc/functional.cc)
    #add_cxx_file (../../quantum/src/picosrc/misc/functional/LayeredNoise2d.cc)
    #add_cxx_file (../../quantum/src/picosrc/misc/functional/Layers.cc)
    #add_cxx_file (../../quantum/src/picosrc/misc/functional/Noise2d.cc)
    #########################################################################################
    
    #########################################################################################
    add_cxx_file (../upstream/wxscintilla/src/PlatWX.cpp)
    add_cxx_file (../upstream/wxscintilla/src/ScintillaWX.cpp)
    add_cxx_file (../upstream/wxscintilla/src/wxscintilla.cpp)
    #########################################################################################
    
    #########################################################################################
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/AutoComplete.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/CallTip.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/CellBuffer.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/CharClassify.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/ContractionState.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/Document.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/DocumentAccessor.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/Editor.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/Indicator.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/KeyMap.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/KeyWords.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexAda.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexAPDL.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexAsm.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexAsn1.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexAU3.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexAVE.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexBaan.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexBash.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexBasic.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexBullant.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexCaml.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexCLW.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexConf.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexCPP.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexCrontab.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexCsound.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexCSS.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexEiffel.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexErlang.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexEScript.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexFlagship.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexForth.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexFortran.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexGui4Cli.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexHaskell.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexHTML.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexInno.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexKix.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexLisp.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexLout.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexLua.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexMatlab.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexMetapost.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexMMIXAL.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexMPT.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexMSSQL.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexOpal.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexNsis.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexOthers.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexPascal.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexPB.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexPerl.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexPOV.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexPS.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexPython.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexRebol.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexRuby.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexScriptol.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexSmalltalk.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexSpecman.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexSpice.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexSQL.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexTADS3.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexTCL.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexTeX.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexVB.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexVerilog.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexVHDL.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LexYAML.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/LineMarker.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/PropSet.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/RESearch.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/ScintillaBase.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/Style.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/StyleContext.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/UniConversion.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/ViewStyle.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/WindowAccessor.cxx )
    add_cxx_file (../upstream/wxscintilla/src/scintilla/src/XPM.cxx)
    #########################################################################################

    add_executable (${project_name} ${FILES})    
    
    
    # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
    # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
    # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
    
    ####
    # Include wxWidgets macros
    #include (${wxWidgets_USE_FILE})
    #include (./UsewxWidgets.cmake)
    #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I../wx/wxGTK-2.6.4/lib/wx/upstream/gtk2-unicode-release-static-2.6 -I../wx/wxGTK-2.6.4/include -I../wx/wxGTK-2.6.4/contrib/include -DGTK_NO_CHECK_CASTS -D__WXGTK__ -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -DNO_GCC_PRAGMA")
    #include_directories( 
        #"../wx/wxGTK-2.6.4/lib/wx/upstream/gtk2-unicode-release-static-2.6" 
        #"../wx/wxGTK-2.6.4/include" 
        #"../wx/wxGTK-2.6.4/contrib/include"
    #)       
    
    #!!GTK?? set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGTK_NO_CHECK_CASTS -D__WXGTK__ -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -DNO_GCC_PRAGMA")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMSW_NO_CHECK_CASTS -D__WXMSW__ -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -DNO_GCC_PRAGMA")

    # link libraries to picogen-wx
    # d'oh
    #
	
	#wx-config --libs
    if (${UNIX})
        target_link_libraries(${project_name} "-pthread   ../upstream/lib/libwx_gtk2u-2.6.a -pthread -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lgdk_pixbuf-2.0 -lm -lpango-1.0 -lgobject-2.0 -lgmodule-2.0 -ldl -lgthread-2.0 -lrt -lglib-2.0 -lXinerama -lpng -lz -ljpeg -ltiff -lz -ldl -lm")
    elseif (${WIN32})
        target_link_libraries(${project_name} "-L/c/wxWidgets-2.6.3/lib   -Wl,--subsystem,windows -mwindows /c/wxWidgets-2.6.3/lib/libwx_msw-2.6.a -lwxregex-2.6 -lwxexpat-2.6 -lwxtiff-2.6 -lwxjpeg-2.6 -lwxpng-2.6 -lwxzlib-2.6 -lrpcrt4 -loleaut32 -lole32 -luuid -lwinspool -lwinmm -lshell32 -lcomctl32 -lcomdlg32 -lctl3d32 -ladvapi32 -lwsock32 -lgdi32")
    else (${UNIX})
        message ("unknown target")
    endif(${UNIX})
    ####    

    #message ("§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§")
    #message ("`${CMAKE_WX_CONFIG} --cxxflags --static` ")
    #message ("cxx-flags: ${CMAKE_CXX_FLAGS}")
    #message ("wx-libraries: ${wxWidgets_LIBRARIES}")
    #message ("flags: ${wxWidgets_CXX_FLAGS}")
    #set (wxWidgets_CXX_FLAGS )    
    #message ("flags: ${wxWidgets_CXX_FLAGS}")
    #message ("definitions: ${wxWidgets_DEFINITIONS}")
    #message ("wx-include-dirs: ${wxWidgets_INCLUDE_DIRS}")
    #message ("wx-use-file: ${wxWidgets_USE_FILE}")
    #message ("§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§")

#########################################################################################
