#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QWidget>
#include <memory>

namespace Ui {
    class RenderWidget;
}


class RenderWidget : public QWidget
{
    Q_OBJECT

public:
        explicit RenderWidget(QWidget *parent = 0);
        ~RenderWidget();

private slots:
        void on_pushButton_clicked();

        void on_resetsceneButton_clicked();

private:
        void updateDisplay ();
private:
        Ui::RenderWidget *ui;
};

#endif // RENDERWIDGET_H
