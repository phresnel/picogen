for f in $(ls DEBS/); do
        #deb

        PACKAGE="picogen-$(cat DEBS/${f}/usr/share/picogen/version)-$f.deb"
        if [ -f ${PACKAGE} ] ; then
                echo ${PACKAGE} already exists, skipping.
        else
                dpkg -b DEBS/$f ${PACKAGE}
        fi

        #tarball
        TARBALL="picogen-$(cat DEBS/${f}/usr/share/picogen/version)-$f.tar.gz"
        TAR="picogen-$(cat DEBS/${f}/usr/share/picogen/version)-$f.tar"
        if [ -f ${TARBALL} ] ; then
                echo ${TARBALL} already exists, skipping.
        else
                mkdir -p TARBALLS/$f

                cp DEBS/$f/usr/bin/* TARBALLS/$f/

                cp -r DEBS/$f/usr/share/doc/picogen/* TARBALLS/$f/
                gunzip TARBALLS/$f/changelog.gz

                cp -r DEBS/$f/usr/share/picogen/* TARBALLS/$f/

                cp DEBS/$f/DEBIAN/control TARBALLS/$f/ABOUT

                cd TARBALLS/$f
                tar -cf ../../${TAR} *
                cd ../..
                gzip ${TAR}
        fi
done