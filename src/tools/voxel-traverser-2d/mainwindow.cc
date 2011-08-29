//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2011  Sebastian Mach (*1983)
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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

#include <cmath>
#include <limits>


typedef float real;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

const qreal x = 100, y = 100;
const qreal w = 320, h = 320;
void drawRect (QPainter &painter, QRectF const &rect) {
        QRectF r (x+rect.left()*w, y+rect.top()*h,
                  rect.width()*w, rect.height()*h);
        painter.drawRect (r);
}
void drawLine (QPainter &painter, QPointF const &from, QPointF &to) {
        painter.drawLine(QPointF(x+from.x()*w, y+from.y()*h),
                         QPointF(x+to.x()*w, y+to.y()*h));
}
void drawHLine (QPainter &painter, QPointF const &center, float length,
                const char *title = 0)
{
        const float c_x = x+center.x()*w;
        const float c_y = y+center.y()*w;
        painter.drawLine(c_x-length/2, c_y, c_x+length/2, c_y);
        if (title) painter.drawText(c_x-40, c_y+10, QString::fromAscii(title));
}
void drawCross (QPainter &painter, QPointF const &center, float length = -1,
                const char *title = 0) {
        const float c_x = center.x()*w;
        const float c_y = center.y()*w;
        if (length < 0) {
                painter.drawLine(QPointF(x+0, y+c_x),
                                 QPointF(x+h, y+c_y));
                painter.drawLine(QPointF(x+c_x, y+0),
                                 QPointF(x+c_x, y+w));
        } else {
                painter.drawLine(QPointF(x+c_x-length/2, y+c_y-length/2),
                                 QPointF(x+c_x+length/2, y+c_y+length/2));
                painter.drawLine(QPointF(y+c_x+length/2, y+c_y-length/2),
                                 QPointF(y+c_x-length/2, y+c_y+length/2));
        }
        if (title) painter.drawText(x+c_x+10, y+c_y+5, QString::fromAscii(title));
}
void drawBall (QPainter &painter, QPointF const &center, const char *title=0) {
        const QPointF c (x+center.x()*w, y+center.y()*h);
        painter.drawEllipse(c, 4, 4);
        if (title) painter.drawText(c+QPointF(10,5), QString::fromAscii(title));
}
void fillRect (QPainter &painter, Qt::BrushStyle style,
               QRectF const &rect)
{
        painter.fillRect(QRectF(x+rect.left()*w, y+rect.top()*h,
                                rect.width()*w, rect.height()*h),
                         style);
}

bool intersect (float p_x, float p_z, float d_x, float d_z,
                QRectF const & rect,
                float &Min, float &Max)
{
        const float &top = rect.top(), &bottom = rect.bottom(), &left = rect.left(), &right = rect.right();

        const float t_left   = (left  - p_x) / d_x,
                    t_right  = (right - p_x) / d_x,
                    t_top    = (top   - p_z) / d_z,
                    t_bottom = (bottom- p_z) / d_z;

        const float z_left   = p_z + d_z * t_left,
                    z_right  = p_z + d_z * t_right,
                    x_top    = p_x + d_x * t_top,
                    x_bottom = p_x + d_x * t_bottom;

        float nearest = std::numeric_limits<float>::max();
        float farthest = -std::numeric_limits<float>::max();

        if (z_left >= top && z_left <= bottom) {
                nearest = std::min (t_left, nearest);
                farthest = std::max (t_left, farthest);
        }
        if (z_right >= top && z_right <= bottom) {
                nearest = std::min (t_right, nearest);
                farthest = std::max (t_right, farthest);
        }
        if (x_top >= left && x_top <= right) {
                nearest = std::min (t_top, nearest);
                farthest = std::max (t_top, farthest);
        }
        if (x_bottom >= left && x_bottom <= right) {
                nearest = std::min (t_bottom, nearest);
                farthest = std::max (t_bottom, farthest);
        }
        Min = nearest;
        Max = farthest;
        return Min<=Max;//nearest < std::numeric_limits<float>::max();
}

void MainWindow::paintEvent(QPaintEvent *e) {
        QPainter painter(this);
        painter.setPen(QPen());
        painter.setBrush(QBrush());
        painter.setRenderHint(QPainter::Antialiasing, true);

        const QRectF rootBox (0.25,0.25,0.5,0.5);

        drawRect(painter, rootBox);
        const int res_x_ = 8, res_z_ = res_x_;

        const real vw = rootBox.width() / res_x_,
                   vd = rootBox.height() / res_z_;
        for (int vz=0; vz<res_z_; ++vz) {
                for (int vx=0; vx<res_x_; ++vx) {
                        const real left = rootBox.left() + vw * vx;
                        const real top  = rootBox.top()  + vd * vz;
                        drawRect (painter, QRectF (left, top, vw, vd));
                }
        }

        drawLine(painter, from_, to_);
        drawCross(painter,rootBox.center());


        // setup stub ray
        const real p_x = from_.x(),
                   p_z = from_.y();
        const real diff[2] = { to_.x()-from_.x(),
                               to_.y()-from_.y() };
        const real len = std::sqrt(diff[0]*diff[0] + diff[1]*diff[1]);
        const real d_x = diff[0] / len,
                   d_z = diff[1] / len;

        real Min, Max;
        if (!intersect (p_x, p_z, d_x, d_z, rootBox, Min, Max))
                return;
        // dda
/*
        const real gridinter_x = p_x + d_x * Min,
                   gridinter_z = p_z + d_z * Min;

        const int Rx = res_x_;
        const int Rz = res_z_;
        const real width_ = right_-left_;
        const real depth_ = back_-front_;*/

}

void MainWindow::mousePressEvent(QMouseEvent *e) {
        if (e->button() == Qt::RightButton) {
                from_ = QPointF ((e->posF().x()-::x) / w,
                                 (e->posF().y()-::y) / h);
                repaint();
        }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e) {
        if (e->buttons() & Qt::LeftButton == Qt::LeftButton) {
                to_ = QPointF ((e->posF().x()-::x) / w,
                               (e->posF().y()-::y) / h);
                repaint();
        }
}
