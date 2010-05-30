
VERSION=${1:-"<missing version>"}
ARCH=${2:-"<missing architecture>"}

cat control.template | \
        sed -e "s/\$(VERSION)/${VERSION}/" -e "s/\$(ARCH)/${ARCH}/"
