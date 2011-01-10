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

#ifndef FRAGMENTWIDGET_HH
#define FRAGMENTWIDGET_HH

#include <QWidget>
#include "parametricpreset.hh"

namespace Ui {
        class FragmentWidget;
}

namespace picogen_repository {

class FragmentWidget : public QWidget
{
        Q_OBJECT

public:
        explicit FragmentWidget(QWidget *parent = 0);
        ~FragmentWidget();

        void setEntity (Fragment *);

private:
        Ui::FragmentWidget *ui;
        Fragment *entity_;

private slots:
        void on_email_editingFinished();
        void on_author_editingFinished();
        void on_title_editingFinished();
        void on_homepage_editingFinished();
};

} // namespace picogen_repository {

#endif // FRAGMENTWIDGET_HH
