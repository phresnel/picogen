makensis picogen.nsi
mkdir -p BUILD/picogen
cp -r data/* BUILD/picogen/

pushd BUILD
 echo making 7z archive
 7z a -t7z -m0=lzma -mx=9 -mfb=64 -md=32m -ms=on ../picogen-VERSION-ARCH.7z picogen/
 echo making zip archive
 zip -r -9 ../picogen-VERSION-ARCH.zip picogen/
popd
rm -rf BUILD

