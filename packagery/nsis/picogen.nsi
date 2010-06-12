!include "MUI2.nsh"
!include "FileAssociation.nsh"


	!define MUI_ICON				"media\icon.ico"
	!define MUI_UNICON				"media\icon.ico"

	!define MUI_WELCOMEFINISHPAGE_BITMAP "media\welcome.bmp"

	!define MUI_HEADERIMAGE
	!define MUI_HEADER_TRANSPARENT_TEXT
	!define MUI_HEADERIMAGE_BITMAP_NOSTRETCH
	!define MUI_HEADERIMAGE_BITMAP "media\header.bmp"

# Welcome page
	!define MUI_WELCOMEPAGE_TITLE "picogen - a terrain synthesizer and renderer"
	!define MUI_WELCOMEPAGE_TITLE_3LINES
	!define MUI_WELCOMEPAGE_TEXT "Welcome, this will install picogen onto your windows-installation."
	!insertmacro MUI_PAGE_WELCOME

# License page
	!define MUI_LICENSEPAGE_TEXT_TOP "To install picogen, please accept the following license terms."
	!insertmacro MUI_PAGE_LICENSE "data\LICENSE.GPL3.txt"

# Directory
	!insertmacro MUI_PAGE_DIRECTORY

# Start-Menu
	Var StartMenuFolder
	!define MUI_STARTMENUPAGE_DEFAULTFOLDER "picogen"
	!insertmacro MUI_PAGE_STARTMENU "Application" $StartMenuFolder

# Install + Finish
	!insertmacro MUI_PAGE_INSTFILES
	!insertmacro MUI_PAGE_FINISH


Name "picogen"
outfile "picogen-VERSION-ARCH-install.exe"
installDir $PROGRAMFILES\picogen

!insertmacro MUI_LANGUAGE "English"

# start default section
section

    # set the installation directory as the destination for the following actions
    setOutPath $INSTDIR

	file data\*.dll
	file data\*.txt
	file data\*.exe
	file /r data\help-content
	file /r data\repository

    # create the uninstaller
    writeUninstaller "$INSTDIR\uninstall.exe"

    createDirectory "$SMPROGRAMS\$StartMenuFolder"
    createShortCut "$SMPROGRAMS\$StartMenuFolder\picogen-simplex.lnk" "$INSTDIR\picogen-simplex.exe"
    createShortCut "$SMPROGRAMS\$StartMenuFolder\picogen-quatsch-editor.lnk" "$INSTDIR\picogen-quatsch-editor.exe"
    createShortCut "$SMPROGRAMS\$StartMenuFolder\picohelp.lnk" "$INSTDIR\picohelp.exe"
    createShortCut "$SMPROGRAMS\$StartMenuFolder\uninstall.lnk" "$INSTDIR\uninstall.exe"

	${registerExtension} "$INSTDIR\picogen-simplex.exe" ".picogen" "Picogen scene file"

sectionEnd

# uninstaller section start
section "uninstall"

    # first, delete the uninstaller
    delete "$INSTDIR\uninstall.exe"

	# remove program data
	delete "$INSTDIR\*.dll"
	delete "$INSTDIR\*.txt"
	delete "$INSTDIR\*.exe"
	rmdir /r "$INSTDIR\repository"
	rmdir /r "$INSTDIR\help-content"
	rmdir "$INSTDIR"

	# remove start menu items
	!insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
	delete "$SMPROGRAMS\$StartMenuFolder\picogen-simplex.lnk"
    delete "$SMPROGRAMS\$StartMenuFolder\picogen-quatsch-editor.lnk"
    delete "$SMPROGRAMS\$StartMenuFolder\picohelp.lnk"
    delete "$SMPROGRAMS\$StartMenuFolder\uninstall.lnk"
	rmdir  "$SMPROGRAMS\$StartMenuFolder"

	${unregisterExtension} ".picogen" "Picogen scene file"

# uninstaller section end
sectionEnd