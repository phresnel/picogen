for f in $(ls DEBS/); do
        mkdir -p DEBS/$f/usr/share/picogen/help-content ;
        cp -r ../dist-extras/help-content/* DEBS/$f/usr/share/picogen/help-content/;
done