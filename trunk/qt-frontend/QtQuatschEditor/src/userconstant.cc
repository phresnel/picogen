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

#include "userconstant.hh"
#include "ui_userconstant.h"

UserConstantUi::UserConstantUi(NodeItem *node_, QWidget *parent)
: QWidget(parent)
, m_ui(new Ui::UserConstant)
, node (node_)
{
        m_ui->setupUi(this);

        node->setEnableAutoUpdateHeightmap(false);

        m_ui->doubleSpinBox->setValue(node->getValue().asFloatConstant());

        node->setEnableAutoUpdateHeightmap(true);
        node->updateHeightmap();
}

UserConstantUi::~UserConstantUi() {
        delete m_ui;
}

void UserConstantUi::changeEvent(QEvent *e) {
        QWidget::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
                m_ui->retranslateUi(this);
                break;
        default:
                break;
        }
}

void UserConstantUi::on_doubleSpinBox_valueChanged(double f) {
        QuatschNodeValue val = node->getValue();
        val.setFloatConstant(f);
        node->setValue (val);
}
