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

#ifndef FILENAMEEDIT_HH
#define FILENAMEEDIT_HH

#include <QWidget>

namespace Ui {
        class FilenameEdit;
}

namespace picogen { namespace qt4 {

class FilenameEdit : public QWidget {
        Q_OBJECT

public:
        explicit FilenameEdit(QWidget *parent = 0);
        ~FilenameEdit();

        QString filename() const;
        void setFilename (QString const &str);

signals:
        void textChanged();

private:
        Ui::FilenameEdit *ui;

private slots:
        void on_toolButton_clicked();
};

} }

#endif // FILENAMEEDIT_HH
