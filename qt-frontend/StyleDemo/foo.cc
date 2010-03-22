#include "foo.hh"
#include "ui_foo.h"

Foo::Foo(QWidget *parent) :
        QMainWindow(parent, 
                    Qt::FramelessWindowHint | Qt::SubWindow),
        ui(new Ui::Foo)
{
    ui->setupUi(this);
}

Foo::~Foo()
{
    delete ui;
}

void Foo::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
