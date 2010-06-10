//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
// * mail: phresnel/at/gmail/dot/com
// * http://phresnel.org
// * http://picogen.org
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <sstream>
#include <cmath>
#include "cover.hh"

//#include "pictureflow-qt/pictureflow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(new QGraphicsScene(this));

    /*
    PictureFlow *flow = new PictureFlow(this);
    QString const base = "C:/Dokumente und Einstellungen/smach/Eigene Dateien/garbage/cc/picogen-picogen-master/picogen-picogen/repository/scenes/examples/calm-hills";
    flow->addSlide(QImage(base+"/done.png"));
    flow->addSlide(QImage(base+"/three hours.png"));
    flow->addSlide(QImage(base+"/funkydots.png"));
    flow->setReflectionEffect(PictureFlow::PlainReflection);
    flow->setSlideSize(QSize(256,256));*/

    //setCentralWidget(ui->graphicsView);
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


void MainWindow::on_pushButton_clicked() {
        QGraphicsScene *scene = ui->graphicsView->scene();

        static bool init = true;
        if (init) {
                init = false;

                /*
                M.setMatrix(
                        M.m11(), M.m12(), M.m13()*4,
                        M.m21(), M.m22(), M.m23()*4,
                        M.m31(), M.m32(), M.m33()
                );
                */

                QString const base = "C:/Dokumente und Einstellungen/smach/Eigene Dateien/garbage/cc/picogen-picogen-master/picogen-picogen/repository/scenes/examples/calm-hills";
                QString const a = base+"/done.png";
                QString const b = base+"/three hours.png";
                QString const c = base+"/funkydots.png";

                const int count = 1;
                for (int i=0; i<count; ++i) {
                        Cover *cover = new Cover(a);
                        /*const float
                                f = i / (float)(count-1)
                        ;
                        cover->slide(2,0);
                        cover->translate(-1,0);
                        cover->rotate(-22.5f+f*45, Qt::ZAxis);*/
                        scene->addItem(cover);
                }

                scene->addLine(0,0, 1,0);
                scene->addLine(0,0, 0,1);


                ui->graphicsView->fitInView(-2,-2,4,4,Qt::KeepAspectRatio);

        }
        ui->graphicsView->update();
}
