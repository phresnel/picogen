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

#include "nodeitem.hh"
#include <edgeitem.hh>

NodeItem::NodeItem(
        QGraphicsScene *scene,
        UpdateHeightmapMixin *updateHeightmapMixin_,
        FindDropNodeMixin *findDropNodeMixin_,
        NodeItem *parent_,
        NodeItem *root_
)
: /*nodeItemEvents(*this),*/ parent(parent_)
, root(root_)
, type(Undefined)
, updateHeightmapMixin (updateHeightmapMixin_)
, findDropNodeMixin (findDropNodeMixin_)
{
        scene->addItem (this);

        QGraphicsItemAnimation::setItem (this);
        QGraphicsItemAnimation::setTimeLine(&timer);
        timer.setFrameRange(0, 1000);

        setZValue(1.0f);
        //setCacheMode (QGraphicsItem::DeviceCoordinateCache);
        setToolTip("foobar!");
        setCursor(Qt::OpenHandCursor);

        setFlag (QGraphicsItem::ItemIsSelectable, true);
        //setFlag (QGraphicsItem::ItemIsMovable, true);

        setType (type, true);
        clearHighlight();


        typedef quatsch::ICreateConfigurableFunction<Function> ccf;
        typedef ccf::ConfigurableFunctionDescriptionPtr ccf_ptr;

        ccf_ptr
                noiseDesc (
                        new quatsch::CreateConfigurableFunction <
                                quatsch::configurable_functions::Noise2d <
                                        Function, Compiler>,
                                Function
                        >
                ),
                layeredNoiseDesc (
                        new quatsch::CreateConfigurableFunction <
                                quatsch::configurable_functions::LayeredNoise2d<
                                        Function, Compiler>,
                                Function
                        >
                )
        ;

        addfuns.addSymbol ("Noise2d", noiseDesc);
        addfuns.addSymbol ("LayeredNoise2d", layeredNoiseDesc);
}


/*
NodeItem &NodeItem::operator = (NodeItem const &rhs) {
        return *this;
}



NodeItem::NodeItem (NodeItem const &rhs)
: addfuns(rhs.addfuns)
, QGraphicsItem (rhs)
{
        rhs.scene()->addItem (this);

        QGraphicsItemAnimation::setItem (this);
        QGraphicsItemAnimation::setTimeLine(&timer);
        timer.setFrameRange(0, 1000);

        setZValue(1.0f);
        //setCacheMode (QGraphicsItem::DeviceCoordinateCache);
        setToolTip("foobar!");
        setCursor(Qt::OpenHandCursor);

        setFlag (QGraphicsItem::ItemIsSelectable, true);
        //setFlag (QGraphicsItem::ItemIsMovable, true);

        setType (type, true);
        clearHighlight();
}*/



NodeItem::~NodeItem () {
        scene()->removeItem(this);

        for (std::list<NodeItem*>::iterator it=children.begin();
             it!=children.end();
             ++it
        ) {
                delete *it;
        }
        children.clear();

        for (std::list<EdgeItem*>::iterator it=edges.begin();
             it!=edges.end();
             ++it
        ) {
                delete *it;
        }
        edges.clear();

        parent = root = 0;
}



void NodeItem::die () {
        if (0 != parent)
                parent->remove (this);
}



void NodeItem::asChildOf (NodeItem *newParent) {
        if (newParent == parent) {
                doLayout();
                return;
        }

        parent->children.remove(this);
        newParent->children.push_back(this);

        for (std::list<EdgeItem*>::iterator it=parent->edges.begin();
             it!=parent->edges.end();
             ++it
        ) {
                if ((*it)->isConnectedTo(parent)
                 && (*it)->isConnectedTo(this)
                ) {
                        parent->edges.remove(*it);
                        delete *it;
                        break;
                }
        }

        parent = newParent;

        parent->edges.push_back(new EdgeItem (parent, this));
        scene()->addItem(parent->edges.back());

        doLayout();
}



void NodeItem::asLeftSiblingOf(NodeItem *sibl) {

        /*if (sibl->parent == parent) {
                moveUp();
                return;
        }*/

        NodeItem *newParent = sibl->parent;
        if (newParent == 0)
                return;

        parent->children.remove(this);
        std::list<NodeItem*>::iterator insertPos =
                find (newParent->children.begin(),
                      newParent->children.end(),
                      sibl);

        newParent->children.insert(insertPos, this);


        for (std::list<EdgeItem*>::iterator it=parent->edges.begin();
             it!=parent->edges.end();
             ++it
        ) {
                if ((*it)->isConnectedTo(parent)
                 && (*it)->isConnectedTo(this)
                ) {
                        parent->edges.remove(*it);
                        delete *it;
                        break;
                }
        }

        parent = newParent;

        parent->edges.push_back(new EdgeItem (parent, this));
        scene()->addItem(parent->edges.back());

        doLayout();
}



void NodeItem::asRightSiblingOf(NodeItem *sibl) {

        /*if (sibl->parent == parent) {
                moveDown();
                return;
        }*/

        NodeItem *newParent = sibl->parent;
        if (newParent == 0)
                return;

        parent->children.remove(this);
        std::list<NodeItem*>::iterator insertPos =
                ++find (newParent->children.begin(),
                      newParent->children.end(),
                      sibl);

        newParent->children.insert(insertPos, this);

        for (std::list<EdgeItem*>::iterator it=parent->edges.begin();
             it!=parent->edges.end();
             ++it
        ) {
                if ((*it)->isConnectedTo(parent)
                 && (*it)->isConnectedTo(this)
                ) {
                        parent->edges.remove(*it);
                        delete *it;
                        break;
                }
        }

        parent = newParent;

        parent->edges.push_back(new EdgeItem (parent, this));
        scene()->addItem(parent->edges.back());

        doLayout();
}



void NodeItem::moveUp () {
        if (0 != parent) {
                std::list<NodeItem*>::iterator a =
                        find (parent->children.begin(),
                              parent->children.end(),
                              this);
                std::list<NodeItem*>::iterator b =
                        --find (parent->children.begin(),
                                parent->children.end(),
                                this);
                if (b != parent->children.end()) {
                        NodeItem *tmp = *a;
                        *a = *b;
                        *b = tmp;
                }
        }
        doLayout();
}



void NodeItem::moveDown () {
        if (0 != parent) {
                std::list<NodeItem*>::iterator a =
                        find (parent->children.begin(),
                              parent->children.end(),
                              this);
                std::list<NodeItem*>::iterator b =
                        ++find (parent->children.begin(),
                                parent->children.end(),
                                this);
                if (b != parent->children.end()) {
                        NodeItem *tmp = *a;
                        *a = *b;
                        *b = tmp;
                }
        }
        doLayout();
}



void NodeItem::asParentsSibling () {
        if (0 != parent && 0 != parent->parent) {
                parent->children.remove(this);
                parent->parent->children.insert(
                        find (parent->parent->children.begin(),
                              parent->parent->children.end(),
                              parent),
                        this);

                for (std::list<EdgeItem*>::iterator it=parent->edges.begin();
                     it!=parent->edges.end();
                     ++it
                ) {
                        if ((*it)->isConnectedTo(parent)
                         && (*it)->isConnectedTo(this)
                        ) {
                                parent->edges.remove(*it);
                                delete *it;
                                break;
                        }
                }


                parent = parent->parent;

                parent->edges.push_back(new EdgeItem (parent, this));
                scene()->addItem(parent->edges.back());

                doLayout();
        }
}



void NodeItem::asLeftSiblingsChild () {
        if (0 != parent) {
                std::list<NodeItem*>::iterator leftSibling =
                        --find(parent->children.begin(),
                               parent->children.end(),
                               this);
                if (leftSibling != parent->children.end()) {
                        parent->children.remove(this);
                        (*leftSibling)->children.push_back(this);

                        typedef std::list<EdgeItem*>::iterator edge_item_it;

                        for (edge_item_it it=parent->edges.begin();
                             it!=parent->edges.end();
                             ++it
                        ) {
                                if ((*it)->isConnectedTo(parent)
                                 && (*it)->isConnectedTo(this)
                                ) {
                                        parent->edges.remove(*it);
                                        delete *it;
                                        break;
                                }
                        }

                        (*leftSibling)->edges.push_back(
                                        new EdgeItem ((*leftSibling), this));
                        scene()->addItem((*leftSibling)->edges.back());

                        parent = *leftSibling;
                        doLayout();
                }
        }
}



void NodeItem::asRightSiblingsChild () {
        if (0 != parent) {
                std::list<NodeItem*>::iterator leftSibling =
                        ++find(parent->children.begin(),
                               parent->children.end(),
                               this);
                if (leftSibling != parent->children.end()) {
                        parent->children.remove(this);
                        (*leftSibling)->children.push_back(this);

                        typedef std::list<EdgeItem*>::iterator ei_it;
                        for (ei_it it=parent->edges.begin();
                             it!=parent->edges.end();
                             ++it
                        ) {
                                if ((*it)->isConnectedTo(parent)
                                 && (*it)->isConnectedTo(this)
                                ) {
                                        parent->edges.remove(*it);
                                        delete *it;
                                        break;
                                }
                        }

                        (*leftSibling)->edges.push_back(
                                        new EdgeItem ((*leftSibling), this));
                        scene()->addItem((*leftSibling)->edges.back());

                        parent = *leftSibling;
                        doLayout();
                }
        }
}



void NodeItem::killChildren () {
        //int i = 0;
        while (children.size() > 0) {
                //std::cout << i++ << std::endl;
                remove (*children.begin());
        }
        //std::cout << "-------" << std::endl;
        //std::cout << children.size() << std::endl;
}



void NodeItem::remove (NodeItem *node) {
        for (std::list<EdgeItem*>::iterator it=edges.begin();
             it!=edges.end();
             ++it
        ) {
                if ((*it)->isConnectedTo (node)) {
                        edges.remove(*it);
                        delete *it;
                        break;
                }
        }
        children.remove(node);
        delete node;

        doLayout();
}



QRectF NodeItem::boundingRect() const {
        return QRectF(0, 0, 140, 50);
}



bool NodeItem::isRootItem () const {
        return parent == 0;
}



QuatschNodeType NodeItem::getType() const {
        return type;
}



void NodeItem::setType(QuatschNodeType type, bool forceReInit) {
        if (!forceReInit && type == this->type)
                return;
        this->type = type;
        updateTitle();
        scene()->invalidate();
}



void NodeItem::updateTitle() {
        QImage tmp;
        switch (type) {
        case Undefined:
                title = "Undefined";
                pixmap.load(":/aggregate/error.n");
                break;
        case PredefinedConstant:
                title = "Predefined Constant";
                pixmap.load(":/aggregate/predefined-constant.n");
                break;
        case UserConstant:
                title = "User Constant";
                pixmap.load(":/aggregate/user-constant.n");
                break;
        case Parameter:
                title = "Parameter";
                pixmap.load(":/aggregate/parameter.n");
                break;
        case Addition:
                title = "Addition";
                pixmap.load(":/aggregate/addition.n");
                break;
        case Subtraction:
                title = "Subtraction";
                pixmap.load(":/aggregate/subtraction.n");
                break;
        case Multiplication:
                title = "Multiplication";
                pixmap.load(":/aggregate/multiplication.n");
                break;
        case Division:
                title = "Division";
                pixmap.load(":/aggregate/division.n");
                break;
        case Exponentiate:
                title = "Exponentiate (x^y)";
                pixmap.load(":/aggregate/exponentiate.n");
                break;
        case Minimize:
                title = "Minimize";
                pixmap.load(":/aggregate/minimize.n");
                break;
        case Maximize:
                title = "Maximize";
                pixmap.load(":/aggregate/maximize.n");
                break;
        case Negate:
                title = "Negate";
                pixmap.load(":/aggregate/negate.n");
                break;
        case Lerp:
                title = "Lerp";
                pixmap.load(":/aggregate/lerp.n");
                break;
        case And:
                title = "And";
                pixmap.load(":/aggregate/and.n");
                break;
        case Or:
                title = "Or";
                pixmap.load(":/aggregate/or.n");
                break;
        case Not:
                title = "Not";
                pixmap.load(":/aggregate/not.n");
                break;
        case LessThan:
                title = "LessThan";
                pixmap.load(":/aggregate/lessthan.n");
                break;
        case LessThanOrEqual:
                title = "LessThanOrEqual";
                pixmap.load(":/aggregate/lessthanorequal.n");
                break;
        case GreaterThan:
                title = "GreaterThan";
                pixmap.load(":/aggregate/greaterthan.n");
                break;
        case GreaterThanOrEqual:
                title = "GreaterThanOrEqual";
                pixmap.load(":/aggregate/greaterthanorequal.n");
                break;
        case Inverse:
                title = "1 / ...";
                pixmap.load(":/aggregate/inverse.n");
                break;
        case Sine:
                title = "sin(...)";
                pixmap.load(":/aggregate/sine.n");
                break;
        case Cosine:
                title = "cos(...)";
                pixmap.load(":/aggregate/cosine.n");
                break;

        case Floor:
                title = "floor";
                pixmap.load(":/aggregate/floor.n");
                break;
        case Absolute:
                title = "absolute";
                pixmap.load(":/aggregate/absolute.n");
                break;
        case Truncate:
                title = "truncate";
                pixmap.load(":/aggregate/truncate.n");
                break;
        case Fractional:
                title = "fractional";
                pixmap.load(":/aggregate/fractional.n");
                break;

        case Sqrt:
                title = "fractional";
                pixmap.load(":/aggregate/sqrt.n");
                break;
        case Log:
                title = "Logarithm";
                pixmap.load(":/aggregate/log.n");
                break;
        case Log10:
                title = "Logarithm_10";
                pixmap.load(":/aggregate/log10.n");
                break;
        case Exp:
                title = "Exponentiate (exp(x))";
                pixmap.load(":/aggregate/exp.n");
                break;

        case IfThenElse:
                title = "IfThenElse";
                pixmap.load(":/aggregate/ifthenelse.n");
                break;

        case Noise2d:
                title = "Noise2d";
                pixmap.load(":/aggregate/noise2d.n");
                break;
        case LayeredNoise2d:
                title = "LayeredNoise2d";
                pixmap.load(":/aggregate/layerednoise2d.n");
                break;

        case MultiplyWithPi:
                title = "pi * ";
                pixmap.load(":/aggregate/error.n");
                break;
        };
        //pixmap = pixmap.scaledToHeight(32);
}



void NodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        QString errorMessage;
        const bool isCompilable = QuatschNode(*this).isCompilable();
        if (isCompilable) {
                if (isSelected()) {
                        painter->setPen(QPen(QBrush(QColor(190,250,150)), 2));
                        painter->setBrush(QBrush(QColor(192,210,192)));
                } else {
                        painter->setPen(QPen(QBrush(QColor(250,190,150)), 2));
                        painter->setBrush(QBrush(QColor(192,192,192)));
                }
        } else {
                // Set some error color.
                if (isSelected()) {
                        painter->setPen(QPen(QBrush(QColor(250,230,230)), 2));
                        painter->setBrush(QBrush(QColor(230,200,200)));
                } else {
                        painter->setPen(QPen(QBrush(QColor(250,100,100)), 2));
                        painter->setBrush(QBrush(QColor(210,180,180)));
                }

                // Prepare some error message. only 1 at a time to not confuse
                // the user.
                QuatschNode qn(*this);
                const int missingChildrenCount = qn.getMissingChildrenCount();
                if (qn.isTerminal() && children.size() != 0) {
                        errorMessage = "no children allowed!";
                } else if (!qn.areChildrenCompilable()) {
                        errorMessage = "error in children!";
                } else if (qn.isAggregate() && missingChildrenCount != 0) {
                        if (missingChildrenCount < 0) {
                                errorMessage = "has "
                                        + QString::number(-missingChildrenCount)
                                        + " more children than allowed";
                        } else {
                                errorMessage = "needs "
                                        + QString::number(missingChildrenCount)
                                        + " more children";
                        }
                        if (qn.hasDefaultParameters()) {
                                errorMessage += " (or 0 for default)";
                        }
                } else if (getType() == Undefined) {
                        errorMessage = "yet undefined!";
                } else {
                        errorMessage = "unknown, maybe submit a report";
                }
        }

        painter->drawRoundedRect(this->boundingRect(), 5.0, 5.0);
        painter->drawPixmap(
            boundingRect().width() - static_cast<float>(pixmap.width()*0.5f),
            boundingRect().height()*0.5-static_cast<float>(pixmap.height()/2),
            pixmap);


        if (isHighlighted) {
                painter->setPen(QPen(Qt::white, 2));
                painter->setBrush(QBrush(QColor(192,192,240)));

                QRectF rect = this->boundingRect();
                switch (highlightRegion) {
                case Complete:
                        break;
                case Right:
                        rect.setLeft(rect.right()-10.0f);
                        break;
                case Top:
                        rect.setBottom(rect.top()+10.0f);
                        break;
                case Bottom:
                        rect.setTop(rect.bottom()-10.0f);
                        break;
                };
                painter->drawRoundedRect(rect,5.0f,5.0f);
        }

        painter->setPen(QPen(Qt::white, 2));

        painter->drawText(0,10,title);

        switch (type) {
        case Undefined:
                painter->setFont(QFont(QFont().family(), 20, QFont::Black, true));
                painter->drawText(20,40,"%$#&!");
                break;
        case PredefinedConstant:
                painter->setFont(QFont(QFont().family(), 17, QFont::Black, true));
                switch (value.asPredefinedConstant()) {
                case QuatschNodeValue::Pi: painter->drawText (10,40,"Pi = 3.141..."); break;
                case QuatschNodeValue::e: painter->drawText (10,40,"e = 2.718..."); break;
                };
                break;
        case Parameter:
                painter->setFont(QFont(QFont().family(), 27, QFont::Black, true));
                painter->drawText(10,40, QString("-> ") + QString::fromStdString(value.asParameter()));
                break;
        case UserConstant:
                painter->setFont(QFont(QFont().family(), 27, QFont::Black, true));
                painter->drawText(10,40,QString::number(value.asFloatConstant()));
                break;
        case Addition:
                painter->setFont(QFont(QFont().family(), 36, QFont::Black, true));
                painter->drawText(40,40,"+");
                break;
        case Subtraction:
                painter->setFont(QFont(QFont().family(), 36, QFont::Black, true));
                painter->drawText(40,40,"-");
                break;
        case Multiplication:
                painter->setFont(QFont(QFont().family(), 36, QFont::Black, true));
                painter->drawText(40,40,"*");
                break;
        case Division:
                painter->setFont(QFont(QFont().family(), 36, QFont::Black, true));
                painter->drawText(40,40,"/");
                break;
        case Exponentiate:
                painter->setFont(QFont(QFont().family(), 32, QFont::Black, true));
                painter->drawText(40,45,"x");
                painter->setFont(QFont(QFont().family(), 23, QFont::Black, true));
                painter->drawText(65,30,"y");
                break;
        case Minimize:
                painter->setFont(QFont(QFont().family(), 20, QFont::Black, true));
                painter->drawText(10,40,"min(...)");
                break;
        case Maximize:
                painter->setFont(QFont(QFont().family(), 20, QFont::Black, true));
                painter->drawText(10,40,"max(...)");
                break;
        case Negate:
                painter->setFont(QFont(QFont().family(), 20, QFont::Black, true));
                painter->drawText(10,40,"h = -x");
                break;
        case Lerp:
                painter->setFont(QFont(QFont().family(), 15, QFont::Black, true));
                painter->drawText(10,40,"h = (1-f)*x + f*y");
                break;
        case And:
                painter->setFont(QFont(QFont().family(), 15, QFont::Black, true));
                painter->drawText(10,40, "And");
                break;
        case Or:
                painter->setFont(QFont(QFont().family(), 15, QFont::Black, true));
                painter->drawText(10,40, "Or");
                break;
        case Not:
                painter->setFont(QFont(QFont().family(), 15, QFont::Black, true));
                painter->drawText(10,40, "Not");
                break;
        case LessThan:
                painter->setFont(QFont(QFont().family(), 15, QFont::Black, true));
                painter->drawText(10,40, "x<y<...");
                break;
        case LessThanOrEqual:
                painter->setFont(QFont(QFont().family(), 15, QFont::Black, true));
                painter->drawText(10,40, "x<=y<= ...");
                break;
        case GreaterThan:
                painter->setFont(QFont(QFont().family(), 15, QFont::Black, true));
                painter->drawText(10,40, "x>y>...");
                break;
        case GreaterThanOrEqual:
                painter->setFont(QFont(QFont().family(), 15, QFont::Black, true));
                painter->drawText(10,40, "x>=y>=...");
                break;
        case Inverse:
                painter->setFont(QFont(QFont().family(), 20, QFont::Black, true));
                painter->drawText(12,40,"1 / ...");
                break;
        case Sine:
                painter->setFont(QFont(QFont().family(), 20, QFont::Black, true));
                painter->drawText(12,40,"sin(...)");
                break;
        case Cosine:
                painter->setFont(QFont(QFont().family(), 20, QFont::Black, true));
                painter->drawText(12,40,"cos(...)");
                break;

        case Floor:
                painter->setFont(QFont(QFont().family(), 15, QFont::Black, true));
                painter->drawText(10,40, "floor(x)");
                break;
        case Absolute:
                painter->setFont(QFont(QFont().family(), 20, QFont::Black, true));
                painter->drawText(12,40,"abs(x)");
                break;
        case Truncate:
                painter->setFont(QFont(QFont().family(), 20, QFont::Black, true));
                painter->drawText(12,40,"trunc(x)");
                break;
        case Fractional:
                painter->setFont(QFont(QFont().family(), 20, QFont::Black, true));
                painter->drawText(12,40,"frac(x)");
                break;

        case Sqrt:
                painter->setFont(QFont(QFont().family(), 20, QFont::Black, true));
                painter->drawText(12,40,"sqrt(x)");
                break;
        case Log:
                painter->setFont(QFont(QFont().family(), 20, QFont::Black, true));
                painter->drawText(12,40,"log(x)");
                break;
        case Log10:
                painter->setFont(QFont(QFont().family(), 20, QFont::Black, true));
                painter->drawText(12,40,"log_10(x)");
                break;
        case Exp:
                painter->setFont(QFont(QFont().family(), 20, QFont::Black, true));
                painter->drawText(12,40,"exp(x)");
                break;

        case IfThenElse:
                painter->setFont(QFont(QFont().family(), 12, QFont::Black, true));
                painter->drawText(12,40,"if(x) then(y) else(z)");
                break;

        case Noise2d:
                painter->setFont(QFont(QFont().family(), 20, QFont::Black, true));
                painter->drawText(12,40,"Noise");
                break;
        case LayeredNoise2d:
                painter->setFont(QFont(QFont().family(), 20, QFont::Black, true));
                painter->drawText(12,40,"Layer-Noise");
                break;

        case MultiplyWithPi:
                painter->setFont(QFont(QFont().family(), 20, QFont::Black, true));
                painter->drawText(12,40,"pi * ...");
                break;
        };

        if (!isCompilable) {
                painter->setFont(QFont(QFont().family(), 10, QFont::Black, true));
                painter->drawText(0, -10, QString("error: ") + errorMessage);
        }

}



void NodeItem::select() {
        setSelected (true);

        // clear all selections but our own
        QList<QGraphicsItem*> selected = scene()->selectedItems();
        for (QList<QGraphicsItem*>::iterator it = selected.begin();
             it != selected.end();
             ++it
        ) {
                if (*it != (QGraphicsItem*)this)
                        (*it)->setSelected (false);
        }
}



void NodeItem::highlight(HighlightRegion region, bool clearOthers) {
        if (clearOthers) {
                typedef QList<QGraphicsItem*> ItemList;
                ItemList items = scene()->items();

                for (ItemList::iterator it = items.begin();
                     it != items.end();
                     ++it
                ) {
                        NodeItem *item = dynamic_cast<NodeItem*>(*it);
                        if (0 != item) {
                                item->isHighlighted = false;
                        }
                }
        }
        isHighlighted = true;
        highlightRegion = region;
        scene()->invalidate();
}



void NodeItem::clearHighlight (bool clearOthers) {
        if (clearOthers) {
                typedef QList<QGraphicsItem*> ItemList;
                ItemList items = scene()->items();

                for (ItemList::iterator it = items.begin();
                     it != items.end();
                     ++it
                ) {
                        NodeItem *item = dynamic_cast<NodeItem*>(*it);
                        if (0 != item) {
                                item->isHighlighted = false;
                        }
                }
        }
        isHighlighted = false;
        scene()->invalidate();
}



void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
        if (event->button() == Qt::LeftButton) {
                setCursor(Qt::ClosedHandCursor);
                select();
                dragMouseCenterOffset = pos() - event->scenePos();
                mouseStartDragScreenPos = event->screenPos();
                isDragging = false;
        } else {
                event->ignore();
        }
}



void NodeItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
        using redshift::tuple;
        using redshift::get;

        setCursor(Qt::ClosedHandCursor);

        const QPoint
                currentScreenPos = event->screenPos(),
                mouseMoveScreenSize = currentScreenPos - mouseStartDragScreenPos
        ;
        if (sqrtf (powf (mouseMoveScreenSize.x(),2.0f) +
                   powf (mouseMoveScreenSize.y(),2.0f))
            > 20.0f)
                isDragging = true;

        // all but root node are draggable
        if (isDragging && 0 != parent) {
                move ((event->scenePos() + dragMouseCenterOffset) - scenePos());
                tuple<NodeItem*,DropType> drop =
                                findDropNodeMixin->findDropNode(
                                      event->scenePos(),
                                      this);

                NodeItem *item = get<0>(drop);
                DropType type = get<1>(drop);

                if (0 != item) switch (type) {
                case SetType:
                        item->highlight (Complete, true);
                        break;
                case AddChild:
                        item->highlight (Right, true);
                        break;
                case InsertLeftSibling:
                        item->highlight (Top, true);
                        break;
                case InsertRightSibling:
                        item->highlight (Bottom, true);
                        break;
                };
        }
        scene()->invalidate();
}



void NodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
        Q_UNUSED(event)
        setCursor(Qt::OpenHandCursor);

        clearHighlight(true);

        if (isDragging && 0 != parent) {
                using redshift::tuple;
                using redshift::get;

                tuple<NodeItem*,DropType> drop =
                                findDropNodeMixin->findDropNode(
                                      event->scenePos(),
                                      this);

                NodeItem *item = get<0>(drop);
                DropType type = get<1>(drop);

                if (0 != item) switch (type) {
                case SetType:
                        //std::cout << "none" << std::endl;
                        doLayout();
                        break;
                case AddChild:
                        //std::cout << "add child" << std::endl;
                        asChildOf (item);
                        break;
                case InsertLeftSibling:
                        //std::cout << "as left sibl" << std::endl;
                        asLeftSiblingOf (item);
                        break;
                case InsertRightSibling:
                        //std::cout << "as right sibl" << std::endl;
                        asRightSiblingOf (item);
                        break;
                };
        }

        scene()->invalidate();
}



void NodeItem::updateEdgeItems () {
        for (std::list<EdgeItem*>::iterator it = edges.begin();
             it != edges.end();
             ++it)
                (*it)->update();

        if (0 != parent) {
                for (std::list<EdgeItem*>::iterator it = parent->edges.begin();
                     it != parent->edges.end();
                     ++it)
                        (*it)->update();
        }
}



NodeItem* NodeItem::addChild (NodeItem *sibling, bool after, QPointF pos) {
        NodeItem *newItem =
          *children.insert (
            !after
              ? std::find (children.begin(), children.end(), sibling)
              : ++std::find (children.begin(), children.end(), sibling),
            new NodeItem(scene(),
                         updateHeightmapMixin, findDropNodeMixin,
                         this, parent == 0 ? this: root)
        );
        if (pos != QPointF())
                newItem->setPos (pos);
        else
                newItem->setPos (this->pos());

        edges.push_back(new EdgeItem (this, newItem));
        scene()->addItem(edges.back());

        doLayout();
        return newItem;
}



NodeItem* NodeItem::insertLeftSibling () {
        if (0 == parent)
                return 0;
        return parent->addChild(this, false, pos());
}



NodeItem* NodeItem::insertRightSibling () {
        if (0 == parent)
                return 0;
        return parent->addChild(this, true, pos());
}



void NodeItem::move (QPointF const &ofs) {
        move (ofs.x(), ofs.y());
}



void NodeItem::move (float ofs_x, float ofs_y) {
        using std::list;
        setPos (scenePos() + QPointF (ofs_x, ofs_y));
        updateEdgeItems();
        QRectF rect = scene()->itemsBoundingRect();
        scene()->views()[0]->setSceneRect(rect);

        for (list<NodeItem*>::iterator it=children.begin();
             it != children.end();
             ++it
        ) {
                (*it)->move (ofs_x, ofs_y);
        }
}



void NodeItem::doLayout() {
        if (0==parent) {
                doLayout(0.0f, 0.0f);
                QRectF rect = scene()->itemsBoundingRect();
                scene()->views()[0]->setSceneRect(rect);
                scene()->invalidate();
        } else {
                root->doLayout ();
                updateTitle();
        }
}



float NodeItem::doLayout (float const base_x, float const base_y) {
        updateTitle();
        const float
                width = boundingRect().width(),
                height = boundingRect().height(),
                e_x = 70.00f,
                e_y = 60.00f
        ;

        std::vector<float> yankees;
        float y = base_y;
        //for (size_t i=0; i<children.size(); ++i) {
        for (std::list<NodeItem*>::iterator it = children.begin();
          it != children.end(); ++it) {
                const float prev_y = y;
                y = (*it)->doLayout (base_x+width+e_x, y);
                yankees.push_back (0.5f*(y+prev_y)+height*0.5f);
        }
        if (children.size() == 0)
                y += height+e_y;
        const float centerY = 0.5f * (y + base_y);

        const bool animated = true;
        if (animated) {
                timer.stop();
                QGraphicsItemAnimation::setPosAt(0.0, pos());
                QGraphicsItemAnimation::setPosAt(1.0, QPointF(base_x, centerY));

                //for (size_t i=0; i<children.size(); ++i) {
                for (std::list<EdgeItem*>::iterator it=edges.begin();
                     it!=edges.end();
                     ++it
                ) {
                        (*it)->startAnimation();
                }
                timer.start();
        } else {
                setPos (base_x, centerY);
                for (std::list<EdgeItem*>::iterator it=edges.begin();
                     it!=edges.end();
                     ++it
                ) {
                        (*it)->update ();
                }
        }
        //graphicsProxyWidget->setPos(pos());
        return y;
}



void NodeItem::afterAnimationStep (qreal step) {
        //graphicsProxyWidget->setPos(pos());
        Q_UNUSED(step)
        QRectF rect = scene()->itemsBoundingRect();
        scene()->views()[0]->setSceneRect(rect);
        scene()->invalidate();
}



QuatschNodeValue NodeItem::getValue () const {
        /*std::cout << "getValue:\nfilter: " << value.asNoise2d().filter << std::endl;
        std::cout << "seed:   " << value.asNoise2d().seed << std::endl;
        std::cout << "width:  " << value.asNoise2d().width << std::endl;*/
        return value;
}



void NodeItem::setValue (QuatschNodeValue val) {
        value = val;
        /*std::cout << "setValue:\nfilter: " << value.asNoise2d().filter << std::endl;
        std::cout << "seed:   " << value.asNoise2d().seed << std::endl;
        std::cout << "width:  " << value.asNoise2d().width << std::endl;*/
        if (enableAutoUpdateHeightmap)
                updateHeightmap();
}



void NodeItem::updateHeightmap () {
        if (updateHeightmapMixin)
                updateHeightmapMixin->updateHeightmap();
        if (scene())
                scene()->invalidate();
}



void NodeItem::setEnableAutoUpdateHeightmap (bool enable) {
        enableAutoUpdateHeightmap = enable;
}



/*void NodeItemEventsGlue::on_addChildNodeButton_pressed() {
        self.on_addChildNodeButton_pressed ();
}*/
void NodeItem::on_addChildNodeButton_pressed() {
        addChild();
}



Compiler::FunctionPtr NodeItem::genQuatsch () const {
        JuxGeneratorState state;
        Compiler::FunctionPtr fun (Compiler::compile (
                "x;y",
                genJuxCode (state).toStdString(),
                addfuns,
                std::cerr
        ));
        return fun;
}



QImage NodeItem::genHeightmap (int w, int h) const {

        Compiler::FunctionPtr fun = genQuatsch();

        const float
                fw = static_cast<float>(w),
                fh = static_cast<float>(h)
        ;

        QImage q (w, h, QImage::Format_RGB32);
        for (int y=0; y<h; y++) for (int x=0; x<w; x++) {
                float const
                        fx = static_cast<float>(x) / fw,
                        fy = static_cast<float>(y) / fh,
                        params [] = {fx, fy},
                        h = (*fun) (params)
                ;
                int const
                        r_ = static_cast<int>(h * 255.0f),
                        r = r_<0?0:r_>255?255:r_,
                        g_ = static_cast<int>(h * 255.0f),
                        g = g_<0?0:g_>255?255:g_,
                        b_ = static_cast<int>(h * 255.0f),
                        b = b_<0?0:b_>255?255:b_
                ;
                q.setPixel(x, y, QColor(r,g,b).rgb());
        }

        return q;
}


const actuarius::Enum<QuatschNodeType> QuatschNodeTypeNames =
        (actuarius::Nvp<QuatschNodeType>(Undefined,"Undefined")
        |actuarius::Nvp<QuatschNodeType>(Parameter,"Parameter")
        |actuarius::Nvp<QuatschNodeType>(UserConstant,"UserConstant")
        |actuarius::Nvp<QuatschNodeType>(PredefinedConstant,"PredefinedConstant")

        // +-*/
        |actuarius::Nvp<QuatschNodeType>(Addition,"Addition")
        |actuarius::Nvp<QuatschNodeType>(Subtraction,"Subtraction")
        |actuarius::Nvp<QuatschNodeType>(Multiplication,"Multiplication")
        |actuarius::Nvp<QuatschNodeType>(Division,"Division")

        // ^ min max neg
        |actuarius::Nvp<QuatschNodeType>(Exponentiate,"Exponentiate")
        |actuarius::Nvp<QuatschNodeType>(Minimize,"Minimize")
        |actuarius::Nvp<QuatschNodeType>(Maximize,"Maximize")
        |actuarius::Nvp<QuatschNodeType>(Negate,"Negate")

        // lerp
        |actuarius::Nvp<QuatschNodeType>(Lerp,"Lerp")

        // and or not
        |actuarius::Nvp<QuatschNodeType>(And,"And")
        |actuarius::Nvp<QuatschNodeType>(Or,"Or")
        |actuarius::Nvp<QuatschNodeType>(Not,"Not")

        // < <= > >= = !=
        |actuarius::Nvp<QuatschNodeType>(LessThan,"LessThan")
        |actuarius::Nvp<QuatschNodeType>(LessThanOrEqual,"LessThanOrEqual")
        |actuarius::Nvp<QuatschNodeType>(GreaterThan,"GreaterThan")
        |actuarius::Nvp<QuatschNodeType>(GreaterThanOrEqual,"GreaterThanOrEqual")
        //Equal,
        //NotEqual,

        // [] ]] ][ [[

        // inv sin cos
        |actuarius::Nvp<QuatschNodeType>(Inverse,"Inverse")
        |actuarius::Nvp<QuatschNodeType>(Sine,"Sine")
        |actuarius::Nvp<QuatschNodeType>(Cosine,"Cosine")

        // floor abs trunc frac
        |actuarius::Nvp<QuatschNodeType>(Floor,"Floor")
        |actuarius::Nvp<QuatschNodeType>(Absolute,"Absolute")
        |actuarius::Nvp<QuatschNodeType>(Truncate,"Truncate")
        |actuarius::Nvp<QuatschNodeType>(Fractional,"Fractional")

        // sqrt log log10 exp
        |actuarius::Nvp<QuatschNodeType>(Sqrt,"Sqrt")
        |actuarius::Nvp<QuatschNodeType>(Log,"Log")
        |actuarius::Nvp<QuatschNodeType>(Log10,"Log10")
        |actuarius::Nvp<QuatschNodeType>(Exp,"Exp")

        // delta

        // if
        |actuarius::Nvp<QuatschNodeType>(IfThenElse,"IfThenElse")

        // [configurable]
        |actuarius::Nvp<QuatschNodeType>(Noise2d,"Noise2d")
        |actuarius::Nvp<QuatschNodeType>(LayeredNoise2d,"LayeredNoise2d")

        // mulpi
        |actuarius::Nvp<QuatschNodeType>(MultiplyWithPi,"MultiplyWithPi")
);



/*
struct QuatschNode {
        NodeItem::Type type;
        QuatschNodeValue value;

        template<typename Archive>
        void serialize (Archive &arch) {
                using actuarius::pack;

                arch & pack ("type", QuatschNodeTypeNames, type);
                arch & pack ("value", value);
        }
};

template<typename Archive>
void NodeItem::serialize (Archive &arch) {
        using actuarius::pack;
        //arch


        arch & pack ("type", QuatschNodeTypeNames, type);
        arch & pack ("value", value);

        // TODO: write small QuatschNode class or so
        if (Archive::serialize) {
                for (std::list<NodeItem*>::iterator it=this->children.begin();
                        it!=this->children.end();
                        ++it
                ) {

                }
        }
        std::vector <NodeItem> ser_children;
        for (std::list<NodeItem*>::iterator it=this->children.begin();
             it!=this->children.end();
             ++it
        ) {
                ser_children.push_back(**it);
        }

        arch & pack ("children", ser_children);

}
*/


void NodeItem::serialize (std::string name, std::ostream &os) {
        using namespace actuarius;
        QuatschNode sni(*this);
        OArchive(os) & pack (name.c_str(), sni);
}



void NodeItem::deserialize (std::string name, std::istream &is) {
        using namespace actuarius;

        QuatschNode sni(*this);
        IArchive(is) & pack (name.c_str(), sni);
        *this = sni;
        doLayout();

        // hacked
        select();
        setSelected(false);
}



NodeItem & NodeItem::operator = (QuatschNode const &rhs) {
        killChildren();

        // init node
        setType (rhs.type);
        setValue (rhs.value);

        // init children
        for (std::vector<QuatschNode>::const_iterator
                it=rhs.children.begin();
                it!=rhs.children.end();
                ++it
        ) {
                NodeItem *kid = addChild();
                *kid = *it;
        }
        return *this;
}
