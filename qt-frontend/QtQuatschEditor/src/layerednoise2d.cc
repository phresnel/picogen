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
#include "layerednoise2d.hh"
#include "ui_layerednoise2d.h"


LayeredNoise2dUi::LayeredNoise2dUi(NodeItem *node_, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::LayeredNoise2d),
    node(node_)
{
        m_ui->setupUi(this);
        QuatschNodeValue value = node->getValue();

        node->setEnableAutoUpdateHeightmap(false);
        switch (value.asLayeredNoise2d().filter) {
        case QuatschNodeValue::LayeredNoise2d::Nearest:
                m_ui->filter->setCurrentIndex(0);
                on_filter_currentIndexChanged(0);
                break;
        case QuatschNodeValue::LayeredNoise2d::Bilinear:
                m_ui->filter->setCurrentIndex(1);
                on_filter_currentIndexChanged(1);
                break;
        case QuatschNodeValue::LayeredNoise2d::Cosine:
                on_filter_currentIndexChanged(2);
                m_ui->filter->setCurrentIndex(2);
                break;
        };


        m_ui->size->setValue(value.asLayeredNoise2d().width);
        on_size_valueChanged(value.asLayeredNoise2d().width);

        m_ui->depth->setValue(value.asLayeredNoise2d().depth);
        on_depth_valueChanged(value.asLayeredNoise2d().depth);

        m_ui->seed->setValue(value.asLayeredNoise2d().seed);
        on_seed_valueChanged(value.asLayeredNoise2d().seed);

        node->setEnableAutoUpdateHeightmap(true);
        node->updateHeightmap();
}



LayeredNoise2dUi::~LayeredNoise2dUi() {
        delete m_ui;
}



void LayeredNoise2dUi::changeEvent(QEvent *e) {
        QWidget::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
                m_ui->retranslateUi(this);
                break;
        default:
                break;
        }
}



void LayeredNoise2dUi::on_filter_currentIndexChanged(int index) {
        QuatschNodeValue value = node->getValue();
        QuatschNodeValue::LayeredNoise2d n2d = value.asLayeredNoise2d();
        switch (index) {
        case 0: n2d.filter = QuatschNodeValue::LayeredNoise2d::Nearest; break;
        case 1: n2d.filter = QuatschNodeValue::LayeredNoise2d::Bilinear; break;
        case 2: n2d.filter = QuatschNodeValue::LayeredNoise2d::Cosine; break;
        };
        value.setLayeredNoise2d (n2d);
        node->setValue (value);
}



void LayeredNoise2dUi::on_size_valueChanged(int val) {
        QuatschNodeValue value = node->getValue();
        QuatschNodeValue::LayeredNoise2d n2d = value.asLayeredNoise2d();
        n2d.width = val;
        value.setLayeredNoise2d (n2d);
        node->setValue (value);
}



void LayeredNoise2dUi::on_depth_valueChanged(int val) {
        QuatschNodeValue value = node->getValue();
        QuatschNodeValue::LayeredNoise2d n2d = value.asLayeredNoise2d();
        n2d.depth = val;
        value.setLayeredNoise2d (n2d);
        node->setValue (value);
}



void LayeredNoise2dUi::on_seed_valueChanged(int val) {
        QuatschNodeValue value = node->getValue();
        QuatschNodeValue::LayeredNoise2d n2d = value.asLayeredNoise2d();
        n2d.seed = val;
        value.setLayeredNoise2d (n2d);
        node->setValue (value);
}
