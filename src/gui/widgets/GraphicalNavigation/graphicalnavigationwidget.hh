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

#ifndef GRAPHICALNAVIGATIONWIDGET_HH
#define GRAPHICALNAVIGATIONWIDGET_HH

#include <QWidget>

class QGraphicsScene;
class QGraphicsPixmapItem;

namespace Ui {
        class GraphicalNavigationWidget;
}

class GraphicalNavigationWidget : public QWidget
{
        Q_OBJECT

public:
        explicit GraphicalNavigationWidget(QWidget *parent = 0);
        ~GraphicalNavigationWidget();

private slots:
        void resizeEvent(QResizeEvent *);
        void showEvent(QShowEvent *);

private:
        Ui::GraphicalNavigationWidget *ui;
        QGraphicsScene *scene;
        QGraphicsPixmapItem *heightmapCutout;

        double fun(double u, double v) const;
        QPixmap pixmapFromFun() const;
};

#endif // GRAPHICALNAVIGATIONWIDGET_HH
