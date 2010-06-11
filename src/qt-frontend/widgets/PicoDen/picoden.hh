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

#ifndef PICODEN_HH
#define PICODEN_HH

#include <QWidget>

namespace Ui
{
class PicoDen;
}

class PicoDen : public QWidget
{
        Q_OBJECT
public:
        PicoDen(QString const & repositoryRoot, QWidget *parent = 0);
        PicoDen(QWidget *parent = 0);
        ~PicoDen();

        void setRoot (QString const &root);

protected:

private:
        void showScenes();

        Ui::PicoDen *ui;
        QString repositoryRoot;

private slots:
    void on_filter_currentIndexChanged(QString );
};

#endif // PICODEN_HH
