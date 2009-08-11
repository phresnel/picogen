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
#include "parameter.hh"
#include "ui_parameter.h"


Parameter::Parameter(NodeItem *node_, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::Parameter),
    node(node_)
{
    m_ui->setupUi(this);

    const NodeItem::Value val = node->getValue();
    const std::string name = val.asParameter();
    if ("x" == name) {
            m_ui->comboBox->setCurrentIndex(0);
            on_comboBox_currentIndexChanged(0);
    } else if ("y" == name) {
            m_ui->comboBox->setCurrentIndex(1);
            on_comboBox_currentIndexChanged(1);
    }
}



Parameter::~Parameter() {
    delete m_ui;
}



void Parameter::changeEvent(QEvent *e) {
        QWidget::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
                m_ui->retranslateUi(this);
                break;
        default:
                break;
        }
}



void Parameter::on_comboBox_currentIndexChanged(int index) {

        NodeItem::Value val = node->getValue();

        switch (index) {
        case 0:
                m_ui->description->setText("Left=0\nRight=1");
                val.setParameter("x");
                break;
        case 1:
                m_ui->description->setText("Top=0\nBottom=1");
                val.setParameter("y");
                break;
        };

        node->setValue(val);
}
