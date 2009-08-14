#include "qtquatscheditor.hh"
#include "layerednoise2d.hh"
#include "ui_layerednoise2d.h"


LayeredNoise2d::LayeredNoise2d(NodeItem *node_, QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::LayeredNoise2d),
    node(node_)
{
        m_ui->setupUi(this);
        NodeItem::Value value = node->getValue();

        switch (value.asLayeredNoise2d().filter) {
        case NodeItem::Value::LayeredNoise2d::Nearest:
                m_ui->filter->setCurrentIndex(0);
                on_filter_currentIndexChanged(0);
                break;
        case NodeItem::Value::LayeredNoise2d::Bilinear:
                m_ui->filter->setCurrentIndex(1);
                on_filter_currentIndexChanged(1);
                break;
        case NodeItem::Value::LayeredNoise2d::Cosine:
                on_filter_currentIndexChanged(2);
                m_ui->filter->setCurrentIndex(2);
                break;
        };


        m_ui->size->setValue(value.asLayeredNoise2d().width);
        on_size_valueChanged(value.asLayeredNoise2d().width);

        m_ui->depth->setValue(value.asLayeredNoise2d().depth);
        on_depth_valueChanged(value.asLayeredNoise2d().depth);

        m_ui->seed->setText(QString::number(value.asLayeredNoise2d().seed));
        on_seed_textChanged(QString::number(value.asLayeredNoise2d().seed));
}



LayeredNoise2d::~LayeredNoise2d() {
        delete m_ui;
}



void LayeredNoise2d::changeEvent(QEvent *e) {
        QWidget::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
                m_ui->retranslateUi(this);
                break;
        default:
                break;
        }
}



void LayeredNoise2d::on_filter_currentIndexChanged(int index) {
        NodeItem::Value value = node->getValue();
        NodeItem::Value::LayeredNoise2d n2d = value.asLayeredNoise2d();
        switch (index) {
        case 0: n2d.filter = NodeItem::Value::LayeredNoise2d::Nearest; break;
        case 1: n2d.filter = NodeItem::Value::LayeredNoise2d::Bilinear; break;
        case 2: n2d.filter = NodeItem::Value::LayeredNoise2d::Cosine; break;
        };
        value.setLayeredNoise2d (n2d);
        node->setValue (value);
}



void LayeredNoise2d::on_size_valueChanged(int val) {
        NodeItem::Value value = node->getValue();
        NodeItem::Value::LayeredNoise2d n2d = value.asLayeredNoise2d();
        n2d.width = val;
        value.setLayeredNoise2d (n2d);
        node->setValue (value);
}



void LayeredNoise2d::on_depth_valueChanged(int val) {
        NodeItem::Value value = node->getValue();
        NodeItem::Value::LayeredNoise2d n2d = value.asLayeredNoise2d();
        n2d.depth = val;
        value.setLayeredNoise2d (n2d);
        node->setValue (value);
}



void LayeredNoise2d::on_seed_textChanged(QString text) {
        NodeItem::Value value = node->getValue();
        NodeItem::Value::LayeredNoise2d n2d = value.asLayeredNoise2d();
        n2d.seed = text.toInt();
        value.setLayeredNoise2d (n2d);
        node->setValue (value);
}
