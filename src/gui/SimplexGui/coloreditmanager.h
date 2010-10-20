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

// qt quarterly 18

#ifndef COLORMANAGER_H
#define COLORMANAGER_H

#include "qtpropertybrowser.h"
#include <QMap>

#include "colorpicker.hh"

class ColorEditManager : public QtAbstractPropertyManager
{
    Q_OBJECT
public:
    ColorEditManager(QObject *parent = 0)
        : QtAbstractPropertyManager(parent)
            { }

    /*QString value(const QtProperty *property) const;
    QString filter(const QtProperty *property) const;*/

    ColorPickerColor value (const QtProperty *property) const;


public slots:
    void setValue(QtProperty *property, const ColorPickerColor &val);
    //void setFilter(QtProperty *property, const QString &fil);
signals:
    void valueChanged(QtProperty *property, const ColorPickerColor &val);
    //void filterChanged(QtProperty *property, const QString &fil);
protected:
    virtual QString valueText(const QtProperty *property) const ;
    virtual QIcon valueIcon(const QtProperty *property) const ;
    virtual void initializeProperty(QtProperty *property) { theValues[property] = Data(); }
    virtual void uninitializeProperty(QtProperty *property) { theValues.remove(property); }
private:

    struct Data
    {
        ColorPickerColor value;
    };

    QMap<const QtProperty *, Data> theValues;
};


#endif
