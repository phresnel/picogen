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

#ifndef QUATSCHPREVIEW_HH
#define QUATSCHPREVIEW_HH

#include <QWidget>
#include <string>

#include <QRect>
#include <boost/optional.hpp>

namespace Ui {
        class QuatschPreview;
}

namespace picogen { namespace qt4 {

class QuatschPreview : public QWidget
{
        Q_OBJECT

public:
        explicit QuatschPreview(QWidget *parent = 0);
        ~QuatschPreview();

        void setStatusText (QString const &str);

        void setCode (std::string const &str);
        void setCode (QString const &str);
        void compileAndRun();
        void compileAndRun(std::string const &str);
        void compileAndRun(QString const &str);
private:
        Ui::QuatschPreview *ui;
        std::string code;

        boost::optional<QRect> drawRect;

        bool eventFilter(QObject *, QEvent *);

private slots:
};

} }

#endif // QUATSCHPREVIEW_HH
