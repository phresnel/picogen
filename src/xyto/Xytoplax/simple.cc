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

#include "ui_simple.h"

#include "../lsystem.hh"
#include "../xyto_ios.hh"
boost::optional<LSystem> compile(const char*, const char*);

Simple::Simple(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Simple)
{
        ui->setupUi(this);

        // Koch curve:
        //ui->sourceCode->setPlainText("foo: foo --> foo right foo left foo left foo right foo;\n"


        ui->sourceCode->setPlainText(
        "f1:   foo --> [1] foo [left foo] \n"
        "              [1] foo [right foo] \n"
        "              [1.3] foo foo"
        ";  \n"
        );
}



Simple::~Simple() {
        delete ui;
}


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

void draw (Pattern pat, Turtle turtle, QGraphicsScene &scene) {
        typedef Pattern::const_iterator It;

        for (It it = pat.begin(); it!=pat.end(); ++it) {
                Segment seg = *it;
                if (seg.type() == Segment::Branch) {
                        draw (seg.branch(), turtle, scene);
                } else if (seg.type() == Segment::Letter) {
                        if (seg.name() == "foo") {
                                const Turtle oldBoy = turtle;
                                turtle.forward(10);
                                scene.addLine(oldBoy.x, -oldBoy.y,
                                              turtle.x, -turtle.y);
                        } else if (seg.name() == "left") {
                                turtle.turnLeft(0.5);
                        } else if (seg.name() == "right") {
                                turtle.turnRight(0.5);
                        }
                }
        }
}



void Simple::on_pushButton_clicked() {
        const boost::optional<LSystem> lsys =
                compile(ui->sourceCode->toPlainText().toAscii(),
                        "foo");
        if (!lsys) {
                ui->outputPattern->setPlainText("<invalid L-System!>");
                return;
        }
        std::stringstream ss;
        for (unsigned int i=0; i<10; ++i) {
                const Pattern pat = lsys->run(i);
                ss << pat << '\n';
        }
        ui->outputPattern->setPlainText(QString::fromStdString(ss.str()));


        //--
        QGraphicsScene *scene = new QGraphicsScene (this);
        scene->addEllipse(-10,-10,20,20);
        draw (lsys->run(10), Turtle(), *scene);

        ui->graphicsView->setScene(scene);

}
