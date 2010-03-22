#include "simpleinputbox.hh"
#include "ui_simpleinputbox.h"

SimpleInputBox::SimpleInputBox(QWidget *parent) :
                QDialog(parent, Qt::FramelessWindowHint),
    ui(new Ui::SimpleInputBox)
{
        ui->setupUi(this);
        setModal(true);
}

SimpleInputBox::~SimpleInputBox()
{
        delete ui;
}

void SimpleInputBox::changeEvent(QEvent *e)
{
        QDialog::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
                ui->retranslateUi(this);
        break;
        default:
        break;
        }
}

void SimpleInputBox::on_lineEdit_returnPressed() {
        value_ = ui->lineEdit->text();
        close();
}
