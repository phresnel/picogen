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

#ifndef COLOREDITFACTORY_H
#define COLOREDITFACTORY_H

#include "qtpropertybrowser.h"
#include "coloreditmanager.h"

class ColorEdit;

class ColorEditFactory : public QtAbstractEditorFactory<ColorEditManager>
{
    Q_OBJECT
public:
    ColorEditFactory(QObject *parent = 0)
        : QtAbstractEditorFactory<ColorEditManager>(parent)
            { }
    virtual ~ColorEditFactory();
protected:
    virtual void connectPropertyManager(ColorEditManager *manager);
    virtual QWidget *createEditor(ColorEditManager *manager, QtProperty *property,
                QWidget *parent);
    virtual void disconnectPropertyManager(ColorEditManager *manager);
private slots:
    void slotPropertyChanged(QtProperty *property, const QString &value);
    void slotFilterChanged(QtProperty *property, const QString &filter);
    void slotSetValue(const QString &value);
    void slotEditorDestroyed(QObject *object);
private:
    QMap<QtProperty *, QList<ColorEdit *> > theCreatedEditors;
    QMap<ColorEdit *, QtProperty *> theEditorToProperty;
};

#endif
