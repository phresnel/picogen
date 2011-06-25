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

#ifndef FILMSETTINGSWINDOW_HH_20100902
#define FILMSETTINGSWINDOW_HH_20100902

#include <QWidget>
#include "redshift/include/smart_ptr.hh"
#include "cosyscene/cosyfwd.hh"

namespace Ui {
    class FilmSettingsWindow;
}

namespace picogen { namespace qt4_gui {

class FilmSettingsWindow : public QWidget
{
    Q_OBJECT
// ================== Public ===================================================
public:
        explicit FilmSettingsWindow(QWidget *parent = 0);
        virtual ~FilmSettingsWindow();

        // Support for this function will cease once tone mapping is implemented.
        void setFilmSettings (redshift::shared_ptr<cosyscene::FilmSettings>,
                              bool blockSignals=true);
public slots:
        void sceneInvalidated(redshift::shared_ptr<cosyscene::Scene> scene);

signals:
        void filmSettingsChanged();

// ================== Private ==================================================
private:
        void updateViews();

private slots:
        void on_brightness_valueChanged(double );

        void on_stashButton_clicked();
        void on_stashRestoreButton_clicked();
        void on_stashResetButton_clicked();

private:
        Ui::FilmSettingsWindow *ui;
        redshift::shared_ptr<cosyscene::FilmSettings> filmSettings_;
};

} }

#endif // FILMSETTINGSWINDOW_HH
