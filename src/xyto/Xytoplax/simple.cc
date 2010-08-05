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

#include <sstream>
#include "simple.hh"

#include <QGraphicsView>
#include <QMessageBox>
#include <QtOpenGL>

#include "ui_simple.h"

#include "../lsystem.hh"
#include "../draw.hh"
#include "../xyto_ios.hh"


/*
struct Turtle {
        double x, y, z;
        double theta, phi;

        Turtle() : x(0), y(0), z(0), theta(3.14159/2.), phi(3.14159/2.) {}

        void forward (double f) {
                using std::sin; using std::cos;
                x += f * sin(theta) * cos(phi);
                y += f * sin(theta) * sin(phi);
                z += f * cos(theta);
        }

        void turnLeft (double f=3.14159/2) {
                phi += f;
        }
        void turnRight (double f=3.14159/2) {
                phi -= f;
        }
};
*/


boost::optional<LSystem> compile(const char*);

Simple::Simple(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Simple)
{
        ui->setupUi(this);

        /*Turtle a;
        a.rollLeft(0.0174532925 * 45);
        a.rollToVertical();

        exit(0);*/

        // Koch curve:
        //ui->sourceCode->setPlainText("foo: foo --> foo right foo left foo left foo right foo;\n"


        ui->sourceCode->setPlainText(
                        //"axiom: dia(1.0) right(45) f(20) f(20) f(20)  f(20) f(20)  f(20) f(20);"

                        //"axiom: f(20) rollright(90) up(90)  f(20) up(135) f(20);"

                        /*
R=1.456;
a=120; // <-- tweak for fun
axiom:up(90) A(1);
p1: A(s) --> f(s)[right(a)A(s/R)][left(a)A(s/R)];
                         */

                        // abop p. 60

                        "d1 = 94.74;\n"
                        "d2 = 132.63;\n"
                        "a = 18.95;\n"
                        "lr = 1.109;\n"
                        "vr = 1.732;\n"
                        "\n"
                        "axiom: dia(1.0) f(200) rollright(45) A;\n"
                        "p1: A --> dia(vr) f(50) \n"
                        "        [down(a) f(50) A] rollright(d1)\n"
                        "        [down(a) f(50) A] rollright(d2)\n"
                        "        [down(a) f(50) A];\n"
                        "p2: f(l) --> f(l*lr);\n"
                        "p3: dia(w) --> dia(w*vr);\n"
                        //*/

                        // abop p. 59
                        /*
                        "r1=0.9;\n"
                        "r2=0.7;\n"
                        "a1=10;\n"
                        "a2=60;\n"
                        "wr=0.707;\n"
                        "\n"
                        "axiom: A(1,10);\n"
                        "p1: A(l,w) --> dia(w) f(l) [down(a1)B(l*r1,w*wr)] rollright(180) [down(a2)B(l*r2,w*wr)];\n"
                        "p2: B(l,w) --> dia(w) f(l) [left(a1) vert B(l*r1,w*wr)] [right(a2) vert B(l*r2,w*wr)];\n"
                        //*/

                        // abop p. 56
                        /*
                        "\n"
                        "r1=0.9;\n"
                        "r2=0.6;\n"
                        "a0=45;\n"
                        "a2=45;\n"
                        "d=137.5;\n"
                        "wr=0.707;\n"
                        "\n"
                        "axiom: A(1, 10);\n"
                        "\n"
                        "p1 : A(l,w) --> dia(w) f(l) [down(a0)       B(l*r2, w*wr)] rollright(d) A(l*r1, w*wr);\n"
                        "p2 : B(l,w) --> dia(w) f(l) [right(a2) vert C(l*r2, w*wr)] C(l*r1, w*wr);\n"
                        "p3 : C(l,w) --> dia(w) f(l) [left(a2)  vert B(l*r2, w*wr)] B(l*r1, w*wr);\n"
                        "\n" //*/

                        /*"f: f --> f;\n"
                        "axiom: f(50) [left(90) f(10)]   left(45) up(70)  f(50); "*/

                        /*
                        "axiom: x(10);\n"
                        "f0:  x(x)  -->  f(x)   [left(75)  x(x*0.6)] right(7) f(x) [right(75) x(x*0.6)] x(0.5*x);\n"
                        */


                        // ABoP, p. 25, figure f

                        /*"axiom: x(10);\n"
                        "f0:  x(x)  -->  f(x) left(22.5) [[x(x)]right(22.5)x(x)]right(22.5)f(x)[right(22.5)f(x)x(x)]left(22.5)x(x);\n"
                        "f1: f(x) --> f(x)f(x);\n"
                        //*/


        );
}



Simple::~Simple() {
        delete ui;
}


class GraphicsSceneMesh {
public:
        GraphicsSceneMesh (QGraphicsScene &scene, TurtleMatrix rot)
        : scene(scene), rot(rot) {
        }

        void addInner (TurtleVector from_, double diameterFrom,
                       TurtleVector to_, double diameterTo
        ) {
                QPen pen;
                pen.setWidthF(diameterTo*1);
                const TurtleVector from = rot*from_;
                const TurtleVector to = rot*to_;
                //const double Zfrom = 1;//1 / (1 + 0.001 * (800+from.z)); // <-- very basic perspective
                //const double Zto = 1;//1 / (1 + 0.001 * (800+to.z));
                scene.addLine(from.x,
                              -from.y,
                              to.x,
                              -to.y,
                              pen);
        }

private:
        QGraphicsScene &scene;
        TurtleMatrix rot;
};


void Simple::on_draw_clicked() {
        const boost::optional<LSystem> lsys =
                compile(ui->sourceCode->toPlainText().toAscii());
        if (!lsys) {
                ui->outputPattern->setPlainText("<invalid L-System!>");
                return;
        }

        //--
        QGraphicsScene *scene = new QGraphicsScene (this);
        scene->setItemIndexMethod(QGraphicsScene::NoIndex);
        scene->addEllipse(-1,-1,2,2);

        ui->graphicsView->setRenderHint(QPainter::Antialiasing, true);
        GraphicsSceneMesh gsm(*scene, TurtleMatrix::RotateY(ui->rotationY->value()*0.0174));
        draw (this->lsys = lsys->run(ui->numIterations->value()),
              Turtle(),
              gsm);
        ui->graphicsView->setScene(scene);
        ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void Simple::on_write_clicked() {
        const boost::optional<LSystem> lsys =
                compile(ui->sourceCode->toPlainText().toAscii());
        if (!lsys) {
                ui->outputPattern->setPlainText("<invalid L-System!>");
                return;
        }
        std::stringstream ss;
        for (int i=0; i<ui->numIterations->value(); ++i) {
                const Pattern pat = lsys->run(i);
                ss << pat << '\n';
        }
        ui->outputPattern->setPlainText(QString::fromStdString(ss.str()));
}



void Simple::resizeEvent(QResizeEvent *) {
        ui->graphicsView->fitInView(ui->graphicsView->sceneRect(),
                                    Qt::KeepAspectRatio);
}


void Simple::on_rotationY_valueChanged(int value)
{
        //--
        QGraphicsScene *scene = new QGraphicsScene (this);
        scene->setItemIndexMethod(QGraphicsScene::NoIndex);
        scene->addEllipse(-1,-1,2,2);

        ui->graphicsView->setRenderHint(QPainter::Antialiasing, true);
        GraphicsSceneMesh gsm(*scene, TurtleMatrix::RotateY(value*0.0174));
        draw (lsys, Turtle(), gsm);
        ui->graphicsView->setScene(scene);
        ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}
