cmake_minimum_required (VERSION 2.6)
project (CosyGui)

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        add_definitions (-O3 -Wall -Wextra -D__STDC_LIMIT_MACROS)
endif()


set(aux_dir ../../../../auxiliary)


include_directories (../../../)
include_directories (../../../redshift/include)
include_directories (${aux_dir})




# -- openmp package -----------------------------------------------------------
find_package (OpenMP REQUIRED)
link_libraries(gomp)
add_definitions (${OpenMP_CXX_FLAGS})




# -- packages ---------------------------------------------------------------
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

### QT ###
find_package(Qt4 REQUIRED)

# Include dirs.
include_directories (
        ./                   # for generated stuff
        ../                  # project source folder
        ${QT_INCLUDE_DIR}    # self-explaining
)

# Used Qt Modules.
set(QT_USE_QTWEBKIT TRUE)

include(${QT_USE_FILE})



### BOOST ###
find_package(Boost COMPONENTS program_options system filesystem REQUIRED)
link_libraries(
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
)


### OpenEXR ###
find_package (OpenEXR REQUIRED )
link_libraries (${OPENEXR_LIBRARIES})
include_directories (${OPENEXR_INCLUDE_PATHS})

### OpenMP ###
find_package (OpenMP REQUIRED)
link_libraries(gomp)   # TODO: is there a portable one?


### LibNoise ###
find_package (Noise REQUIRED)
link_libraries(${NOISE_LIBRARY})
include_directories(${NOISE_INCLUDE_DIR})

### SDL ###
find_package (SDL REQUIRED )
find_package (SDL_image REQUIRED )
if (NOT SDL_FOUND )
   message ( FATAL_ERROR "SDL not found!" )
endif (NOT SDL_FOUND )

link_libraries (
   ${SDL_LIBRARY}
   ${SDLIMAGE_LIBRARY} # if using SDL_image, obviously
   SDLmain # Sadly not included in SDL_LIBRARY variable
)
include_directories(${SDL_INCLUDE_DIR})



# -- subs ---------------------------------------------------------------------
set (widgets_dir ../../widgets)

MACRO(priinclude subdir)
        set (old_pwd ${pwd})
        set (pwd "${subdir}")
        include("${subdir}/include.cmake")
        set (pwd ${old_pwd})
ENDMACRO(priinclude)

priinclude("../../../../upstream/QtSolutions")

priinclude("${widgets_dir}/AppNavigation")
priinclude("${widgets_dir}/TextDialog")
priinclude("${widgets_dir}/LazyQuadtreeLodPlot")
priinclude("${widgets_dir}/SpectralColorPicker")
priinclude("${widgets_dir}/TristimulusColorPicker")
priinclude("${widgets_dir}/ColorPicker")
priinclude("${widgets_dir}/RenderWidget")
priinclude("${widgets_dir}/RenderWindow")
priinclude("${widgets_dir}/GraphicalNavigation")
priinclude("${widgets_dir}/QtSunSkyEditor")
priinclude("${widgets_dir}/QuatschPresetEditor")
priinclude("${widgets_dir}/QuatschPreview")
priinclude("${widgets_dir}/QuatschSourceEditor")
priinclude("${widgets_dir}/Picohelp")
priinclude("${widgets_dir}/Preview")

priinclude("../../Repository")
priinclude("../../Repository/repository")
priinclude("../../Repository/fragment")
priinclude("../../Repository/fragment/preset")

priinclude("../../../cosyscene")
priinclude("../../../redshift/")
priinclude("${aux_dir}/xyto/")
priinclude("${aux_dir}/quatsch-preprocessor/")



# -- sources ------------------------------------------------------------------
set (cosygui_src_dir ..)
set (sources ${sources}
        ${cosygui_src_dir}/camerawidget.cc
        ${cosygui_src_dir}/filmsettingsandcamera.cc
        ${cosygui_src_dir}/filmsettingswindow.cc
        ${cosygui_src_dir}/main.cc
        ${cosygui_src_dir}/mainwindow.cc
        ${cosygui_src_dir}/material-ui.cc
        ${cosygui_src_dir}/navigationwindow.cc
        ${cosygui_src_dir}/rendersettingswindow.cc
        ${cosygui_src_dir}/startpopup.cc
        ${cosygui_src_dir}/stashframe.cc
        ${cosygui_src_dir}/stashview.cc
        ${cosygui_src_dir}/stylesheetliveeditor.cc
        ${cosygui_src_dir}/sunskywindow.cc
        ${cosygui_src_dir}/terrainfitting.cc
        ${cosygui_src_dir}/terrainformation.cc
        ${cosygui_src_dir}/terrainwindow.cc
        ${cosygui_src_dir}/twinrendersettingswindow.cc
        ${cosygui_src_dir}/waterfitting.cc
        ${cosygui_src_dir}/waterwindow.cc
)



# -- ui -----------------------------------------------------------------------
set(forms ${forms}
        ${cosygui_src_dir}/camerawidget.ui
        ${cosygui_src_dir}/filmsettingsandcamera.ui
        ${cosygui_src_dir}/filmsettingswindow.ui
        ${cosygui_src_dir}/mainwindow.ui
        ${cosygui_src_dir}/material-ui.ui
        ${cosygui_src_dir}/navigationwindow.ui
        ${cosygui_src_dir}/rendersettingswindow.ui
        ${cosygui_src_dir}/startpopup.ui
        ${cosygui_src_dir}/stashframe.ui
        ${cosygui_src_dir}/stashview.ui
        ${cosygui_src_dir}/stylesheetliveeditor.ui
        ${cosygui_src_dir}/sunskywindow.ui
        ${cosygui_src_dir}/terrainfitting.ui
        ${cosygui_src_dir}/terrainformation.ui
        ${cosygui_src_dir}/terrainwindow.ui
        ${cosygui_src_dir}/twinrendersettingswindow.ui
        ${cosygui_src_dir}/waterfitting.ui
        ${cosygui_src_dir}/waterwindow.ui
)

qt4_add_resources(resources "${cosygui_src_dir}/resources.qrc")
set (sources ${sources} ${resources})

qt4_automoc (${sources})
qt4_wrap_ui (forms_h ${forms})
link_libraries (${QT_LIBRARIES})





# -- targets ------------------------------------------------------------------
add_executable (CosyGui  ${sources} ${forms_h})
