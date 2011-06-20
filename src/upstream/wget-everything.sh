mkdir -p QtSolutions && pushd QtSolutions

echo getting QtColorTriangle ...
wget http://get.qt.nokia.com/qt/solutions/lgpl/qtcolortriangle-2.6_1-opensource.tar.gz
tar -xzf qtcolortriangle-2.6_1-opensource.tar.gz
rm qtcolortriangle-2.6_1-opensource.tar.gz

popd

echo creating tarball of downloaded stuff
tar -cf QtSolutions.tar QtSolutions
gzip QtSolutions.tar

