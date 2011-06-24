#include "complexgui.hh"
#include "ui_complexgui.h"

ComplexGUI::ComplexGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ComplexGUI)
{
    ui->setupUi(this);
}

ComplexGUI::~ComplexGUI()
{
    delete ui;
}

void ComplexGUI::changeEvent(QEvent *e)
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
