for f in $(ls DEBS/); do
        #deb

        PACKAGE="picogen-$(cat DEBS/${f}/usr/share/picogen/version)-$f.deb"
        if [ -f PACKAGES/${PACKAGE} ] ; then
                echo PACKAGES/${PACKAGE} already exists, skipping.
        else
                mkdir -p PACKAGES
                dpkg -b DEBS/$f PACKAGES/${PACKAGE}
                lintian PACKAGES/${PACKAGE}
        fi

        #tarball
        TARBALL="picogen-$(cat DEBS/${f}/usr/share/picogen/version)-$f.tar.gz"
        TAR="picogen-$(cat DEBS/${f}/usr/share/picogen/version)-$f.tar"
        if [ -f PACKAGES/${TARBALL} ] ; then
                echo PACKAGES/${TARBALL} already exists, skipping.
        else
                mkdir -p TARBALLS/$f

                cp DEBS/$f/usr/bin/* TARBALLS/$f/

                cp -r DEBS/$f/usr/share/doc/picogen/* TARBALLS/$f/
                gunzip TARBALLS/$f/changelog.gz

                cp -r DEBS/$f/usr/share/picogen/* TARBALLS/$f/

                cp DEBS/$f/DEBIAN/control TARBALLS/$f/ABOUT

                cd TARBALLS/$f
                tar -cf ../../PACKAGES/${TAR} *
                cd ../..
                gzip PACKAGES/${TAR}

                echo "deleting temporary folder TARBALLS"
                rm -rf TARBALLS
        fi
done
