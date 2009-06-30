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

#include "qtquatscheditor.hh"
#include "ui_qtquatscheditor.h"

#include <iostream>
#include <algorithm>

#include <QComboBox>


QtQuatschEditor::QtQuatschEditor(QWidget *parent)
    : QWidget(parent), ui(new Ui::QtQuatschEditor)
    , currentPropertyWidget (0)
{
        ui->setupUi(this);


        // Graph Editor
        {
                QGraphicsScene *scene = new QGraphicsScene (this);
                scene->setItemIndexMethod(QGraphicsScene::NoIndex);
                ui->graphicsView->setScene(scene);
                ui->graphicsView->setRenderHint(QPainter::Antialiasing);

                QObject::connect(scene, SIGNAL(selectionChanged()),
                  this, SLOT(on_graphicsScene_selectionChanged()));

                this->rootNode = new NodeItem (scene, 0, 0);

                on_graphicsScene_selectionChanged();
                rootNode->doLayout();

                QRectF rect = scene->itemsBoundingRect();
                ui->graphicsView->setSceneRect(rect);
                ui->graphicsView->show();
        }

        // Heightmap Preview
        {
                QGraphicsScene *scene = new QGraphicsScene (this);
                scene->setItemIndexMethod(QGraphicsScene::NoIndex);
                ui->heightmapView->setScene(scene);
                ui->heightmapView->setRenderHint(QPainter::Antialiasing);


                heightmap = QPixmap (256,256);
                ui->heightmapLabel->setPixmap(heightmap);

                QRectF rect = scene->itemsBoundingRect();
                ui->heightmapView->setSceneRect(rect);
                ui->heightmapView->show();
        }
}



QtQuatschEditor::~QtQuatschEditor() {
    delete ui;
}



void QtQuatschEditor::mousePressEvent(QGraphicsSceneMouseEvent *event) {
        Q_UNUSED(event)
}



void QtQuatschEditor::keyPressEvent(QKeyEvent* event) {
        if (event->modifiers().testFlag(Qt::ControlModifier)) {
                ui->graphicsView->verticalScrollBar()->setEnabled(false);
                ui->graphicsView->horizontalScrollBar()->setEnabled(false);
        }
}



void QtQuatschEditor::keyReleaseEvent(QKeyEvent* event) {
        Q_UNUSED(event)
        ui->graphicsView->verticalScrollBar()->setEnabled(true);
        ui->graphicsView->horizontalScrollBar()->setEnabled(true);
}



void QtQuatschEditor::wheelEvent(QWheelEvent* wheel) {
        if (wheel->modifiers().testFlag(Qt::ControlModifier)) {
                wheel->accept();
                const float
                        degrees =  static_cast<float>(wheel->delta()) / 8.f,
                        scale_ = 1.0f + degrees * 0.01f,
                        scale = scale_ < 0.125f ? 0.125f : scale_
                ;
                ui->graphicsView->scale(scale, scale);
        }
}



void QtQuatschEditor::on_addNodeButton_clicked() {
        QList<QGraphicsItem*> selected = ui->graphicsView->scene()->selectedItems();

        if (0 == selected.size()) {
                return;
        } else if (1 == selected.size()) {
                NodeItem* node = dynamic_cast<NodeItem*> (selected [0]);
                if (0 != node) {
                        node->addChild();
                }
        }
}



void QtQuatschEditor::on_insertLeftSiblingButton_clicked() {
        QList<QGraphicsItem*> selected = ui->graphicsView->scene()->selectedItems();

        if (0 == selected.size()) {
                return;
        } else if (1 == selected.size()) {
                NodeItem* node = dynamic_cast<NodeItem*> (selected [0]);
                if (0 != node) {
                        node->insertLeftSibling();
                }
        }
}



void QtQuatschEditor::on_insertRightSiblingButton_clicked() {
        QList<QGraphicsItem*> selected = ui->graphicsView->scene()->selectedItems();

        if (0 == selected.size()) {
                return;
        } else if (1 == selected.size()) {
                NodeItem* node = dynamic_cast<NodeItem*> (selected [0]);
                if (0 != node) {
                        node->insertRightSibling();
                }
        }
}



void QtQuatschEditor::on_graphicsScene_selectionChanged() {
        QList<QGraphicsItem*> selected = ui->graphicsView->scene()->selectedItems();
        if (1 == selected.size()) {
                NodeItem* node = dynamic_cast<NodeItem*> (selected [0]);
                if (0 == node)
                        return;

                const NodeItem::Type type = node->getType();
                ui->typeCombo->setCurrentIndex (type);

                if (node->isRootItem()) {
                        ui->insertLeftSiblingButton->setEnabled (false);
                        ui->insertRightSiblingButton->setEnabled (false);
                        ui->addNodeButton->setEnabled (true);
                        ui->deleteNodeButton->setEnabled (false);
                        ui->typeCombo->setEnabled (true);
                } else {
                        ui->insertLeftSiblingButton->setEnabled (true);
                        ui->insertRightSiblingButton->setEnabled (true);
                        ui->addNodeButton->setEnabled (true);
                        ui->typeCombo->setEnabled (true);
                        ui->deleteNodeButton->setEnabled (true);
                }
                ui->centerOnSelectedButton->setEnabled (true);
                ui->nodePropertiesGroupBox->setEnabled(true);

                if (node->isCompilable()) {
                        QImage q = node->genHeightmap(256,256);
                        heightmap = QPixmap::fromImage(q);
                        ui->heightmapLabel->setPixmap (heightmap);
                } else {
                        QImage q(256,256,QImage::Format_RGB32);
                        q.fill(QColor(200,130,130).rgb());
                        heightmap = QPixmap::fromImage(q);
                        ui->heightmapLabel->setPixmap (heightmap);
                }

        } else {
                ui->insertLeftSiblingButton->setEnabled (false);
                ui->insertRightSiblingButton->setEnabled (false);
                ui->addNodeButton->setEnabled (false);
                ui->typeCombo->setEnabled (false);
                ui->centerOnSelectedButton->setEnabled (false);
                ui->deleteNodeButton->setEnabled (false);
                ui->typeCombo->setCurrentIndex(0);
                ui->nodePropertiesGroupBox->setEnabled(false);

                if (rootNode->isCompilable()) {
                        QImage q = rootNode->genHeightmap(256,256);
                        heightmap = QPixmap::fromImage(q);
                        ui->heightmapLabel->setPixmap (heightmap);
                } else {
                        QImage q(256,256,QImage::Format_RGB32);
                        q.fill(QColor(200,130,130).rgb());
                        heightmap = QPixmap::fromImage(q);
                        ui->heightmapLabel->setPixmap (heightmap);
                }
        }

        displayPropertyWindow();
        repaint();
}



void QtQuatschEditor::on_typeCombo_currentIndexChanged(int index) {
        QList<QGraphicsItem*> selected = ui->graphicsView->scene()->selectedItems();
        if (1 == selected.size()) {
                NodeItem* node = dynamic_cast<NodeItem*> (selected [0]);
                if (0 == node)
                        return;

                node->setType(static_cast<NodeItem::Type>(index));
        }
        displayPropertyWindow();
}



void QtQuatschEditor::on_completeViewButton_clicked() {
        QRectF rect = ui->graphicsView->scene()->itemsBoundingRect();
        ui->graphicsView->setSceneRect(rect);
        rect.adjust(-30,-30,30,30);
        ui->graphicsView->fitInView (rect, Qt::KeepAspectRatio);
        ui->graphicsView->show();
}



void QtQuatschEditor::on_centerOnSelectedButton_clicked() {
        QList<QGraphicsItem*> selected = ui->graphicsView->scene()->selectedItems();
        if (1 == selected.size()) {
                ui->graphicsView->resetTransform();
                ui->graphicsView->centerOn(selected[0]);
        }
}



void QtQuatschEditor::on_deleteNodeButton_clicked() {
        QList<QGraphicsItem*> selected = ui->graphicsView->scene()->selectedItems();
        if (1 == selected.size()) {
                NodeItem* node = dynamic_cast<NodeItem*> (selected [0]);
                if (0 == node)
                        return;

                QMessageBox box;
                box.setText("This will delete ALL children, too!");
                box.setIcon(QMessageBox::Warning);
                box.setStandardButtons(QMessageBox::Abort | QMessageBox::Apply);
                box.setDefaultButton(QMessageBox::Abort);
                if (box.exec() == QMessageBox::Apply) {
                        node->die();
                }
        }
}



void QtQuatschEditor::displayPropertyWindow() {
        if (0 != currentPropertyWidget) {
                ui->nodePropertiesLayout->removeWidget (currentPropertyWidget);
                delete currentPropertyWidget;
                currentPropertyWidget = 0;
        }


        QList<QGraphicsItem*> selected = ui->graphicsView->scene()->selectedItems();
        if (1 == selected.size()) {
                NodeItem* node = dynamic_cast<NodeItem*> (selected [0]);
                if (0 == node)
                        return;

                switch (node->getType()) {
                case NodeItem::Undefined:
                        break;
                case NodeItem::Addition:
                        break;
                case NodeItem::Subtraction:
                        break;
                case NodeItem::Multiplication:
                        break;
                case NodeItem::Division:
                        break;
                case NodeItem::PredefinedConstant:
                        currentPropertyWidget = new PredefinedConstant(node);
                        ui->nodePropertiesLayout->addWidget(currentPropertyWidget);
                        break;
                case NodeItem::Parameter:
                        currentPropertyWidget = new Parameter(node);
                        ui->nodePropertiesLayout->addWidget(currentPropertyWidget);
                        break;
                case NodeItem::UserConstant:
                        currentPropertyWidget = new UserConstant(node);
                        ui->nodePropertiesLayout->addWidget(currentPropertyWidget);
                        break;
                };
                this->repaint();
        }
}



void QtQuatschEditor::on_pushButton_5_clicked() {
        QMessageBox box;
        box.setWindowTitle("code");
        box.setText(rootNode->genJuxCode());
        box.exec();
}



void QtQuatschEditor::on_moveNodeUpButton_clicked() {
        QList<QGraphicsItem*> selected = ui->graphicsView->scene()->selectedItems();
        if (1 == selected.size()) {
                NodeItem* node = dynamic_cast<NodeItem*> (selected [0]);
                if (0 == node)
                        return;
                node->moveUp();
        }
}



void QtQuatschEditor::on_moveNodeDownButton_clicked() {
        QList<QGraphicsItem*> selected = ui->graphicsView->scene()->selectedItems();
        if (1 == selected.size()) {
                NodeItem* node = dynamic_cast<NodeItem*> (selected [0]);
                if (0 == node)
                        return;
                node->moveDown();
        }
}



void QtQuatschEditor::on_asParentsSiblingButton_clicked() {
        QList<QGraphicsItem*> selected = ui->graphicsView->scene()->selectedItems();
        if (1 == selected.size()) {
                NodeItem* node = dynamic_cast<NodeItem*> (selected [0]);
                if (0 == node)
                        return;
                node->asParentsSibling();
        }
}



void QtQuatschEditor::on_asLeftSiblingsChildButton_clicked() {
        QList<QGraphicsItem*> selected = ui->graphicsView->scene()->selectedItems();
        if (1 == selected.size()) {
                NodeItem* node = dynamic_cast<NodeItem*> (selected [0]);
                if (0 == node)
                        return;
                node->asLeftSiblingsChild();
        }
}



void QtQuatschEditor::on_asRightSiblingsChildButton_clicked() {
        QList<QGraphicsItem*> selected = ui->graphicsView->scene()->selectedItems();
        if (1 == selected.size()) {
                NodeItem* node = dynamic_cast<NodeItem*> (selected [0]);
                if (0 == node)
                        return;
                node->asRightSiblingsChild();
        }
}
