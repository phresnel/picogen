#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QWidget>

namespace Ui {
    class RenderWidget;
}

namespace picogen { namespace cracker {
        class RenderTarget;
} }

class RenderWidget : public QWidget
{
    Q_OBJECT

public:
        explicit RenderWidget(QWidget *parent = 0);
        ~RenderWidget();

private slots:
        void on_pushButton_clicked();

private:
        void updateDisplay (picogen::cracker::RenderTarget const &);
private:
        Ui::RenderWidget *ui;
};

#endif // RENDERWIDGET_H
