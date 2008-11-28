#!/bin/bash
clear
echo ""
echo "**************************************************************"
echo "Welcome to the minimal installer script for picogen-wx"
echo "**************************************************************"
echo ""
echo "This install script and picogen have been tested on the "
echo "following GNU/Linux Distros:"
echo ""
echo " Picogen runs after fresh installation of ..."
echo "  * Debian GNU/Linux 4.0r4 ('Debian Etch')"
echo "  * gNewSense 2.1          ('gNewSense DeltaH')"
echo "  * Linux Mint 5           ('Linux Mint Elyssa')"
echo "  * Ubuntu 8.04.1          ('Ubuntu Hardy Heron')"
echo ""
echo " But ... "
echo "  * most x86/64 GNU/Linuxes with GCC 4.1 should work!"
echo ""
echo -e "If you have questions, please visit \033[1mhttp://picogen.org/\033[0m"
echo -e "or get direct support at \033[1mhttp://picogen.org/forum/\033[0m"
echo -e "Porting-/Testing-Work is highly welcome!"
echo ""

echo "(any key to continue)"
read

echo ""
echo "I will begin by copying the following files:"
echo "* f/picogen             to   /usr/bin/"
echo "* f/picogen-wx          to   /usr/bin/"
echo ""
echo "After that, I will change the user of those files to root:root",
echo "set the permissions to read,write,execute for user root,"
echo "                       read,execute for group root,"
echo "                   and execute for all users"
echo ""
echo "Proceed? [yes,no] "
read DOIT

function copyFile () {
    src=$1
    dest=$2
    if [ $# -eq 2 ]; then
        if [ -e $dest ]; then
            echo -e "\033[1m${dest} \033[0malready exists, overwrite? [yes|no]"
            read OVERWRITE
        fi
        if [ ! -e $dest ] || [ "$OVERWRITE" = "yes" ] || [ "$OVERWRITE" = "y" ]; then
            echo -e "\033[32mcopying \033[1m${src} \033[0m\033[32mto\033[1m ${dest} \033[0m\033[32m ...\033[0m"
            cp -f $src $dest
            chown root:root $dest
            chmod a=x $dest
            chmod u=rw $dest
            chmod g=r $dest
        else 
            echo -e "\033[31mcopying \033[1m${src} \033[0m\033[31mto\033[1m ${dest} \033[0m\033[31mskipped ...\033[0m"
        fi
    else
        echo "function 'copyFile()' requires 2 arguments."
    fi
}

if [ "$DOIT" = "yes" ] || [ "$DOIT" = "y" ]; then
    echo "copying files ... "
        
    copyFile f/picogen /usr/bin/picogen
    copyFile f/picogen-wx /usr/bin/picogen-wx
    
    echo "I think the Installation was successfull."
    echo "Please logout to normal user and type 'picogen-wx'! "
    echo "See you."
else
    echo "You have chosen to quit. See you."
fi
