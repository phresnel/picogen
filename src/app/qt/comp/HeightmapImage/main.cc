#include <QtGui/QApplication>
#include "heightmapimage.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HeightmapImage w;
    w.show();

    return a.exec();
}
