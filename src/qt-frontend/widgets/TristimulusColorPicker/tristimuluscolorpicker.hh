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
        double y_;
public:

        TristimulusColor () : r(0), g(0), b(0), y_(1) {}

        explicit TristimulusColor (QColor const &col, double y)
                : r(col.redF())
                , g(col.greenF())
                , b(col.blueF())
                , y_(y)
        {}

        static TristimulusColor fromRgb (int r, int g, int b, double y) {
                TristimulusColor ret;
                ret.r = r / 255.;
                ret.g = g / 255.;
                ret.b = b / 255.;
                ret.y_ = y;
                return  ret;
        }

        static TristimulusColor fromRgbf (double r, double g, double b, double y) {
                TristimulusColor ret;
                ret.r = r;
                ret.g = g;
                ret.b = b;
                ret.y_ = y;
                return  ret;
        }

        void setY (double v) { y_ = v; }
        double y () const { return y_; }

        void setRed (int v)   { r = v / 255.; }
        void setGreen (int v) { g = v / 255.; }
        void setBlue (int v)  { b = v / 255.; }

        void setRedF (double v)   { r = v; }
        void setGreenF (double v) { g = v; }
        void setBlueF (double v)  { b = v; }

        int red()   const { return r * 255; }
        int green() const { return g * 255; }
        int blue()  const { return b * 255; }

        double redF()   const { return r; }
        double greenF() const { return g; }
        double blueF()  const { return b; }

        bool isGray(double epsilon = 0.0001) const {
                return r >= g-epsilon && r <= g+epsilon
                    && r >= b-epsilon && r <= b+epsilon
                ;
        }

        QColor toQColor () const {
                return QColor::fromRgbF(r, g, b);
        }


        bool operator == (TristimulusColor const &rhs) const {
                return r == rhs.r
                    && g == rhs.g
                    && b == rhs.b
                    && y_ == rhs.y_
                    ;
        }
        bool operator != (TristimulusColor const &rhs) const {
                return !(*this == rhs);
        }
};



class TristimulusColorPicker : public QWidget {
        Q_OBJECT
public:
        TristimulusColorPicker(QWidget *parent = 0);
        ~TristimulusColorPicker();

        TristimulusColor color() const ;
        void setColor (TristimulusColor const &TristimulusColor) ;

protected:
        void changeEvent(QEvent *e);
        void resizeEvent(QResizeEvent *);

private:
        void drawColorPreview();
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
        void on_spinBrightness_valueChanged(double );
        void on_spinR_valueChanged(double);
        void on_spinG_valueChanged(double);
        void on_spinB_valueChanged(double);

        void on_spinH_valueChanged(double);
        void on_spinS_valueChanged(double);
        void on_spinV_valueChanged(double);

        void on_spinC_valueChanged(double);
        void on_spinM_valueChanged(double);
        void on_spinY_valueChanged(double);
        void on_spinK_valueChanged(double);
};



#endif // TRISTIMULUSCOLORPICKER_HH
