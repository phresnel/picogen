cp ../CHANGES.txt data/
todos data/CHANGES.txt
cp -r ../DIST-EXTRAS/help-content/ data/
cp -r ../DIST-EXTRAS/repository/ data/

# data extras (dlls)
cp data-extras.tar.bz2 data/
pushd data
bunzip2 data-extras.tar.bz2
tar -xf data-extras.tar
rm data-extras.*
popd

# done
echo "Done preparation. Ensure the .exe-files are in data."
