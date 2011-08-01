#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QWidget>
#include <memory>

namespace Ui {
    class RenderWidget;
}

namespace picogen { namespace cracker {
        class RenderTarget;
        class Scene;
} }

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
        void updateDisplay (picogen::cracker::RenderTarget const &);
private:
        Ui::RenderWidget *ui;

        std::shared_ptr<picogen::cracker::Scene> scene_;
        std::shared_ptr<picogen::cracker::RenderTarget> target_;
};

#endif // RENDERWIDGET_H
