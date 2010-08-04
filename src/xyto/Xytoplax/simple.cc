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
TurtleVector operator+ (TurtleVector lhs, TurtleVector rhs) {
        return TurtleVector(lhs.x+rhs.x, lhs.y+rhs.y, lhs.z+rhs.z);
}
TurtleVector operator* (TurtleVector lhs, double rhs) {
        return TurtleVector(lhs.x*rhs, lhs.y*rhs, lhs.z*rhs);
}
TurtleVector operator* (double lhs, TurtleVector rhs) {
        return TurtleVector(lhs*rhs.x, lhs*rhs.y, lhs*rhs.z);
}

double dot (TurtleVector lhs, TurtleVector rhs) {
        return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
}
double length_sq (TurtleVector vec) {
        return dot(vec, vec);
}
double length (TurtleVector vec) {
        return std::sqrt(length_sq(vec));
}
TurtleVector normalize (TurtleVector vec) {
        const double len = 1/length(vec);
        return TurtleVector(vec.x*len,
                            vec.y*len,
                            vec.z*len);
}

TurtleVector cross (TurtleVector lhs, TurtleVector rhs) {
        return TurtleVector(
                lhs.y*rhs.z - lhs.z*rhs.y,
                lhs.z*rhs.x - lhs.x*rhs.z,
                lhs.x*rhs.y - lhs.y*rhs.x
        );
}
std::ostream& operator<< (std::ostream& o, TurtleVector const &rhs) {
        o << "[" << rhs.x << ", " << rhs.y << ", " << rhs.z << "]";
        return o;
}


class TurtleMatrix {
public:
        TurtleMatrix()  :
                m00(1), m01(0),  m02(0),
                m10(0), m11(1),  m12(0),
                m20(0), m21(0),  m22(1)
        {
        }

        TurtleMatrix(TurtleVector right, TurtleVector up, TurtleVector forw)
                :
                m00(right.x), m01(up.x),  m02(forw.x),
                m10(right.y), m11(up.y),  m12(forw.y),
                m20(right.z), m21(up.z),  m22(forw.z)
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

        static TurtleMatrix RotateX(double a) {
                using std::sin; using std::cos;
                return TurtleMatrix(
                        1, 0,      0,
                        0, cos(a), -sin(a),
                        0, sin(a), cos(a)
                );
        }
        static TurtleMatrix RotateY(double a) {
                using std::sin; using std::cos;
                return TurtleMatrix(
                        cos(a), 0, -sin(a),
                        0,      1, 0,
                        sin(a), 0, cos(a)
                );
        }
        static TurtleMatrix RotateZ(double a) {
                using std::sin; using std::cos;
                return TurtleMatrix(
                        cos(a),  -sin(a), 0,
                        sin(a), cos(a), 0,
                        0,       0,      1
                );
        }

        static TurtleMatrix Rotate (double angle, TurtleVector axis) {
                using std::sin; using std::cos;
                const TurtleVector a = normalize(axis);
                const double s = sin(angle), c = cos(angle);

                return TurtleMatrix (
                        a.x * a.x + (1.f - a.x * a.x) * c,
                        a.x * a.y * (1.f - c) - a.z * s,
                        a.x * a.z * (1.f - c) + a.y * s,

                        a.x * a.y * (1.f - c) + a.z * s,
                        a.y * a.y + (1.f - a.y * a.y) * c,
                        a.y * a.z * (1.f - c) - a.x * s,

                        a.x * a.z * (1.f - c) - a.y * s,
                        a.y * a.z * (1.f - c) + a.x * s,
                        a.z * a.z + (1.f - a.z * a.z) * c
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
struct Turtle {
        TurtleVector position;
        TurtleMatrix rotation;
        TurtleVector tropism;
        double e;
        double diameter;

        Turtle() {
                pitchUp(3.14159*0.5);
                diameter = 1;
                tropism = TurtleVector(0,-1,0);
                e = 0.22;
        }


        /*

(defun turtle-update-orientation (torque angle)
  (setf (state-orientation *turtle*)
        (turn-space (state-orientation *turtle*) torque angle)))


(defun turtle-adjust (vector amount)
  (let ((torque (cross-product (point-normalize vector) (get-normal *turtle*))))
    (turtle-update-orientation torque (* amount (point-length torque)))))


(defun turtle-move (amount)
  (setf (state-location *turtle*) (turtle-displacement amount)))


(defun turtle-forward (amount)
  (if (null (turtle-branch *turtle*)) (add-new-branch))
  (turtle-move amount)
  (add-new-branch))


(defun turtle-tropism (distance vector strength)
  (turtle-forward distance)
  (turtle-adjust vector strength))
        */

        //(defun turtle-adjust (vector amount)
        //  (let ((torque (cross-product (point-normalize vector) (get-normal *turtle*))))
        //    (turtle-update-orientation torque (* amount (point-length torque)))))
        void adjust (TurtleVector vector, double amount) {
                TurtleVector torque = cross (normalize(vector), heading(1));
                rotation = rotation * TurtleMatrix::Rotate(
                                -amount*length(torque),
                                normalize(torque))
                           ;
                //pitchUp(length(torque));
        }

        void tropism_ (double distance, TurtleVector vector, double strength) {
                adjust (vector, strength);
                position += heading(distance); //forward (distance);
        }


        void forward (double f) {
                tropism_ (f, tropism, e);
        }

        void turnLeft (double f) {
                rotation = TurtleMatrix::RotateY(f) * rotation;
        }
        void turnRight (double f) {
                rotation = TurtleMatrix::RotateY(-f) * rotation;
        }

        void pitchUp (double f) {
                rotation = TurtleMatrix::RotateX(-f) * rotation;
        }
        void pitchDown (double f) {
                rotation = TurtleMatrix::RotateX(f) * rotation;
        }

        void rollLeft (double f) {
                rotation = TurtleMatrix::RotateZ(f) * rotation;
        }
        void rollRight (double f) {
                rotation = TurtleMatrix::RotateZ(-f) * rotation;
        }

        void decrementDiameter (double f) {
                diameter = f;
        }

        TurtleVector up() const { return rotation*TurtleVector(0,1,0); }
        /*TurtleVector right() const { return rotation*TurtleVector(1,0,0); }*/
        TurtleVector heading(double f=1) const {
                return rotation*TurtleVector(0,0,f);
        }

        void rollToVertical() {
                const TurtleVector
                        up = TurtleVector (0,1,0),
                        forward = normalize(rotation*TurtleVector(0,0,1)),
                        newRight = normalize(cross(up,forward)),
                        newUp = normalize (cross(forward,newRight));

                rotation = TurtleMatrix(newRight, newUp, forward);
        }
};



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



void draw (Pattern pat, Turtle turtle, QGraphicsScene &scene, float rotation=0) {
        typedef Pattern::const_iterator It;

        const TurtleMatrix rot = TurtleMatrix::RotateY(rotation);

        for (It it = pat.begin(); it!=pat.end(); ++it) {
                Segment seg = *it;
                if (seg.type() == Segment::Branch) {
                        draw (seg.branch(), turtle, scene, rotation);
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
                        } else if (seg.name() == "up") {
                                if (!seg.parameterList().empty()) {
                                        turtle.pitchUp(seg.parameterList()[0].toReal() * 0.0174532925);
                                } else {
                                        turtle.pitchUp(0.5);
                                }
                        } else if (seg.name() == "down") {
                                if (!seg.parameterList().empty())
                                        turtle.pitchDown(seg.parameterList()[0].toReal() * 0.0174532925);
                                else
                                        turtle.pitchDown(0.5);
                        } else if (seg.name() == "rollleft") {
                                if (!seg.parameterList().empty()) {
                                        turtle.rollLeft(seg.parameterList()[0].toReal() * 0.0174532925);
                                } else {
                                        turtle.rollLeft(0.5);
                                }
                        } else if (seg.name() == "rollright") {
                                if (!seg.parameterList().empty())
                                        turtle.rollRight(seg.parameterList()[0].toReal() * 0.0174532925);
                                else
                                        turtle.rollRight(0.5);
                        } else if (seg.name() == "vert") {
                                turtle.rollToVertical();
                        } else if (seg.name() == "dia") {
                                if (!seg.parameterList().empty())
                                        turtle.decrementDiameter(seg.parameterList()[0].toReal());
                        } else if (seg.name() == "f"){
                                const Turtle oldBoy = turtle;

                                if (!seg.parameterList().empty()) {
                                        turtle.forward(seg.parameterList()[0].toReal());
                                } else {
                                        turtle.forward(1);
                                }

                                QPen pen;
                                pen.setWidthF(turtle.diameter*1);


                                const TurtleVector from = rot*oldBoy.position;
                                const TurtleVector to   = rot*turtle.position;
                                const double Zfrom = 1;//1 / (1 + 0.001 * (800+from.z)); // <-- very basic perspective
                                const double Zto = 1;//1 / (1 + 0.001 * (800+to.z));
                                scene.addLine(from.x * Zfrom,
                                              -from.y * Zfrom,
                                              to.x * Zto,
                                              -to.y * Zto,
                                              pen);
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
        scene->setItemIndexMethod(QGraphicsScene::NoIndex);
        scene->addEllipse(-1,-1,2,2);

        ui->graphicsView->setRenderHint(QPainter::Antialiasing, true);
        draw (this->lsys = lsys->run(ui->numIterations->value()), Turtle(), *scene);
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
        draw (lsys, Turtle(), *scene, value*0.0174);
        ui->graphicsView->setScene(scene);
        ui->graphicsView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}
