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
#include "predefinedconstant.hh"
#include "ui_predefinedconstant.h"


PredefinedConstant::PredefinedConstant(NodeItem *node_, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::PredefinedConstant),
    node (node_)
{
        m_ui->setupUi(this);

        node->setEnableAutoUpdateHeightmap(false);

        switch (node->getValue().asPredefinedConstant()) {
        case NodeItem::Value::Pi: m_ui->comboBox->setCurrentIndex(0); break;
        case NodeItem::Value::e: m_ui->comboBox->setCurrentIndex(1); break;
        };
        on_comboBox_currentIndexChanged (m_ui->comboBox->currentIndex());

        node->setEnableAutoUpdateHeightmap(true);
        node->updateHeightmap();
}



PredefinedConstant::~PredefinedConstant() {
        delete m_ui;
}



void PredefinedConstant::changeEvent(QEvent *e) {
        QWidget::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
                m_ui->retranslateUi(this);
                break;
        default:
                break;
        }
}

void PredefinedConstant::on_comboBox_currentIndexChanged(int index) {
        NodeItem::Value val = node->getValue();
        switch (index) {
        case 0:
                val.setPredefinedConstant(NodeItem::Value::Pi);
                m_ui->valueLabel->setText("3.14159265...");
                break;
        case 1:
                val.setPredefinedConstant(NodeItem::Value::e);
                m_ui->valueLabel->setText("2.71828183...");
                break;
        };
        node->setValue (val);
}
