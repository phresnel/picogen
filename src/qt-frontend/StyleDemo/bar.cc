#include "bar.hh"
#include "ui_bar.h"

const Qt::WFlags subwindowFlag = ( Qt::Window
                                   | Qt::WindowStaysOnTopHint
                                   | Qt::FramelessWindowHint
                                   | Qt::Tool
#if defined(Q_WS_X11)
                                   | Qt::X11BypassWindowManagerHint
#endif
                                 );

Bar::Bar(QWidget *parent) :
    QWidget(parent, subwindowFlag),
    ui(new Ui::Bar)
{
    ui->setupUi(this);
}

Bar::~Bar()
{
    delete ui;
}

void Bar::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
