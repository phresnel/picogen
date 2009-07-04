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

#ifndef QTQUATSCHEDITOR_HH
#define QTQUATSCHEDITOR_HH

#include <QtGui/QWidget>
#include <QtGui>
#include <QtGui/QMainWindow>
#include <QtOpenGL>

#include <userconstant.hh>
#include <predefinedconstant.hh>
#include <parameter.hh>

#include <edgeitem.hh>
#include <nodeitem.hh>

namespace Ui
{
    class QtQuatschEditor;
}


//class GLWidget ;
#include "../glwidget.h"

class QtQuatschEditor : public QWidget, public UpdateHeightmapMixin
{
        Q_OBJECT

public:
        QtQuatschEditor(QWidget *parent = 0);
        ~QtQuatschEditor();

        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void wheelEvent(QWheelEvent*);

        void keyPressEvent(QKeyEvent*);
        void keyReleaseEvent(QKeyEvent*);

        void updateHeightmap ();

private:
        Ui::QtQuatschEditor *ui;
        NodeItem *rootNode;
        QWidget *currentPropertyWidget;

        void displayPropertyWindow ();
        void updateHeightmap (NodeItem *node);
        void drawHeightmap3d (QImage const &heightmap);

        QPixmap heightmap;

        GLWidget *glWidget;

private slots:
        void on_asRightSiblingsChildButton_clicked();
        void on_asLeftSiblingsChildButton_clicked();
        void on_asParentsSiblingButton_clicked();
        void on_moveNodeDownButton_clicked();
        void on_moveNodeUpButton_clicked();
        void on_pushButton_5_clicked();
        void on_deleteNodeButton_clicked();
        void on_centerOnSelectedButton_clicked();
        void on_completeViewButton_clicked();
        void on_typeCombo_currentIndexChanged(int index);
        void on_addNodeButton_clicked();
        void on_insertLeftSiblingButton_clicked();
        void on_insertRightSiblingButton_clicked();
        void on_graphicsScene_selectionChanged();
};


class GLGraphicsScene : public QGraphicsScene {
        Q_OBJECT
public:
        GLGraphicsScene (QWidget *parent = 0) ;
        void drawBackground( QPainter* painter, const QRectF & rect );
        QRectF itemsBoundingRect () const ;
};


#endif // QTQUATSCHEDITOR_HH
