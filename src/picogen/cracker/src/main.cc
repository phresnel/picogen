#include <QtGui/QApplication>
#include "mainwindow.h"
#include <cassert>

void cracker_static_init();

int main(int argc, char *argv[])
{
    cracker_static_init ();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
