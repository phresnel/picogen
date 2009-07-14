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

#include <jux_gen.hh>
#include <list>
#include <QtGui>


class UpdateHeightmapMixin {
public:
        virtual void updateHeightmap () = 0;
};

class EdgeItem;
class NodeItem : public QGraphicsItem, QGraphicsItemAnimation {
public:
        class Value {
        public:
                enum PredefinedConstant {
                        Pi, e
                };

                Value ()
                : predefinedConstant (Pi)
                , floatConstant (0.0f)
                , parameter ("x")
                {}

                PredefinedConstant asPredefinedConstant () const { return predefinedConstant; }
                void setPredefinedConstant (PredefinedConstant val) { predefinedConstant = val; }

                float asFloatConstant () const { return floatConstant; }
                void setFloatConstant (float val) { floatConstant = val; }

                std::string asParameter () const { return parameter; }
                void setParameter (std::string p) { parameter = p; }

        private:
                PredefinedConstant predefinedConstant;
                double floatConstant;
                std::string parameter;
        };


        enum Type {
                Undefined,
                Parameter,
                UserConstant,
                PredefinedConstant,

                // +-*/
                Addition,
                Subtraction,
                Multiplication,
                Division,

                // ^ min max neg
                Exponentiate,
                Minimize,
                Maximize,
                Negate,

                // and or not
                //And,
                //Or,
                //Not,

                // lerp
                Lerp,

                // < <= > >= = !=

                // [] ]] ][ [[

                // inv sin cos
                Inverse,
                Sine,
                Cosine,

                // floor abs trunc frac

                // sqrt log log10 exp

                // delta

                // if

                // mulpi
                MultiplyWithPi
        };

        Type getType() const ;
        void setType(Type, bool forceReInit = false) ;

        bool isRootItem () const ;

        //NodeItem(NodeItem const &rhs) ;
        NodeItem(QGraphicsScene *scene, UpdateHeightmapMixin *updateHeightmapMixin, NodeItem *parent, NodeItem *root);
        virtual ~NodeItem();

        //NodeItem &operator = (NodeItem const &rhs) ;

        QRectF boundingRect() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        NodeItem* addChild (NodeItem *sibling=0, bool after=false, QPointF pos=QPointF());
        NodeItem* insertLeftSibling ();
        NodeItem* insertRightSibling ();
        void doLayout();

        Value getValue () const;
        void setValue (Value val);

        int getMinimumParameterCount () const ;
        int getMaximumParameterCount () const ;

        bool isChildCountOkay () const ;
        int getMissingChildrenCount () const ;

        bool isTerminal () const ;
        bool isAggregate () const ;
        bool isCompilable () const ;
        bool areChildrenCompilable () const ;

        void moveUp() ;
        void moveDown() ;
        void asParentsSibling () ;
        void asLeftSiblingsChild () ;
        void asRightSiblingsChild () ;
        void die () ;
        void swap (NodeItem &rhs) ;

        QString genJuxCode () const {
                JuxGeneratorState state;
                return genJuxCode (state);
        }
        QString genJuxCode (JuxGeneratorState &state) const ;

        QImage genHeightmap (int w, int h) const ;
        Compiler::FunctionPtr genQuatsch () const ;

protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:

        Compiler::ConfigurableFunctionsMap addfuns;

        void remove (NodeItem *node) ;

        int getParameterCount (bool getMinCount) const ;

        Value value;
        //QWidget *propertyWidget;

        QString title;
        Type type;

        void afterAnimationStep (qreal step);
        float doLayout (float const base_x, float const base_y);

        std::list<NodeItem*> children;
        std::list<EdgeItem*> edges;
        QTimeLine timer;

        // relatives
        NodeItem *parent, *root;


        UpdateHeightmapMixin *updateHeightmapMixin;


        QPixmap pixmap;

        // buttonsbbb
        /*
        QWidget *form;
        QGridLayout *layout;
        QPushButton *addChildNodeButton;
        QPushButton *addLeftSiblingNodeButton;
        QPushButton *addRightSiblingNodeButton;
        QGraphicsProxyWidget *graphicsProxyWidget;

        friend class NodeItemEventsGlue;
        NodeItemEventsGlue nodeItemEvents;*/


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


private:
        void on_addChildNodeButton_pressed ();
};


#endif // NODEITEM_HH
