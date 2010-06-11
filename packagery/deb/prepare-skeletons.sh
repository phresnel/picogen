VERSION=${1}

if [ ! ${VERSION} ] ; then
        echo "missing version."
        exit
fi

function make_control {
        cat control.template | \
                sed -e "s/\$(VERSION)/${1}/" -e "s/\$(ARCH)/${2}/"
}

function make_arch {
        echo "putting ${1} to DEBS/${1}"
        mkdir -p DEBS/${1}/

        # copy deb and control
        cp -r deb.template/* DEBS/${1}/
        make_control ${VERSION} ${1} > DEBS/${1}/DEBIAN/control

        # copy changelog and ensure unix line endings
        cp ../CHANGES.txt DEBS/${1}/usr/share/doc/picogen/changelog
        #fromdos DEBS/${1}/usr/share/doc/picogen/changelog
        gzip --best DEBS/${1}/usr/share/doc/picogen/changelog

        mkdir -p DEBS/${1}/usr/bin/
        echo "replace me with the corresponding binary" > DEBS/${1}/usr/bin/picogen-simplex
        echo "replace me with the corresponding binary" > DEBS/${1}/usr/bin/picohelp
        echo "replace me with the corresponding binary" > DEBS/${1}/usr/bin/picogen-quatsch-editor

        # write version to share/picogen
        echo ${VERSION} > DEBS/${1}/usr/share/picogen/version

        # compress manpages
        gzip --best DEBS/${1}/usr/share/man/man1/picogen.1
}


mkdir -p DEBS
make_arch i386
make_arch amd64

