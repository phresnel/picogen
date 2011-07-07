#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QWidget>

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

private:
    Ui::RenderWidget *ui;
};

#endif // RENDERWIDGET_H
