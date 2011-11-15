#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "renderwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget (new RenderWidget(this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

#include "mainwindow.moc"
