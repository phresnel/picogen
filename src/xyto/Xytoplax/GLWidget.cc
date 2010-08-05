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

GLWidget::GLWidget(QWidget *parent)
     : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
        xRot = 0;
        yRot = 0;
        zRot = 0;

        qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
        qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
}


GLWidget::~GLWidget()
{
}


QSize GLWidget::minimumSizeHint() const {
        return QSize(50, 50);
}


QSize GLWidget::sizeHint() const {
        return QSize(400, 400);
}
 /*void GLWidget::setXRotation(int angle)
  {
      qNormalizeAngle(angle);
      if (angle != xRot) {
          xRot = angle;
          emit xRotationChanged(angle);
          updateGL();
      }

  }*/


void GLWidget::initializeGL() {
        qglClearColor(qtPurple.dark());

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glShadeModel(GL_SMOOTH);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_MULTISAMPLE);
        static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}


void GLWidget::resizeGL(int width, int height) {
        int side = qMin(width, height);
        glViewport((width - side) / 2, (height - side) / 2, side, side);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
#ifdef QT_OPENGL_ES_1
        glOrthof(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#else
        glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#endif
        glMatrixMode(GL_MODELVIEW);
}


void GLWidget::paintGL() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glTranslatef(0.0, 0.0, -10.0);
        glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
        glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
        glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

        glBegin(GL_LINES);
        glVertex3f(0,0,2);
        glVertex3f(0,10,2);
        glEnd();
}


void GLWidget::mousePressEvent(QMouseEvent *event) {
        lastPos = event->pos();
}


void GLWidget::mouseMoveEvent(QMouseEvent *event) {
        int dx = event->x() - lastPos.x();
        int dy = event->y() - lastPos.y();
        Q_UNUSED(dx)
        Q_UNUSED(dy)

        if (event->buttons() & Qt::LeftButton) {
                //setXRotation(xRot + 8 * dy);
                //setYRotation(yRot + 8 * dx);
        } else if (event->buttons() & Qt::RightButton) {
                //setXRotation(xRot + 8 * dy);
                //setZRotation(zRot + 8 * dx);
        }
        lastPos = event->pos();
}

