for f in $(ls DEBS/); do
        dpkg -b DEBS/$f picogen-$(cat DEBS/${f}/usr/share/picogen/version)-$f.deb
done