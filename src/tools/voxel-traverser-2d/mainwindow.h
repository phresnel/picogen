#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


        void paintEvent(QPaintEvent *);
        void mouseMoveEvent(QMouseEvent *);
        void mousePressEvent(QMouseEvent *);

private:
        Ui::MainWindow *ui;
        QPointF from_, to_;
};

#endif // MAINWINDOW_H
