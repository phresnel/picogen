#include "quatscheditor.h"
#include "ui_quatscheditor.h"

QuatschEditor::QuatschEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuatschEditor)
{
    ui->setupUi(this);
}

QuatschEditor::~QuatschEditor()
{
    delete ui;
}
