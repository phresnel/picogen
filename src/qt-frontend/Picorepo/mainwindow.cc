#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <sstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(new QGraphicsScene(this));
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

#include <QGraphicsPixmapItem>

void MainWindow::on_pushButton_clicked() {
        QGraphicsScene *scene = ui->graphicsView->scene();//new QGraphicsScene(this);



        class Cover {
        public:
                Cover (QString const &filename, QGraphicsItem *parent=0) {
                        QPixmap pix (filename);
                        T.translate(-pix.width()*.5, -pix.height()*.5);

                        item_ = new QGraphicsPixmapItem(pix, parent);
                        updateTransform();
                }

                QTransform transform() const {
                        return T*R;
                }

                QGraphicsItem *item() {
                        return item_;
                }

                void rotate (qreal a, Qt::Axis axis = Qt::ZAxis) {
                        R.rotate(a, axis);
                        updateTransform();
                }

                void translate (qreal x, qreal y) {
                        T.translate(x, y);
                        updateTransform();
                }

        private:

                void updateTransform() {
                        item_->setTransform(transform());
                }
                QTransform T, R;
                QGraphicsItem *item_;
        };

        /*
        M.setMatrix(
                M.m11(), M.m12(), M.m13()*4,
                M.m21(), M.m22(), M.m23()*4,
                M.m31(), M.m32(), M.m33()
        );
        */

        Cover *cover = new Cover ("/home/smach/Desktop/hmmmm_____skylighlighlight_by_greenhybrid.png");
        cover->translate(-700, 0);
        cover->rotate(-75, Qt::YAxis);
        scene->addItem(cover->item());

        Cover *cover2 = new Cover ("/home/smach/Desktop/hmmmm_____skylighlighlight_by_greenhybrid.png");
        cover2->translate(700, 0);
        cover2->rotate(75, Qt::YAxis);
        scene->addItem(cover2->item());

        scene->addLine(0,0, 100,0);
        scene->addLine(0,0, 0,100);

        /*
         x' = m11*x + m21*y + dx
         y' = m22*y + m12*x + dy
         if (is not affine) {
             w' = m13*x + m23*y + m33
             x' /= w'
             y' /= w'
         }
        */
        QTransform P;
        P.setMatrix(
                1, 0, 0,
                0, 1, 0,
                0, 0, 1
        );
        ui->graphicsView->setTransform(P);
        ui->graphicsView->setSceneRect(QRectF(-10,-10, 20, 20));

        //ui->graphicsView->setupViewport();

        /*QTransform w;
        w.translate(100,0);
        ui->graphicsView->setTransform(w);*/

        //ui->graphicsView->setScene(scene);
}
