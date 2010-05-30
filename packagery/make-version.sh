VERSION=${1:-"<missing version>"}

function make_arch {
        echo "putting ${1} to DEBS/${1}"

        # create deb and control
        cp -r deb.template DEBS/${1}
        ./make-control-file.sh ${VERSION} ${1} > DEBS/${1}/DEBIAN/control

        # copy changelog and ensure unix line endings
        cp CHANGES.txt DEBS/${1}/DEBIAN/changelog
        fromdos DEBS/${1}/DEBIAN/changelog
}


mkdir -p DEBS
make_arch i386
make_arch amd64

