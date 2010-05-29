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

#ifndef NODEITEM_HH
#define NODEITEM_HH

#include "kallisto/common.hh"
#include "redshift/include/tuple.hh"
#include "actuarius/actuarius.hh"
#include "../../common/include/quatschnode.hh"

#include <list>
#include <QtGui>

class EdgeItem;
class NodeItem;

class UpdateHeightmapMixin {
public:
        virtual void updateHeightmap () = 0;
};

enum DropType {
        SetType,
        AddChild,
        InsertLeftSibling,
        InsertRightSibling
};

class FindDropNodeMixin {
public:
        virtual redshift::tuple<NodeItem *, DropType>
          findDropNode (QPointF const &point, NodeItem *exclude=0) const = 0;
};




struct QuatschNode;
class NodeItem : public QGraphicsItem, QGraphicsItemAnimation {
public:
        QuatschNodeType getType() const ;
        void setType(QuatschNodeType, bool forceReInit = false) ;

        bool isRootItem () const ;

        //NodeItem(NodeItem const &rhs) ;
        NodeItem(QGraphicsScene *scene, UpdateHeightmapMixin *updateHeightmapMixin, FindDropNodeMixin *findDropNodeMixin, NodeItem *parent, NodeItem *root);
        virtual ~NodeItem();

        NodeItem &operator = (NodeItem const &rhs) ;
        NodeItem (NodeItem const &rhs) ;

        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        NodeItem* addChild (NodeItem *sibling=0, bool after=false, QPointF pos=QPointF());
        NodeItem* insertLeftSibling ();
        NodeItem* insertRightSibling ();
        void doLayout();

        void select();

        enum HighlightRegion {
                Complete,
                Top,
                Bottom,
                Right
        };
        void highlight (HighlightRegion, bool clearOthers=true);
        void clearHighlight (bool clearOthers=true);

        QuatschNodeValue getValue () const;
        void setValue (QuatschNodeValue val);
        void updateHeightmap ();
        void updateTitle();
        void setEnableAutoUpdateHeightmap (bool enable);

        void moveUp() ;
        void moveDown() ;
        void asParentsSibling () ;
        void asLeftSiblingsChild () ;
        void asRightSiblingsChild () ;
        void asLeftSiblingOf (NodeItem *sibl);
        void asRightSiblingOf (NodeItem *sibl);
        void asChildOf (NodeItem *sibl);
        void die () ;
        void killChildren () ;
        void swap (NodeItem &rhs) ;

        QString genJuxCode () const {
                JuxGeneratorState state;
                return genJuxCode (state);
        }
        QString genJuxCode (JuxGeneratorState &state) const {
                QuatschNode qn (*this);
                return QString::fromStdString(qn.genJuxCode(state));
        }
        Compiler::FunctionPtr genQuatsch () const ;

        QImage genHeightmap (int w, int h) const ;
        NodeItem & operator = (QuatschNode const &rhs);

protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
        QPointF dragMouseCenterOffset;
        QPoint mouseStartDragScreenPos;
        bool isDragging;
        bool enableAutoUpdateHeightmap;

        void remove (NodeItem *node) ;
        void afterAnimationStep (qreal step);
        float doLayout (float const base_x, float const base_y);
        void move (float ofs_x, float ofs_y);
        void move (QPointF const &ofs);
        void updateEdgeItems ();

        QuatschNodeValue value;
        //QWidget *propertyWidget;

        QString title;
        bool isHighlighted;
        HighlightRegion highlightRegion;

        std::list<NodeItem*> children;
        std::list<EdgeItem*> edges;
        QTimeLine timer;
        NodeItem *parent, *root;
        QuatschNodeType type;
        UpdateHeightmapMixin *updateHeightmapMixin;
        FindDropNodeMixin *findDropNodeMixin;
        QPixmap pixmap;

        class NodeException: public std::exception {
        public:
                NodeException (std::string msg_) : msg(msg_) {}
                virtual ~NodeException() throw() {}
                const char* what() const throw() {
                        return msg.c_str();
                }
        private:
                std::string msg;
        };

        friend struct QuatschNode;
public:
        void serialize (std::string name, std::ostream &out);
        void deserialize (std::string name, std::istream &in);


private:
        void on_addChildNodeButton_pressed ();
};




#endif // NODEITEM_HH
