#ifndef LAYEREDNOISE2D_HH
#define LAYEREDNOISE2D_HH

#include <QtGui/QWidget>

class NodeItem;

namespace Ui {
    class LayeredNoise2d;
}

class LayeredNoise2d : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(LayeredNoise2d)
public:
    explicit LayeredNoise2d(NodeItem *node_, QWidget *parent = 0);
    virtual ~LayeredNoise2d();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::LayeredNoise2d *m_ui;
    NodeItem *node;

private slots:
    void on_depth_valueChanged(int );
    void on_size_valueChanged(int );
    void on_filter_currentIndexChanged(int index);
    //void on_size_textChanged(QString );
    void on_seed_textChanged(QString );
};

#endif // LAYEREDNOISE2D_HH
