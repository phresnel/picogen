#include <QtGui/QApplication>
#include "mainwindow.hh"
#include "complexgui.hh"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ComplexGUI w;
    w.show();
    return a.exec();
}
