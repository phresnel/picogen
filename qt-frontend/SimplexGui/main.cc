#include <QtGui/QApplication>
#include "mainwindow.hh"

#include "../../redshift/include/static_init.hh"
int main(int argc, char *argv[])
{
        redshift::static_init();
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
}
