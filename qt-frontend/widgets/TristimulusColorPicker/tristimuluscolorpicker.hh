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


class TristimulusColor {
        double r, g, b;
public:

        TristimulusColor () : r(0), g(0), b(0) {}

        explicit TristimulusColor (QColor const &col)
                : r(col.redF())
                , g(col.greenF())
                , b(col.blueF())
        {}

        TristimulusColor (int r, int g, int b)
        : r(r/255), g(g/255), b(b/255) {}

        TristimulusColor (double r, double g, double b)
        : r(r), g(g), b(b) {}

        void setRed (int v)   { r = v; }
        void setGreen (int v) { g = v; }
        void setBlue (int v)  { b = v; }

        int red() const { return r / 255; }
        int green() const { return g / 255; }
        int blue() const { return b / 255; }

        int redF() const { return r / 255; }
        int greenF() const { return g / 255; }
        int blueF() const { return b / 255; }

        QColor toQColor () const {
                /*const int r_ = this->r, r = r_<0 ? 0 : r_>255 ? 255 : r_;
                const int g_ = this->g, g = g_<0 ? 0 : g_>255 ? 255 : g_;
                const int b_ = this->b, b = b_<0 ? 0 : b_>255 ? 255 : b_;*/
                return QColor::fromRgbF(r, g, b);
                /*return QColor::fromRgb(
                                r, g, b
                );*/
        }
};

inline bool operator == (TristimulusColor const &lhs, TristimulusColor const &rhs) {
        return lhs.red() == rhs.red()
            && lhs.green() == rhs.green()
            && lhs.blue() == rhs.blue();
}
inline bool operator != (TristimulusColor const &lhs, TristimulusColor const &rhs) {
        return !(lhs == rhs);
}


class TristimulusColorPicker : public QWidget {
        Q_OBJECT
public:
        TristimulusColorPicker(QWidget *parent = 0);
        ~TristimulusColorPicker();

        TristimulusColor color() const ;
        void setColor (TristimulusColor const &TristimulusColor) ;

protected:
        void changeEvent(QEvent *e);


private:
        Ui::TristimulusColorPicker *ui;
        bool isUpdating;
        TristimulusColor color_;


//------------------------------------------------------------------------------
// Signals + Slots
//------------------------------------------------------------------------------
signals:
        void colorChanged (const TristimulusColor & color);

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
