VERSION=${1:-"missing_version"}

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
        cp ../CHANGES.txt DEBS/${1}/DEBIAN/changelog
        fromdos DEBS/${1}/DEBIAN/changelog
}


mkdir -p DEBS
make_arch i386
make_arch amd64

