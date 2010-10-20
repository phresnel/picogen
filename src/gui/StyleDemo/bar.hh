#ifndef BAR_HH
#define BAR_HH

#include <QWidget>

namespace Ui {
    class Bar;
}

class Bar : public QWidget {
    Q_OBJECT
public:
    Bar(QWidget *parent = 0);
    ~Bar();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Bar *ui;
};

#endif // BAR_HH
