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

#ifndef TRISTIMULUSCOLORPICKER_HH
#define TRISTIMULUSCOLORPICKER_HH

#include <QWidget>
#include <QColor>

namespace Ui {
    class TristimulusColorPicker;
}

class TristimulusColorPicker : public QWidget {
        Q_OBJECT
public:
        TristimulusColorPicker(QWidget *parent = 0);
        ~TristimulusColorPicker();

        QColor color() const ;
        void setColor (QColor const &color) ;

protected:
        void changeEvent(QEvent *e);


private:
        Ui::TristimulusColorPicker *ui;
        bool isUpdating;
        QColor color_;


//------------------------------------------------------------------------------
// Signals + Slots
//------------------------------------------------------------------------------
signals:
        void colorChanged (const QColor & color);

public slots:
        void on_triangle_colorChanged(const QColor & color);

private slots:
        void on_spinR_valueChanged(int);
        void on_spinG_valueChanged(int);
        void on_spinB_valueChanged(int);

        void on_spinH_valueChanged(int);
        void on_spinS_valueChanged(int);
        void on_spinV_valueChanged(int);

        void on_spinC_valueChanged(int);
        void on_spinM_valueChanged(int);
        void on_spinY_valueChanged(int);
        void on_spinK_valueChanged(int);
};



#endif // TRISTIMULUSCOLORPICKER_HH
