//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
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

#include <QMouseEvent>
#include "colorwheelwidget.hh"
#include <iostream>



ColorWheelWidget::ColorWheelWidget (QWidget *parent)
: QGLWidget(QGLFormat(QGL::SampleBuffers),parent) {
        currentColor.r = 1.0f;
        currentColor.g = 0.0f;
        currentColor.b = 0.0f;
}



void ColorWheelWidget::initializeGL() {
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
        glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
        glEnable (GL_LINE_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);
        glDisable(GL_LIGHTING);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glClearColor (0.25f, 0.25f, 0.25f, 1.0f);
        glClearDepth (1.0f);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
}



void ColorWheelWidget::resizeGL(int w, int h) {
        glViewport(0, 0, (GLint)w, (GLint)h);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        gluOrtho2D(0,w,h,0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
}



void ColorWheelWidget::paintGL () {
        using std::sin; using std::cos;
        using namespace color;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        const hsvf current = rgb_to_hsv(currentColor);
        const float
                width = (float)this->width(),
                height = (float)this->height(),
                width05 = width/2.0,
                height05 = height/2.0,
                center_x = width05,
                center_y = height05,

                // TODO: unhardcode that
                outer_radius = width05<height05?width05:height05,
                inner_radius = outer_radius/1.5f,
                pi2 = 2*pi,
                perimeter = 2*pi * outer_radius,
                f_step = pi2/(.5f*perimeter)
        ;

        const QPointF center (center_x, center_y);

        // Draw hue circle.
        glBegin (GL_QUAD_STRIP);
        for (float f=0.0f; f<=pi2+f_step; f+=f_step) {
                const rgbf col = hsv_to_rgb (f * 180.f / pi, 1.0f, 1.0f);
                const QPointF
                        a = center+inner_radius*QPointF (cos(f),-sin(f)),
                        b = center+outer_radius*QPointF (cos(f),-sin(f))
                ;
                glColor3f (col.r, col.g, col.b);
                glVertex2f (a.x(), a.y());
                glVertex2f (b.x(), b.y());
        }
        glEnd();

        // mark hue in circle
        {
                const QPointF currentDir (cos(current.h * pi / 180),
                                        -sin(current.h * pi / 180));

                //p.setPen(QColor(255,255,255));
                glColor3f (1.f, 1.f, 1.f);
                glBegin (GL_LINES);
                glLineWidth(1.0f);
                const QPointF
                        inner = center+.9f*inner_radius*currentDir,
                        outer = center+1.1f*outer_radius*currentDir
                ;
                glVertex2f (inner.x(), inner.y());
                glVertex2f (outer.x(), outer.y());
                glEnd();
        }



        // Draw sv triangle.
        const QPointF hue = center+0.99*inner_radius *
                            QPointF (cos(current.h * pi / 180),
                                     -sin(current.h * pi / 180));
        const QPointF white = center+0.99*inner_radius *
                              QPointF  (cos((-120+current.h) * pi / 180),
                                        -sin((-120+current.h) * pi / 180));
        const QPointF black = center+0.99*inner_radius *
                              QPointF(cos((120+current.h) * pi / 180),
                                      -sin((120+current.h) * pi / 180));

        const color::rgbf currentFullHue = hsv_to_rgb (current.h, 1.f, 1.f);
        glBegin (GL_TRIANGLES);
        glColor3f (currentFullHue.r, currentFullHue.g, currentFullHue.b);
        glVertex2f (hue.x(), hue.y());
        glColor3f(1,1,1);
        glVertex2f (white.x(), white.y());
        glColor3f(0,0,0);
        glVertex2f (black.x(), black.y());
        glEnd();
}



void ColorWheelWidget::mouseMoveEvent(QMouseEvent* e) {
        update(e->posF());
}



void ColorWheelWidget::mousePressEvent(QMouseEvent* e) {
        update(e->posF());
}



void ColorWheelWidget::update(QPointF const & mousePosition) {
        const float
                width = this->width(),
                height = this->height(),
                width05 = width/2.0,
                height05 = height/2.0,
                center_x = width05,
                center_y = height05,
                diff_x = mousePosition.x() - center_x,
                diff_y = mousePosition.y() - center_y,
                len = sqrt (diff_x*diff_x+diff_y*diff_y),

                // TODO: unhardcode that
                outer_radius = width05<height05?width05:height05,
                inner_radius = outer_radius/1.5f
        ;

        if (len >= inner_radius && len <= outer_radius) {
                const float
                        rlen = 1.f / len,
                        n_x = diff_x*rlen,
                        n_y = diff_y*rlen,
                        alpha = (acos(n_x)*180.f/color::pi),
                        hue = n_y>0?360-alpha:alpha
                ;
                currentColor = color::hsv_to_rgb(hue, 1, 1);
                emit currentColorChanged(currentColor);
                repaint();
        }
}



void ColorWheelWidget::setCurrentColor (color::rgbf col) {
        currentColor = col;
        repaint();
}
