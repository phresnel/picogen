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
#include <cmath>

#include "simple.hh"

#include <QGraphicsView>
#include <QMessageBox>

#include "ui_simple.h"

#include "../lsystem.hh"
#include "../xyto_ios.hh"
boost::optional<LSystem> compile(const char*);

Simple::Simple(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Simple)
{
        ui->setupUi(this);

        // Koch curve:
        //ui->sourceCode->setPlainText("foo: foo --> foo right foo left foo left foo right foo;\n"


        ui->sourceCode->setPlainText(
                        "axiom: x(10);\n"
                        "f0:  x(x)  -->  f(x)   [left(75)  x(x*0.6)] right(7) f(x) [right(75) x(x*0.6)] x(0.5*x);\n"

                        /*
                        // ABoP, p. 25, figure f
                        f0:  x(x)  -->  f(x) left(22.5) [[x(x)]right(22.5)x(x)]right(22.5)f(x)[right(22.5)f(x)x(x)]left(22.5)x(x);
                        f1: f(x) --> f(x)f(x);
                        */

        );
}



Simple::~Simple() {
        delete ui;
}


struct TurtleVector {
        double x,y,z;
        TurtleVector () : x(0), y(0), z(0) {}
        TurtleVector (double x, double y, double z) : x(x), y(y), z(z) {}

        TurtleVector& operator += (TurtleVector rhs) {
                x += rhs.x;
                y += rhs.y;
                z += rhs.z;
                return *this;
        }
};

class TurtleMatrix {
public:
        TurtleMatrix()  :
                m00(1), m01(0),  m02(0),
                m10(0), m11(1),  m12(0),
                m20(0), m21(0),  m22(1)
        {
        }

        TurtleMatrix (double m00, double m01, double m02,
                      double m10, double m11, double m12,
                      double m20, double m21, double m22
                      ) :
                m00(m00), m01(m01),  m02(m02),
                m10(m10), m11(m11),  m12(m12),
                m20(m20), m21(m21),  m22(m22)
        {
        }

        static TurtleMatrix Heading(double a) {
                using std::sin; using std::cos;
                return TurtleMatrix(
                        1, 0,      0,
                        0, cos(a), -sin(a),
                        0, sin(a), cos(a)
                );
        }
        static TurtleMatrix Left(double a) {
                using std::sin; using std::cos;
                return TurtleMatrix(
                        cos(a), 0, -sin(a),
                        0,      1, 0,
                        sin(a), 0, cos(a)
                );
        }
        static TurtleMatrix Up(double a) {
                using std::sin; using std::cos;
                return TurtleMatrix(
                        cos(a),  sin(a), 0,
                        -sin(a), cos(a), 0,
                        0,       0,      1
                );
        }

        TurtleMatrix operator * (TurtleMatrix rhs) const {
                TurtleMatrix ret;
                for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) {
                        ret[i+j*3] =
                                (*this)[i + 0*3] * rhs[0 + 3*j] +
                                (*this)[i + 1*3] * rhs[1 + 3*j] +
                                (*this)[i + 2*3] * rhs[2 + 3*j]
                        ;
                }
                return ret;
        }

        TurtleVector operator * (TurtleVector v) const {
                return TurtleVector(
                        m00*v.x + m01*v.y + m02*v.z,
                        m10*v.x + m11*v.y + m12*v.z,
                        m20*v.x + m21*v.y + m22*v.z
                );
        }

private:
        double& operator [] (unsigned int index) {
                switch (index) {
                default:
                case 0*3+0: return m00;
                case 0*3+1: return m01;
                case 0*3+2: return m02;
                case 1*3+0: return m10;
                case 1*3+1: return m11;
                case 1*3+2: return m12;
                case 2*3+0: return m20;
                case 2*3+1: return m21;
                case 2*3+2: return m22;
                }
        }
        double operator [] (unsigned int index) const {
                switch (index) {
                default:
                case 0*3+0: return m00;
                case 0*3+1: return m01;
                case 0*3+2: return m02;
                case 1*3+0: return m10;
                case 1*3+1: return m11;
                case 1*3+2: return m12;
                case 2*3+0: return m20;
                case 2*3+1: return m21;
                case 2*3+2: return m22;
                }
        }

        double m00, m01, m02;
        double m10, m11, m12;
        double m20, m21, m22;
};


/*
struct Turtle {
        float x, y, z;
        float theta, phi;

        Turtle() : x(0), y(0), z(0), theta(3.14159/2.), phi(3.14159/2.) {}

        void forward (float f) {
                using std::sin; using std::cos;
                x += f * sin(theta) * cos(phi);
                y += f * sin(theta) * sin(phi);
                z += f * cos(theta);
        }

        void turnLeft (float f=3.14159/2) {
                phi += f;
        }
        void turnRight (float f=3.14159/2) {
                phi -= f;
        }
};
*/
struct Turtle {
        TurtleVector position;
        TurtleMatrix rotation;

        Turtle() {}

        void forward (float f) {
                using std::sin; using std::cos;
                position += rotation*TurtleVector(0,0,f);
        }

        void turnLeft (float f=3.14159/2) {
                rotation = rotation * TurtleMatrix::Left(-f);
        }
        void turnRight (float f=3.14159/2) {
                rotation = rotation * TurtleMatrix::Left(f);
        }
};



void draw (Pattern pat, Turtle turtle, QGraphicsScene &scene) {
        typedef Pattern::const_iterator It;

        for (It it = pat.begin(); it!=pat.end(); ++it) {
                Segment seg = *it;
                if (seg.type() == Segment::Branch) {
                        draw (seg.branch(), turtle, scene);
                } else if (seg.type() == Segment::Letter) {
                        if (seg.name() == "left") {
                                if (!seg.parameterList().empty()) {
                                        turtle.turnLeft(seg.parameterList()[0].toReal() * 0.0174532925);
                                } else {
                                        turtle.turnLeft(0.5);
                                }
                        } else if (seg.name() == "right") {
                                if (!seg.parameterList().empty())
                                        turtle.turnRight(seg.parameterList()[0].toReal() * 0.0174532925);
                                else
                                        turtle.turnRight(0.5);
                        } else if (seg.name() == "f"){
                                const Turtle oldBoy = turtle;

                                if (!seg.parameterList().empty())
                                        turtle.forward(seg.parameterList()[0].toReal());
                                else
                                        turtle.forward(1);

                                scene.addLine(oldBoy.position.x, -oldBoy.position.z,
                                              turtle.position.x, -turtle.position.z);
                        }
                }
        }
}




void Simple::on_draw_clicked() {
        const boost::optional<LSystem> lsys =
                compile(ui->sourceCode->toPlainText().toAscii());
        if (!lsys) {
                ui->outputPattern->setPlainText("<invalid L-System!>");
                return;
        }

        //--
        QGraphicsScene *scene = new QGraphicsScene (this);
        scene->addEllipse(-10,-10,20,20);

        ui->graphicsView->setRenderHint(QPainter::Antialiasing, true);
        draw (lsys->run(ui->numIterations->value()), Turtle(), *scene);
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
