for f in $(ls DEBS/); do
        mkdir -p DEBS/$f/usr/share/picogen/help-content ;
        cp -r ../DIST-EXTRAS/help-content/* DEBS/$f/usr/share/picogen/help-content/;
done