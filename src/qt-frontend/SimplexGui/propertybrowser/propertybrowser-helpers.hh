#ifndef PROPERTYBROWSERHELPERS_HH
#define PROPERTYBROWSERHELPERS_HH

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

#include <QtProperty>
#include <QtTreePropertyBrowser>
#include <QtVariantProperty>

// Some helper functions for the lack of a method in QtProperty
// that lets us select sub-items directly by name.

template <typename T>
T readValue (QString name, QList<QtProperty*> list) {
        QtProperty* retty = 0;
        foreach (QtProperty* looky, list) {
                if (name == looky->propertyName()) {
                        retty = looky;
                        break;
                }
        }
        if (0 != retty) {
                const QtVariantProperty *v = (QtVariantProperty*)retty;
                const QVariant vv = v->value();
                const T ret = vv.value<T>();
                return ret;
        }
        return T();
}
template <typename T>
T readValue (QString name, QtProperty* prop) {
        return readValue<T> (name, prop->subProperties());
}



template <typename T>
void writeValue (QString name, QList<QtProperty*> list, T const &val) {
        QtProperty* retty = 0;
        foreach (QtProperty* looky, list) {
                if (name == looky->propertyName()) {
                        retty = looky;
                        break;
                }
        }
        if (0 != retty) {
                QtVariantProperty *v = (QtVariantProperty*)retty;
                v->setValue(val);
        }
}
template <typename T>
void writeValue (QString name, QtProperty* prop, T const &val) {
        return writeValue<T> (name, prop->subProperties(), val);
}

QString readValueText (QString name, QList<QtProperty*> list);
QString readValueText (QString name, QtProperty *prop) ;
QList<QtProperty*> readSubProperties (QString name, QList<QtProperty*> list);
QList<QtProperty*> readSubProperties (QString name, QtProperty* prop);
QtProperty* readSubProperty (QString name, QList<QtProperty*> list);
QtProperty* readSubProperty (QString name, QtProperty* prop);
QtProperty *findParent(QtProperty *root, QtProperty* child);
QtProperty *findParent(QList<QtProperty*> props, QtProperty* child);
void collapse (QtTreePropertyBrowser *browser,
               QtProperty* property
);
bool isFirst(QtProperty *root, QtBrowserItem* node);
bool isLast(QtProperty *root, QtBrowserItem* node);

void moveUp(QtTreePropertyBrowser *browser, QtProperty *root, QtBrowserItem* node);
void moveDown(QtTreePropertyBrowser *browser, QtProperty *root, QtBrowserItem* node);

//QtBrowserItem* toBrowserItem (QtProperty *);

#endif // PROPERTYBROWSERHELPERS_HH
