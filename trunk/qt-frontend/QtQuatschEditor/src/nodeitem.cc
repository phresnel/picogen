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
        NodeItem *parent_,
        NodeItem *root_
)
: /*nodeItemEvents(*this),*/ parent(parent_)
, root(root_)
, type(Undefined)
, updateHeightmapMixin (updateHeightmapMixin_)
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

        setType (type, true);
        clearHighlight();


        quatsch::ICreateConfigurableFunction<Function>::ConfigurableFunctionDescriptionPtr noiseDesc (
                new quatsch::CreateConfigurableFunction <
                        quatsch :: configurable_functions :: Noise2d <Function, Compiler>,
                        Function
                >
        );
        quatsch::ICreateConfigurableFunction<Function>::ConfigurableFunctionDescriptionPtr layeredNoiseDesc (
                new quatsch::CreateConfigurableFunction <
                        quatsch :: configurable_functions :: LayeredNoise2d <Function, Compiler>,
                        Function
                >
        );

        addfuns.addSymbol ("Noise2d", noiseDesc);
        addfuns.addSymbol ("LayeredNoise2d", layeredNoiseDesc);

        // create form elems
        /*
        layout = new QGridLayout;

        form = new QWidget;
        form->setLayout(layout);

        addChildNodeButton = new QPushButton;
        layout->addWidget(addChildNodeButton, 1, 3);

        addLeftSiblingNodeButton = new QPushButton;
        layout->addWidget(addLeftSiblingNodeButton, 0, 2);

        addRightSiblingNodeButton = new QPushButton;
        layout->addWidget(addRightSiblingNodeButton, 2, 2);

        graphicsProxyWidget = scene->addWidget(form);
        graphicsProxyWidget->setZValue(2.0);

        QObject::connect(addChildNodeButton, SIGNAL(clicked(bool)),
             &nodeItemEvents, SLOT(on_addChildNodeButton_pressed()));*/
}



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



void NodeItem::moveUp () {
        if (0 != parent) {
                std::list<NodeItem*>::iterator a =
                        find (parent->children.begin(), parent->children.end(), this);
                std::list<NodeItem*>::iterator b =
                        --find (parent->children.begin(), parent->children.end(), this);
                if (b != parent->children.end()) {
                        NodeItem *tmp = *a;
                        *a = *b;
                        *b = tmp;
                        doLayout();
                }
        }
}



void NodeItem::moveDown () {
        if (0 != parent) {
                std::list<NodeItem*>::iterator a =
                        find (parent->children.begin(), parent->children.end(), this);
                std::list<NodeItem*>::iterator b =
                        ++find (parent->children.begin(), parent->children.end(), this);
                if (b != parent->children.end()) {
                        NodeItem *tmp = *a;
                        *a = *b;
                        *b = tmp;
                        doLayout();
                }
        }
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

                        (*leftSibling)->edges.push_back(new EdgeItem ((*leftSibling), this));
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

                        (*leftSibling)->edges.push_back(new EdgeItem ((*leftSibling), this));
                        scene()->addItem((*leftSibling)->edges.back());

                        parent = *leftSibling;
                        doLayout();
                }
        }
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



NodeItem::Type NodeItem::getType() const {
        return type;
}



void NodeItem::setType(NodeItem::Type type, bool forceReInit) {
        if (!forceReInit && type == this->type)
                return;
        this->type = type;

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
                title = "Exponentiate";
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

        scene()->invalidate();
}



void NodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        QString errorMessage;
        const bool isCompilable = this->isCompilable();
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
                const int missingChildrenCount = getMissingChildrenCount();
                if (!areChildrenCompilable()) {
                        errorMessage = "error in children!";
                } else if (isTerminal() && children.size() != 0) {
                        errorMessage = "this type -> no children!";
                } else if (isAggregate() && missingChildrenCount != 0) {
                        if (missingChildrenCount < 0) {
                                errorMessage = "has "
                                        + QString::number(-missingChildrenCount)
                                        + " more children than allowed";
                        } else {
                                errorMessage = "needs "
                                        + QString::number(missingChildrenCount)
                                        + " more children";
                        }
                        if (hasDefaultParameters()) {
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
                case Value::Pi: painter->drawText (10,40,"Pi = 3.141..."); break;
                case Value::e: painter->drawText (10,40,"e = 2.718..."); break;
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
        scene()->clearSelection();
        setSelected (true);
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
        if (event->button() != Qt::LeftButton) {
                event->ignore();
                return;
        }
        setCursor(Qt::ClosedHandCursor);
        select();
}



void NodeItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
        Q_UNUSED(event)
        setCursor(Qt::ClosedHandCursor);
        scene()->invalidate();
}



void NodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
        Q_UNUSED(event)
        setCursor(Qt::OpenHandCursor);
        scene()->invalidate();
}



NodeItem* NodeItem::addChild (NodeItem *sibling, bool after, QPointF pos) {
        NodeItem *newItem =
          *children.insert (
            !after
              ? std::find (children.begin(), children.end(), sibling)
              : ++std::find (children.begin(), children.end(), sibling),
            new NodeItem(scene(), updateHeightmapMixin, this, parent == 0 ? this: root)
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



void NodeItem::doLayout() {
        if (0==parent) {
                doLayout(0.0f, 0.0f);
                QRectF rect = scene()->itemsBoundingRect();
                scene()->views()[0]->setSceneRect(rect);
                scene()->invalidate();
        } else {
                root->doLayout ();
        }
}



float NodeItem::doLayout (float const base_x, float const base_y) {
        const float
                width = boundingRect().width(),
                height = boundingRect().height(),
                e_x = 70.00f,
                e_y = 40.00f
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



NodeItem::Value NodeItem::getValue () const {
        /*std::cout << "getValue:\nfilter: " << value.asNoise2d().filter << std::endl;
        std::cout << "seed:   " << value.asNoise2d().seed << std::endl;
        std::cout << "width:  " << value.asNoise2d().width << std::endl;*/
        return value;
}



void NodeItem::setValue (NodeItem::Value val) {
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



bool NodeItem::isTerminal () const {
        // Note: isTerminal() is not necessarily !isAggregate(), in case getType() is Undefined
        switch (getType()) {
        case PredefinedConstant:
        case UserConstant:
        case Parameter:
                return true;

        case Undefined:
        case Addition:
        case Subtraction:
        case Multiplication:
        case Division:
        case Exponentiate:
        case Minimize:
        case Maximize:
        case Negate:
        case Lerp:
        case Inverse:
        case Sine:
        case Cosine:
        case Noise2d:
        case LayeredNoise2d:
        case MultiplyWithPi:
                return false;
        };
        throw NodeException ("switch() in NodeItem::isTerminal() is incomplete");
}



bool NodeItem::isAggregate () const {
        // Note: isTerminal() is not necessarily !isAggregate(), in case getType() is Undefined
        switch (getType()) {
        case PredefinedConstant:
        case UserConstant:
        case Parameter:
        case Undefined:
                return false;

        case Addition:
        case Subtraction:
        case Multiplication:
        case Division:
        case Exponentiate:
        case Minimize:
        case Maximize:
        case Negate:
        case Lerp:
        case Inverse:
        case Sine:
        case Cosine:
        case Noise2d:
        case LayeredNoise2d:
        case MultiplyWithPi:
                return true;
        };
        throw NodeException ("switch() in NodeItem::isAggregate() is incomplete");
}



bool NodeItem::hasDefaultParameters () const {
        switch (getType()) {
        case PredefinedConstant:
        case UserConstant:
        case Parameter:
        case Undefined:
        case Addition:
        case Subtraction:
        case Multiplication:
        case Division:
        case Exponentiate:
        case Minimize:
        case Maximize:
        case Negate:
        case Lerp:
        case Inverse:
        case Sine:
        case Cosine:
        case MultiplyWithPi:
                return false;

        case Noise2d:
        case LayeredNoise2d:
                return true;
        };
        throw NodeException ("switch() in NodeItem::isAggregate() is incomplete");
}



QString NodeItem::getDefaultParameters () const {
        switch (getType()) {
        case PredefinedConstant:
        case UserConstant:
        case Parameter:
        case Undefined:
        case Addition:
        case Subtraction:
        case Multiplication:
        case Division:
        case Exponentiate:
        case Minimize:
        case Maximize:
        case Negate:
        case Lerp:
        case Inverse:
        case Sine:
        case Cosine:
        case MultiplyWithPi:
                return false;

        case Noise2d:
        case LayeredNoise2d:
                return "x y";
        };
        throw NodeException ("switch() in NodeItem::isAggregate() is incomplete");
}



int NodeItem::getMinimumParameterCount () const {
        return getParameterCount (true);
}



int NodeItem::getMaximumParameterCount () const {
        return getParameterCount (false);
}



int NodeItem::getParameterCount (bool getMinCount) const {
        switch (getType()) {
        case PredefinedConstant:
        case UserConstant:
        case Parameter:
                return 0;

        case Undefined:
                return -1;

        case Addition:
                if (getMinCount) return 2;
                else return -1;
        case Subtraction:
                if (getMinCount) return 2;
                else return -1;
        case Multiplication:
                if (getMinCount) return 2;
                else return -1;
        case Division:
                if (getMinCount) return 2;
                else return -1;

        case Exponentiate:
                if (getMinCount) return 2;
                else return -1;
        case Minimize:
                if (getMinCount) return 1;
                else return -1;
        case Maximize:
                if (getMinCount) return 1;
                else return -1;
        case Negate:
                if (getMinCount) return 1;
                else return 1;

        case Lerp:
                if (getMinCount) return 2;
                else return -1;

        case Inverse:
                if (getMinCount) return 1;
                else return 1;
        case Sine:
                if (getMinCount) return 1;
                else return 1;
        case Cosine:
                if (getMinCount) return 1;
                else return 1;

        case Noise2d:
                if (getMinCount) return 2;
                else return 2;
        case LayeredNoise2d:
                if (getMinCount) return 2;
                else return 2;

        case MultiplyWithPi:
                if (getMinCount) return 1;
                else return 1;
        };
        throw NodeException ("switch() in NodeItem::getParameterCount() is incomplete");
}



bool NodeItem::isChildCountOkay () const {
        if (isTerminal() && children.size() == 0)
                return true;
        if (hasDefaultParameters() && children.size() == 0)
                return true;
        if (isAggregate()) {
                const int
                        minpc = getMinimumParameterCount(),
                        maxpc = getMaximumParameterCount(),
                        cc = children.size(),
                        min_ok = (minpc == -1) || (cc >= minpc),
                        max_ok = (maxpc == -1) || (cc <= maxpc),
                        both_ok = min_ok && max_ok
                ;
                if (both_ok)
                        return true;
        }
        return false;
}



bool NodeItem::areChildrenCompilable () const {
        using std::list;
        for (list<NodeItem*>::const_iterator it=children.begin();
             it!=children.end();
             ++it)
        {
                if (!(*it)->isCompilable())
                        return false;
        }
        return true;
}



int NodeItem::getMissingChildrenCount () const {
        if (isAggregate()) {
                const int cs = static_cast<int>(children.size());

                if (hasDefaultParameters() && cs == 0)
                        return 0;

                if (-1 != getMinimumParameterCount()
                &&  cs < getMinimumParameterCount()) {
                        return getMinimumParameterCount() - cs;
                }

                if (-1 != getMaximumParameterCount()
                && cs > getMaximumParameterCount()) {
                        return getMaximumParameterCount() - cs;
                }
        }
        return 0;
}



bool NodeItem::isCompilable () const {
        if (isChildCountOkay() && areChildrenCompilable())
                return true;
        return false;
}



/*void NodeItemEventsGlue::on_addChildNodeButton_pressed() {
        self.on_addChildNodeButton_pressed ();
}*/
void NodeItem::on_addChildNodeButton_pressed() {
        addChild();
}



QString NodeItem::genJuxCode (JuxGeneratorState &state) const {
        //return "([Noise2d frequency{10} filter{nearest} seed{41}] x y)";
        JuxGeneratorState::Frame frame = state.getFrame();
        const QString indent = state.getIndendationString();

        QString tmp;

        switch (getType()) {
        case PredefinedConstant:
                switch (value.asPredefinedConstant()) {
                case Value::Pi: return indent + "3.1415926535897932385/*pi*/\n";
                case Value::e:  return indent + "2.7182818284590452355/*e*/\n";
                };

        case UserConstant:
                return indent + QString::number(value.asFloatConstant()) + "\n";

        case Parameter:
                return indent + QString::fromStdString(value.asParameter()) + "\n";

        case Undefined:

        case Addition:
                tmp = indent + "( +\n";
                goto aggregate;
        case Subtraction:
                tmp = indent + "( -\n";
                goto aggregate;
        case Multiplication:
                tmp = indent + "( *\n";
                goto aggregate;
        case Division:
                tmp = indent + "( /\n";
                goto aggregate;

        case Exponentiate:
                tmp = indent + "( ^ \n";
                goto aggregate;
        case Minimize:
                tmp = indent + "( min \n";
                goto aggregate;
        case Maximize:
                tmp = indent + "( max \n";
                goto aggregate;
        case Negate:
                tmp = indent + "( neg \n";
                goto aggregate;

        case Lerp:
                tmp = indent + "( lerp \n";
                goto aggregate;

        case Inverse:
                tmp = indent + "( inv\n";
                goto aggregate;
        case Sine:
                tmp = indent + "( sin\n";
                goto aggregate;
        case Cosine:
                tmp = indent + "( cos\n";
                goto aggregate;

        case Noise2d:
                tmp = indent + "( [Noise2d " +
                      "frequency{" + QString::number(value.asNoise2d().width) + "} " +
                      "seed{" + QString::number(value.asNoise2d().seed) + "}";
                switch (value.asNoise2d().filter) {
                case Value::Noise2d::Nearest: tmp += "filter{nearest}"; break;
                case Value::Noise2d::Bilinear: tmp += "filter{bilinear}"; break;
                case Value::Noise2d::Cosine: tmp += "filter{cosine}"; break;
                };
                tmp += "] \n";
                goto aggregate;
        case LayeredNoise2d:
                tmp = indent + "( [LayeredNoise2d " +
                      "frequency{" + QString::number(value.asLayeredNoise2d().width) + "} " +
                      "seed{" + QString::number(value.asLayeredNoise2d().seed) + "}"
                      "persistence{0.5}" // + QString::number(value.asNoise2d().seed) + "}"
                      "layercount{" + QString::number(value.asLayeredNoise2d().depth) + "}"
                ;
                switch (value.asLayeredNoise2d().filter) {
                case Value::LayeredNoise2d::Nearest: tmp += "filter{nearest}"; break;
                case Value::LayeredNoise2d::Bilinear: tmp += "filter{bilinear}"; break;
                case Value::LayeredNoise2d::Cosine: tmp += "filter{cosine}"; break;
                };
                tmp += "] \n";
                goto aggregate;

        case MultiplyWithPi:
                tmp = indent + "( *\n"
                    + indent + indent + "3.1415926535897932385/*pi*/\n";
                goto aggregate;
        aggregate:

                if (hasDefaultParameters() && children.size() == 0) {
                       tmp += getDefaultParameters();
                } else for (std::list<NodeItem*>::const_iterator it=children.begin();
                     it != children.end();
                     ++it
                ) {
                        tmp += (*it)->genJuxCode (state);
                }

                tmp += indent + ")\n";
                return tmp;
        };
        return "<err>";
        throw NodeException ("switch() in NodeItem::genJuxCode() is incomplete");
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
