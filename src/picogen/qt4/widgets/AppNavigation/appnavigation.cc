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

#include <QToolButton>

#include "appnavigation.h"
#include "ui_appnavigation.h"

namespace picogen { namespace qt4 {

AppNavigation::AppNavigation(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AppNavigation)
{
        ui->setupUi(this);
}

AppNavigation::~AppNavigation()
{
        delete ui;
}

AppNavigationButton* AppNavigation::addButton (const QString &title, const QIcon &icon) {
        AppNavigationButton *button = new AppNavigationButton ();
        button->setIcon(icon);
        button->setText(title);
        addToolButton (button);
        return button;
}

void AppNavigation::addToolButton (AppNavigationButton *button) {
        button->setIconSize(QSize(32,32));
        button->setParent(this);
        button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
        button->setActive(false);
        connect(button, SIGNAL(activated(AppNavigationButton*)),
                this,   SLOT(activateButton(AppNavigationButton*)));
        // Take spacer, insert widget, re-insert spacer
        QLayoutItem *spacer = layout()->takeAt(layout()->count()-1);
        layout()->addWidget(button);
        layout()->addItem(spacer);

        buttons.push_back(button);
}

void AppNavigation::activateButton(AppNavigationButton *button) {
        // This might be slightly wasteful, but saves us from
        // having to keep a prev pointer that someday might go
        // invalid.
        foreach (AppNavigationButton *b, buttons) {
                if (b != button) {
                        b->setActive(false);
                }
        }
        button->setActive(true);
}

} }

#include "appnavigation.moc"
