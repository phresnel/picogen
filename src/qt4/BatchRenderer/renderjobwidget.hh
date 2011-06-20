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

#ifndef RENDERJOBWIDGET_HH
#define RENDERJOBWIDGET_HH

#include <QWidget>

namespace Ui {
    class RenderJobWidget;
}

class RenderJob;

class QFile;

class RenderJobWidget : public QWidget {
        Q_OBJECT
public:
        RenderJobWidget(QFile &jobfile, QWidget *parent = 0);
        ~RenderJobWidget();

        void updateProgress();

        bool isValid() const;

protected:
        void changeEvent(QEvent *e);

private:
        Ui::RenderJobWidget *ui;
        QString const jobFilename;
        QString const statusDirectory;
        RenderJob *job;
        bool isValid_;
};

#endif // RENDERJOBWIDGET_HH
