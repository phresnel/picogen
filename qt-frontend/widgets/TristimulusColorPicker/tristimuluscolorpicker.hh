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
public slots:
        void on_triangle_colorChanged(const QColor & color);
signals:
        void colorChanged (const QColor & color);
protected:
        void changeEvent(QEvent *e);

private:
        Ui::TristimulusColorPicker *ui;        
        void updateAllByRgb();
        bool isInside_updateAllByRgb;

        void updateAllByHsv();
        bool isInside_updateAllByHsv;

        void updateCmykSpins(QColor const &color);
        void updateRgbSpins(QColor const &color);
        void updateHsvSpins(QColor const &color);

        void updateAllSpins(QColor const &color);

private slots:
        void on_spinR_valueChanged(int);
        void on_spinG_valueChanged(int);
        void on_spinB_valueChanged(int);

        void on_spinH_valueChanged(int);
        void on_spinS_valueChanged(int);
        void on_spinV_valueChanged(int);
};

#endif // TRISTIMULUSCOLORPICKER_HH
