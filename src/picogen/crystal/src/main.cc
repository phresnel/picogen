#include <QtGui/QApplication>
#include "mainwindow.h"
#include <cassert>

void psssunsky_static_init();
int main(int argc, char *argv[])
{
        psssunsky_static_init();
        QApplication a(argc, argv);
        MainWindow w;
        w.show();

        return a.exec();
}
