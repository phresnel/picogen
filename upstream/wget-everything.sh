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

popd

echo creating tarball of downloaded stuff
tar -cf QtSolutions.tar QtSolutions
gzip QtSolutions.tar
