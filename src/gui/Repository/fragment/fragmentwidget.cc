//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2011  Sebastian Mach (*1983)
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

#include "fragmentwidget.hh"
#include "ui_fragmentwidget.h"

namespace picogen_repository {

FragmentWidget::FragmentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FragmentWidget)
{
        ui->setupUi(this);
}

FragmentWidget::~FragmentWidget() {
        delete ui;
}

void FragmentWidget::setEntity (Fragment *e) {
        entity_ = e;

        ui->title->setText(e->title());
        ui->email->setText(e->email());
        ui->homepage->setText(e->homepage());
        ui->author->setText(e->author());
}

void FragmentWidget::on_title_editingFinished() {
        entity_->setTitle (ui->title->text());
}

void FragmentWidget::on_author_editingFinished() {
        entity_->setAuthor (ui->author->text());
}

void FragmentWidget::on_email_editingFinished() {
        entity_->setEmail(ui->email->text());
}

void FragmentWidget::on_homepage_editingFinished() {
        entity_->setHomepage(ui->homepage->text());
}

} // namespace picogen_repository {
