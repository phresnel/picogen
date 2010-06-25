#
# TODO:
#   - should wget QtSolutions
#   - should gunzip QtSolutions
#   - doesn't clean up upstream

# that's a bit hacky, sorry.
SHELL := /bin/bash
QMAKE := $(shell if [ -f "c:/Qt/2010.02.1/qt/bin/qmake" ] ; then echo "c:/Qt/2010.02.1/qt/bin/qmake" ; else echo "qmake"; fi)
MAKE  := $(shell if [ -f "c:/mingw/bin/mingw32-make" ] ; then echo "c:/mingw/bin/mingw32-make" ; else echo "make"; fi)


.PHONY: all
all: picogen-lib  qt-frontend  strip-binaries

.PHONY: clean
clean: qt-clean  picogen-lib-clean

.PHONY: distclean
distclean: qt-distclean  picogen-lib-distclean

###############################################################################
# core
###############################################################################
.PHONY: picogen-lib picogen-lib-clean picogen-lib-distclean

picogen-lib: directories
	@cd redshift ; \
	$(MAKE) lib

picogen-lib-clean:
	@cd redshift ; \
	$(MAKE) clean

picogen-lib-distclean:


#	@cd redshift ; \
#	$(MAKE) disintegrate


###############################################################################
# upstream
###############################################################################
.PHONY: upstream
upstream: qtsolutions

.PHONY: qtsolutions qtcolortriangle qtpropertybrowser qtwindowlistmenu
qtsolutions: qtcolortriangle qtpropertybrowser qtwindowlistmenu

qtcolortriangle:
	@cd upstream/QtSolutions/qtcolortriangle-2.6_1-opensource ; \
	echo "license accepted" > .licenseAccepted ; \
	./configure

qtpropertybrowser:
	@cd upstream/QtSolutions/qtpropertybrowser-2.5_1-opensource ; \
	echo "license accepted" > .licenseAccepted ; \
	./configure

qtwindowlistmenu:
	@cd upstream/QtSolutions/qtwindowlistmenu-2.2_1-opensource ; \
	echo "license accepted" > .licenseAccepted ; \
	./configure

###############################################################################
# Qt
###############################################################################
.PHONY: qt-frontend quatsch-source-editor picohelp simplexgui
qt-frontend: quatsch-source-editor picohelp simplexgui

quatsch-source-editor: qtsolutions directories picogen-lib
	@cd ./qt-frontend/widgets/QuatschSourceEditor/ ;\
	$(QMAKE) && $(MAKE) ; \
        cp picogen-quatsch-editor ../../../BUILD/

        #windows
        #\
	#&& $(MAKE) release \
	#&& cp release/picogen-quatsch-editor.exe ../../../BUILD/

picohelp: qtsolutions directories
	@cd ./qt-frontend/widgets/Picohelp/ ;\
	$(QMAKE) && $(MAKE) ;\
        cp picohelp ../../../BUILD/

        # windows
        #\
        #$(MAKE) release \
	#&& cp release/picohelp.exe ../../../BUILD/

simplexgui: qtsolutions directories picogen-lib
	@cd ./qt-frontend/SimplexGui/ ;\
	$(QMAKE) && $(MAKE); \
        cp picogen-simplex ../../BUILD/

        # windows
        #\
	#&& $(MAKE) release \
	#&& cp release/picogen-simplex.exe ../../BUILD/


.PHONY: qt-clean
qt-clean:
	@cd ./qt-frontend/widgets/QuatschSourceEditor/ ; $(MAKE) clean
	@cd ./qt-frontend/widgets/Picohelp/ ; $(MAKE) clean
	@cd ./qt-frontend/SimplexGui/ ; $(MAKE) clean

.PHONY: qt-distclean
qt-distclean:
	@cd ./qt-frontend/widgets/QuatschSourceEditor/ ; $(MAKE) distclean
	@cd ./qt-frontend/widgets/Picohelp/ ; $(MAKE) distclean
	@cd ./qt-frontend/SimplexGui/ ; $(MAKE) distclean


###############################################################################
# Meta
###############################################################################

.PHONY: directories
directories:
	mkdir -p BUILD

.PHONY: strip-binaries
strip-binaries:
	strip --strip-all BUILD/*
