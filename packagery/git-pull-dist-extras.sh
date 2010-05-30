
echo "Checking if DIST-EXTRAS/.git exists ..."
if [ ! -d DIST-EXTRAS/.git ]; then
        echo "... does not, hence I will clone picogen into that folder now."
        echo "THIS IS A VAST OPERATION, DO IT AS SELDOMLY AS POSSIBLE."
        echo ""
        git clone git://gitorious.org/picogen/picogen.git DIST-EXTRAS
        echo "Switching to dist-extras."
        cd DIST-EXTRAS
        git checkout -b dist-extras origin/dist-extras
        echo "Done."
else
        echo "... is already there. Assuming that origin/dist-extras has been checked out already."
        cd DIST-EXTRAS
        git checkout dist-extras
fi

echo "Pulling."
git pull

echo "Done :)"