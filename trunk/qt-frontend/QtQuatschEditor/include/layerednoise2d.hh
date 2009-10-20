//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
// * mail: phresnel/at/gmail/dot/com
// * http://phresnel.org
// * http://picogen.org
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef LAYEREDNOISE2D_HH
#define LAYEREDNOISE2D_HH

#include <QtGui/QWidget>

class NodeItem;

namespace Ui {
    class LayeredNoise2d;
}

class LayeredNoise2dUi : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(LayeredNoise2dUi)
public:
    explicit LayeredNoise2dUi(NodeItem *node_, QWidget *parent = 0);
    virtual ~LayeredNoise2dUi();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::LayeredNoise2d *m_ui;
    NodeItem *node;

private slots:
    void on_depth_valueChanged(int );
    void on_size_valueChanged(int );
    void on_filter_currentIndexChanged(int index);
    void on_seed_valueChanged(int );
};

#endif // LAYEREDNOISE2D_HH
