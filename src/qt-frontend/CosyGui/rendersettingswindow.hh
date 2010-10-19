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

#ifndef RENDERSETTINGS_HH
#define RENDERSETTINGS_HH

#include <QWidget>
#include "redshift/include/smart_ptr.hh"

namespace Ui {
        class RenderSettingsWindow;
}
namespace cosyscene {
        class RenderSettings;
}

class RenderSettingsWindow : public QWidget
{
        Q_OBJECT

public:
        explicit RenderSettingsWindow(QWidget *parent = 0);
        ~RenderSettingsWindow();

        void setTitle (QString string);

        void setRenderSettings (redshift::shared_ptr<cosyscene::RenderSettings>,
                                bool blockSignals=true);

private:
        Ui::RenderSettingsWindow *ui;
        redshift::shared_ptr<cosyscene::RenderSettings> renderSettings_;

private:
        void updateViews();

private slots:
        void on_seedSpin_editingFinished();
        void on_samplesPerPixelSpin_editingFinished();
        void on_heightSpin_editingFinished();
        void on_widthSpin_editingFinished();
};

#endif // RENDERSETTINGS_HH
