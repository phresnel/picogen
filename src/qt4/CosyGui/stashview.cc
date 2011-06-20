//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
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

// TODO: implement deletion

#include "stashview.h"
#include "ui_stashview.h"

#include <QDateTime>

StashView::StashView(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::StashView)
{
        ui->setupUi(this);
}

StashView::~StashView() {
        delete ui;
}

void StashView::addItem (const StashViewItem &item) {
        items.push_back(item);

        ui->listWidget->clear();

        const QDate today = QDateTime::currentDateTime().date();
        for (int i=0; i<items.size(); ++i) {
                const StashViewItem &item = items.at(i);
                const QDateTime datetime = QDateTime::fromTime_t(item.time);

                const int daysAgo = datetime.date().daysTo(today);
                const QString dateFmt =
                                (daysAgo == 0) ? "hh:mm:ss" :
                                (daysAgo < 7) ? "dddd, hh:mm:ss" :
                                (daysAgo < 365) ? "MMM dd, hh:mm:ss" :
                                "MMM dd yyyy, hh:mm:ss";
                const QString dateString = datetime.toString(dateFmt);


                QString text = dateString;

                const QString description =
                                QString::fromStdString(item.description)
                                .simplified();
                if (description != "")
                        text += ": " + description;
                QListWidgetItem *lwi = new QListWidgetItem(text);
                ui->listWidget->addItem(lwi);
        }
}

void StashView::on_listWidget_itemDoubleClicked(QListWidgetItem* ) {
        accept();
}

bool StashView::hasSelectedData() const {
        return  0 < ui->listWidget->selectedItems().size();
}

int StashView::selectedIndex() const {
        return ui->listWidget->currentRow();
}

void StashView::on_okayButton_clicked() {
        if (hasSelectedData())
                accept();
        else
                reject();
}

void StashView::on_cancelButton_clicked() {
        reject();
}

#include "stashview.moc"
