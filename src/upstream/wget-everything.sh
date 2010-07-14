mkdir -p QtSolutions && pushd QtSolutions

echo getting QtColorTriangle ...
wget http://get.qt.nokia.com/qt/solutions/lgpl/qtcolortriangle-2.6_1-opensource.tar.gz
tar -xzf qtcolortriangle-2.6_1-opensource.tar.gz
rm qtcolortriangle-2.6_1-opensource.tar.gz

echo getting QtPropertyBrowser ...
wget http://get.qt.nokia.com/qt/solutions/lgpl/qtpropertybrowser-2.5_1-opensource.tar.gz
tar -xzf qtpropertybrowser-2.5_1-opensource.tar.gz
rm qtpropertybrowser-2.5_1-opensource.tar.gz

echo getting QtWindowListMenu
wget http://get.qt.nokia.com/qt/solutions/lgpl/qtwindowlistmenu-2.2_1-opensource.tar.gz
tar -xzf qtwindowlistmenu-2.2_1-opensource.tar.gz
rm qtwindowlistmenu-2.2_1-opensource.tar.gz

echo getting QtSingleApplication
wget http://get.qt.nokia.com/qt/solutions/lgpl/qtsingleapplication-2.6_1-opensource.tar.gz
tar -xzf qtsingleapplication-2.6_1-opensource.tar.gz
rm qtsingleapplication-2.6_1-opensource.tar.gz

echo getting QtLockedFile
wget http://get.qt.nokia.com/qt/solutions/lgpl/qtlockedfile-2.4_1-opensource.tar.gz
tar -xzf qtlockedfile-2.4_1-opensource.tar.gz
rm qtlockedfile-2.4_1-opensource.tar.gz

popd

echo creating tarball of downloaded stuff
tar -cf QtSolutions.tar QtSolutions
gzip QtSolutions.tar



mkdir -p qt-widgets && pushd qt-widgets


#echo getting QtCoverFlow
#wget http://th30z.netsons.org/wp-content/uploads/qtcoverflow.zip
#unzip qtcoverflow.zip
#rm qtcoverflow.zip

echo getting qt-pictureflow
wget http://pictureflow.googlecode.com/files/pictureflow-0.1.0.tar.gz
tar -xzf pictureflow-0.1.0.tar.gz
rm pictureflow-0.1.0.tar.gz

popd

echo creating tarball of downloaded stuff
tar -cf qt-widgets.tar qt-widgets
gzip qt-widgets.tar


mkdir -p image-formats && pushd image-formats

echo getting OpenEXR
wget http://download.savannah.gnu.org/releases/openexr/openexr-1.6.1.tar.gz
tar -xzf openexr-1.6.1.tar.gz
rm openexr-1.6.1.tar.gz

popd

echo creating tarball of downloaded stuff
tar -cf image-formats.tar image-formats
gzip image-formats.tar
