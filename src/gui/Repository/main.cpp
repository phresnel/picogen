#include <QtGui/QApplication>
#include "mainwindow.hh"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    picogen_repository::MainWindow w;
    w.show();

    return a.exec();
}
