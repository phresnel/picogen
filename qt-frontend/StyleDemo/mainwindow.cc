#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <foo.hh>
#include <bar.hh>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);    

    Bar *s = new Bar(this);
    ui->mdiArea->addSubWindow(s);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
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
