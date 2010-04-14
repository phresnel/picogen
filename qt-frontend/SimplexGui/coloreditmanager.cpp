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

// based on qt quarterly 18

#include "coloreditmanager.h"

ColorPickerColor ColorEditManager::value(const QtProperty *property) const
{
    if (!theValues.contains(property))
        return ColorPickerColor();
    return theValues[property].value;
}

/*QString ColorEditManager::filter(const QtProperty *property) const
{
    if (!theValues.contains(property))
        return QString();
    return theValues[property].filter;
}*/

void ColorEditManager::setValue(QtProperty *property, const ColorPickerColor &val)
{
    if (!theValues.contains(property))
        return;

    Data data = theValues[property];

    if (data.value == val)
        return;

    data.value = val;

    theValues[property] = data;

    emit propertyChanged(property);
    emit valueChanged(property, data.value);
}

/*void ColorEditManager::setFilter(QtProperty *property, const QString &fil)
{
    if (!theValues.contains(property))
        return;

    Data data = theValues[property];

    if (data.filter == fil)
        return;

    data.filter = fil;

    theValues[property] = data;

    emit filterChanged(property, data.filter);
}*/

