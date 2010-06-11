for f in $(ls DEBS/); do
        mkdir -p DEBS/$f/usr/share/picogen/help-content ;
        cp -r ../DIST-EXTRAS/help-content/* DEBS/$f/usr/share/picogen/help-content/;

        mkdir -p DEBS/$f/usr/share/picogen/repository ;
        cp -r ../DIST-EXTRAS/repository/* DEBS/$f/usr/share/picogen/repository/;
done