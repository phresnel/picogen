#include <QtGui/QApplication>
#include "mainwindow.hh"
#include <QCleanlooksStyle>

#include "../../redshift/include/static_init.hh"
int main(int argc, char *argv[])
{
        QCleanlooksStyle *style = new QCleanlooksStyle ();
        QApplication::setStyle(style);

        redshift::static_init();
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        const int ret = a.exec();
        return ret;
}
