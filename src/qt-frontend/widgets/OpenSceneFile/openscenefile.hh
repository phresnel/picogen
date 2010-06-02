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


#ifndef OPENSCENEFILE_HH
#define OPENSCENEFILE_HH

#include <QDialog>

namespace Ui {
        class OpenSceneFile;
}

class OpenSceneFile : public QDialog {
    Q_OBJECT
public:
        enum Result { Cancel, OpenInNewInstance, OpenInCurrentInstance };
        OpenSceneFile(QString const &path, QWidget *parent = 0);
        ~OpenSceneFile();

        Result result () const { return result_; }

protected:
        void changeEvent(QEvent *e);

private:
        Ui::OpenSceneFile *ui;
        Result result_;
        const QString path;

private slots:
        void on_cancel_clicked();
        void on_currentInstance_clicked();
        void on_newInstance_clicked();
};

#endif // OPENSCENEFILE_HH
