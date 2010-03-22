#include <QtGui/QApplication>
#include "mainwindow.hh"
#include <QCleanlooksStyle>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QCleanlooksStyle *q = new QCleanlooksStyle();
    w.setStyle(q);
    w.show();
    return a.exec();
}
