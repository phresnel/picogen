#ifndef COMPLEXGUI_HH
#define COMPLEXGUI_HH

#include <QMainWindow>

namespace Ui {
    class ComplexGUI;
}

class ComplexGUI : public QMainWindow {
    Q_OBJECT
public:
    ComplexGUI(QWidget *parent = 0);
    ~ComplexGUI();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ComplexGUI *ui;
};

#endif // COMPLEXGUI_HH
