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

#include "GLWidget.hh"
#include "../turtle.hh"
#include "../draw.hh"


class GLDisplayListMesh {
public:
        GLDisplayListMesh () {
                displayList = glGenLists(1);
                glNewList(displayList, GL_COMPILE);
        }

        ~GLDisplayListMesh () {
                glEndList();
        }

        void moveTo (Turtle state) {
                this->state = state;
        }

        void drawTo (Turtle newState) {
                glColor3f(0, 0, 0);
                /*glLineWidth(newState.diameter);
                glEnable(GL_LINE_SMOOTH);

                glBegin(GL_LINES);
                glVertex3f(state.position.x,
                           state.position.y,
                           state.position.z);
                glLineWidth(newState.diameter*10);
                glVertex3f(newState.position.x,
                           newState.position.y,
                           newState.position.z);*/

                glBegin(GL_QUAD_STRIP);
                const double pi = 3.14159, pi2 = pi*2;
                int count = 16;
                for (int i=0; i<=count; ++i) {
                        const double phi = (i/(float)count) * pi2;
                        const TurtleVector a = state.disk(phi) * state.scale;
                        const TurtleVector b = newState.disk(phi) * newState.scale;
                        const TurtleVector na = state.normal(phi);
                        const TurtleVector nb = newState.normal(phi);
                        glColor3f(0,1,0);
                        glNormal3f(nb.x, nb.y, nb.z);
                        glVertex3f(b.x, b.y, b.z);
                        glNormal3f(na.x, na.y, na.z);
                        glVertex3f(a.x, a.y, a.z);
                }
                glEnd();

                state = newState;
                glEnd();
        }

        GLuint result() const {
                return displayList;
        }
private:
        Turtle state;
        GLuint displayList;
};



int normalizeAngle(int angle) {
     while (angle < 0)
         angle += 360 * 16;
     while (angle > 360 * 16)
         angle -= 360 * 16;
     return angle;
}



GLWidget::GLWidget(QWidget *parent)
        : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{

        xRot = 0;
        yRot = 0;
        zRot = 0;

        qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
        qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);

        displayList = 0;

        if (!format().depth())
                 qWarning("Could not get depth buffer; results will be shitty");

}


GLWidget::~GLWidget()
{
        makeCurrent();
        if (displayList != 0)
                glDeleteLists(displayList, 1);
}


QSize GLWidget::minimumSizeHint() const {
        return QSize(50, 50);
}


QSize GLWidget::sizeHint() const {
        return QSize(400, 400);
}


void GLWidget::setXRotation(int angle) {
      angle = normalizeAngle(angle);
      if (angle != xRot) {
          xRot = angle;
          updateGL();
      }
}
void GLWidget::setYRotation(int angle) {
      angle = normalizeAngle(angle);
      if (angle != yRot) {
          yRot = angle;
          updateGL();
      }
}
void GLWidget::setZRotation(int angle) {
      angle = normalizeAngle(angle);
      if (angle != zRot) {
          zRot = angle;
          updateGL();
      }
}


void GLWidget::initializeGL() {

        //glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        //glShadeModel(GL_SMOOTH);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        //glEnable(GL_MULTISAMPLE);
        static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

        glClearColor(0.5, 0.4, 0.3, 1.0);
}


void GLWidget::resizeGL(int width, int height) {
        glViewport(0,0, width, height);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        gluPerspective(90, width / (float)height, 1, 100);
        //glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);

        glMatrixMode(GL_MODELVIEW);
}


void GLWidget::paintGL() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();
        glTranslatef(0.0, -10.0, -30.0);
        //glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
        glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
        //glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
        glCallList(displayList);

        glBegin(GL_QUADS);
        glColor3f(1,0,0);
        glVertex3f(-10, 10, 0);
        glColor3f(0,1,0);
        glVertex3f(10, 10, 0);
        glColor3f(0,0,1);
        glVertex3f(10, -10, 0);
        glColor3f(1,0,1);
        glVertex3f(-10, -10, 0);
        glEnd();
}


void GLWidget::mousePressEvent(QMouseEvent *event) {
        lastPos = event->pos();
}


void GLWidget::mouseMoveEvent(QMouseEvent *event) {
        int dx = event->x() - lastPos.x();
        int dy = event->y() - lastPos.y();

        if (event->buttons() & Qt::LeftButton) {
                setXRotation(xRot + 8 * dy);
                setYRotation(yRot + 8 * dx);
        } else if (event->buttons() & Qt::RightButton) {
                setXRotation(xRot + 8 * dy);
                setZRotation(zRot + 8 * dx);
        }
        lastPos = event->pos();
}


void GLWidget::updateData(LSystem const &lsys, Pattern const &pat) {
        if (displayList != 0)
                glDeleteLists(displayList, 1);

        if (1) {
                {
                        GLDisplayListMesh dlm;
                        draw (lsys, pat, Turtle(), dlm);
                        displayList = dlm.result();
                }
                updateGL();
        } else {
                displayList = glGenLists(1);
                glNewList(displayList, GL_COMPILE);

                glBegin(GL_LINES);
                glColor3f(0.5,0.7,0.9);
                glVertex3f(0,0,0);
                glVertex3f(10,50,0);
                glEnd();

                glEndList();

                updateGL();
        }
}

