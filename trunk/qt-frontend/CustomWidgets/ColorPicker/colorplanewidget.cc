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
#include "colorplanewidget.hh"
#include <iostream>



ColorPlaneWidget::ColorPlaneWidget (QWidget *parent)
: QGLWidget(QGLFormat(QGL::SampleBuffers),parent) {
        currentColor.r = 1.0f;
        currentColor.g = 0.0f;
        currentColor.b = 0.0f;
}



void ColorPlaneWidget::initializeGL() {
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



void ColorPlaneWidget::resizeGL(int w, int h) {
        glViewport(0, 0, (GLint)w, (GLint)h);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        gluOrtho2D(0,w,h,0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
}



void ColorPlaneWidget::paintGL () {
        using std::sin; using std::cos;
        using namespace color;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
}



void ColorPlaneWidget::mouseMoveEvent(QMouseEvent* e) {
        update(e->posF());
}



void ColorPlaneWidget::mousePressEvent(QMouseEvent* e) {
        update(e->posF());
}



void ColorPlaneWidget::update(QPointF const & mousePosition) {
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
                repaint();
        }
}
