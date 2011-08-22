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

#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

#include <cmath>
#include <limits>

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
        drawLine(painter, from_, to_);
        drawCross(painter,rootBox.center());

        const float d_x_ = to_.x() - from_.x(),
                    d_z_ = to_.y() - from_.y(),
                    l = std::sqrt(d_x_*d_x_ + d_z_*d_z_);
        const float d_x = d_x_ / l,
                    d_z = d_z_ / l;

        const float p_x = from_.x(),
                    p_z = from_.y();

        const float c_x = rootBox.center().x(),
                    c_z = rootBox.center().y();

        float t_x = (c_x - p_x) / d_x,
              t_z = (c_z - p_z) / d_z;
        //if (d_x < 0) std::swap (t_x, t_z);

        float min, max;
        if (!intersect (p_x, p_z, d_x, d_z,
                        rootBox, min, max))
        {
                return;
        }
        if (min < 0) min = 0;
        if (max > l) max = l;
        if (min > max) return;

        drawBall (painter, QPointF (p_x + d_x * min,
                                    p_z + d_z * min), "min");
        drawBall (painter, QPointF (p_x + d_x * max,
                                    p_z + d_z * max), "max");
        drawCross (painter, QPointF (p_x + d_x * t_x,
                                     p_z + d_z * t_x), 10, "t_x");
        drawCross (painter, QPointF (p_x + d_x * t_z,
                                     p_z + d_z * t_z), 10, "t_z");

        const float sub_width = rootBox.width()*0.5,
                    sub_height = rootBox.height()*0.5;

        const bool right   = d_x >= 0,
                   forward = d_z <= 0;
        //const rangeÄ
        bool children[4];

        // +---+---+
        // | 0 | 1 |
        // +---+---+
        // | 3 | 2 |
        // +---+---+
        int child_a, child_b, child_c, child_d;
        if (forward) {
                if (right) {
                        child_a = 0;
                        child_b = 1;
                        child_c = 2;
                        child_d = 3;
                } else {
                        child_a = 1;
                        child_b = 0;
                        child_c = 3;
                        child_d = 2;
                }
        } else {
                if (right) {
                        child_a = 3;
                        child_b = 2;
                        child_c = 1;
                        child_d = 0;
                } else {
                        child_a = 2;
                        child_b = 3;
                        child_c = 0;
                        child_d = 1;
                }
        }

        children[child_a] = (t_x > min)  & (t_z < t_x) & (t_z < max);
        children[child_b] = (t_x < max)  & (t_z < max);
        children[child_c] = (t_z >= t_x) & (t_z >= min) & (t_x < max);
        children[child_d] = (t_x >= min) & (t_z >= min);

        //§ TODO: generalize for all directions
        //§ TODO: set children min/max-pairs and possibly simplify conditions (if it isn't simpler, skip it)
        if (children[0]) fillRect(painter, Qt::Dense6Pattern, QRectF(rootBox.left(),
                                                                     rootBox.top(),
                                                                     sub_width,
                                                                     sub_height));
        if (children[1]) fillRect(painter, Qt::Dense6Pattern, QRectF(rootBox.center().x(),
                                                                     rootBox.top(),
                                                                     sub_width,
                                                                     sub_height));
        if (children[2]) fillRect(painter, Qt::Dense6Pattern, QRectF(rootBox.center().x(),
                                                                     rootBox.center().y(),
                                                                     sub_width,
                                                                     sub_height));
        if (children[3]) fillRect(painter, Qt::Dense6Pattern, QRectF(rootBox.left(),
                                                                     rootBox.center().y(),
                                                                     sub_width,
                                                                     sub_height));
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
