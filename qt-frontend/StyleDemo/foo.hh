#ifndef FOO_HH
#define FOO_HH

#include <QMainWindow>

namespace Ui {
    class Foo;
}

class Foo : public QMainWindow {
    Q_OBJECT
public:
    Foo(QWidget *parent = 0);
    ~Foo();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Foo *ui;
};

#endif // FOO_HH
