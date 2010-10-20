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

// see qt quarterly 18

#ifndef COLOREDIT_H
#define COLOREDIT_H

#include <QLineEdit>
#include <QMdiArea>
#include <QLabel>
#include "colorpicker.hh"

class ColorEdit : public QWidget
{
    Q_OBJECT
public:
    ColorEdit(QWidget *parent = 0, QMdiArea *displayArea = 0);
    //void setFilePath(const QString &filePath) { if (theLineEdit->text() != filePath) theLineEdit->setText(filePath); }
    //QString filePath() const { return theLineEdit->text(); }
    //void setFilter(const QString &filter) { theFilter = filter; }
    //QString filter() const { return theFilter; }

    void setColor (ColorPickerColor const &);
    ColorPickerColor color () const ;
signals:
    void colorChanged(const ColorPickerColor &);
protected:
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
private slots:
    void buttonClicked();
private:
    //QLabel *theLabel;
    //QLineEdit *theLineEdit;
    //QString theFilter;

    QMdiArea *displayArea;

    ColorPickerColor color_;
};

#endif
