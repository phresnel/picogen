#ifndef HEIGHTMAPIMAGE_H
#define HEIGHTMAPIMAGE_H

#include <QWidget>

namespace Ui {
    class HeightmapImage;
}

class HeightmapImage : public QWidget
{
        Q_OBJECT

public:
        explicit HeightmapImage(QWidget *parent = 0);
        ~HeightmapImage();

private:
        void paintEvent (QPaintEvent *);

private:
        Ui::HeightmapImage *ui;
        bool errorSignaled;
};

#endif // HEIGHTMAPIMAGE_H
