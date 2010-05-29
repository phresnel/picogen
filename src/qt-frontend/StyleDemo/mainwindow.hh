#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QMouseEvent>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:    
        void mousePressEvent( QMouseEvent *e){
                if (e->buttons()&Qt::LeftButton) {
                        offset = e->globalPos()-pos();
                        e->accept();
                }
        }

        void mouseMoveEvent(QMouseEvent *e){
                if (e->buttons()&Qt::LeftButton) {
                        move(e->globalPos()-offset);
                        e->accept();
                }
        }

        void mouseReleaseEvent(QMouseEvent *e){
                if (e->buttons()&Qt::LeftButton) {
                        e->accept();
                }
        }

protected:
        void changeEvent(QEvent *e);

private:
    QPoint offset;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_HH
