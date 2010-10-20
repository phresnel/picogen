#ifndef SIMPLEINPUTBOX_HH
#define SIMPLEINPUTBOX_HH

#include <QDialog>

namespace Ui {
    class SimpleInputBox;
}

class SimpleInputBox : public QDialog {
        Q_OBJECT
public:
        SimpleInputBox(QWidget *parent = 0);
        ~SimpleInputBox();
        QString value() const {
                return value_;
        }
protected:
        void changeEvent(QEvent *e);

private:
        Ui::SimpleInputBox *ui;
        QString value_;

private slots:
    void on_lineEdit_returnPressed();
};

#endif // SIMPLEINPUTBOX_HH
