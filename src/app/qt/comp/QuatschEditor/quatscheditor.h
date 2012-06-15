#ifndef QUATSCHEDITOR_H
#define QUATSCHEDITOR_H

#include <QWidget>

namespace Ui {
    class QuatschEditor;
}

class QuatschEditor : public QWidget
{
    Q_OBJECT

public:
    explicit QuatschEditor(QWidget *parent = 0);
    ~QuatschEditor();

private:
    Ui::QuatschEditor *ui;
};

#endif // QUATSCHEDITOR_H
