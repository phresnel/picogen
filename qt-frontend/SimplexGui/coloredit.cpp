//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
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

// Based on QtQuarterly 18

#include "coloredit.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QFileDialog>
#include <QFocusEvent>

#include "redshift/include/setup.hh"

ColorEdit::ColorEdit(QWidget *parent, QMdiArea *displayArea)
    : QWidget(parent), displayArea(displayArea)
{
        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->setMargin(0);
        layout->setSpacing(0);
        theLineEdit = new QLineEdit(this);
        theLineEdit->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
        QToolButton *button = new QToolButton(this);
        button->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));
        button->setText(QLatin1String("..."));
        layout->addWidget(theLineEdit);
        layout->addWidget(button);
        theLineEdit->setFocusProxy(this);
        setFocusPolicy(Qt::StrongFocus);
        setAttribute(Qt::WA_InputMethodEnabled);

        color_.mode = ColorPickerColor::Tristimulus;
        color_.tristimulus = QColor(255,122,64);

        /*connect(theLineEdit, SIGNAL(textEdited(const QString &)),
                this, SIGNAL(filePathChanged(const QString &)));*/
        connect(button, SIGNAL(clicked()),
                this, SLOT(buttonClicked()));
}

void ColorEdit::buttonClicked() {
        ColorPicker *picker = new ColorPicker(0);
        picker->setColor (color_);
        if (QDialog::Accepted == picker->exec() || true) {
                //emit filePathChanged("okay");

                if (color_ != picker->color()) {
                        color_ = picker->color();
                        emit colorChanged(color_);
                        if (color_.mode == ColorPickerColor::Tristimulus) {
                                theLineEdit->setText(
                                        QString::number(color_.tristimulus.red())
                                        + ":" + QString::number(color_.tristimulus.green())
                                        + ":" + QString::number(color_.tristimulus.blue()));
                        } else {
                                theLineEdit->setText("spectral{...}");
                        }
                }
        }
        delete picker;

    /*QString filePath = QFileDialog::getOpenFileName(this, tr("Choose a file"), theLineEdit->text(), theFilter);
    if (filePath.isNull())
        return;
    theLineEdit->setText(filePath);
    emit filePathChanged(filePath);*/
}

void ColorEdit::setColor (ColorPickerColor const &col) {
        color_ = col;
}

ColorPickerColor ColorEdit::color () const {
        return color_;
}

void ColorEdit::focusInEvent(QFocusEvent *e) {
        theLineEdit->event(e);
        if (e->reason() == Qt::TabFocusReason || e->reason() == Qt::BacktabFocusReason) {
                theLineEdit->selectAll();
        }
        QWidget::focusInEvent(e);
}

void ColorEdit::focusOutEvent(QFocusEvent *e) {
        theLineEdit->event(e);
        QWidget::focusOutEvent(e);
}

void ColorEdit::keyPressEvent(QKeyEvent *e) {
        theLineEdit->event(e);
}

void ColorEdit::keyReleaseEvent(QKeyEvent *e) {
        theLineEdit->event(e);
}
