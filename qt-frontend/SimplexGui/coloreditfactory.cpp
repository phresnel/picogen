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

#include "coloreditfactory.h"
#include "coloredit.h"

ColorEditFactory::~ColorEditFactory()
{
    QList<ColorEdit *> editors = theEditorToProperty.keys();
    QListIterator<ColorEdit *> it(editors);
    while (it.hasNext())
        delete it.next();
}

void ColorEditFactory::connectPropertyManager(ColorEditManager *manager)
{
    connect(manager, SIGNAL(valueChanged(QtProperty *, const QString &)),
                this, SLOT(slotPropertyChanged(QtProperty *, const QString &)));
    connect(manager, SIGNAL(filterChanged(QtProperty *, const QString &)),
                this, SLOT(slotFilterChanged(QtProperty *, const QString &)));
}

QWidget *ColorEditFactory::createEditor(ColorEditManager *manager,
        QtProperty *property, QWidget *parent)
{
    ColorEdit *editor = new ColorEdit(parent);
    editor->setFilePath(manager->value(property));
    editor->setFilter(manager->filter(property));
    theCreatedEditors[property].append(editor);
    theEditorToProperty[editor] = property;

    connect(editor, SIGNAL(filePathChanged(const QString &)),
                this, SLOT(slotSetValue(const QString &)));
    connect(editor, SIGNAL(destroyed(QObject *)),
                this, SLOT(slotEditorDestroyed(QObject *)));
    return editor;
}

void ColorEditFactory::disconnectPropertyManager(ColorEditManager *manager)
{
    disconnect(manager, SIGNAL(valueChanged(QtProperty *, const QString &)),
                this, SLOT(slotPropertyChanged(QtProperty *, const QString &)));
    disconnect(manager, SIGNAL(filterChanged(QtProperty *, const QString &)),
                this, SLOT(slotFilterChanged(QtProperty *, const QString &)));
}

void ColorEditFactory::slotPropertyChanged(QtProperty *property,
                const QString &value)
{
    if (!theCreatedEditors.contains(property))
        return;

    QList<ColorEdit *> editors = theCreatedEditors[property];
    QListIterator<ColorEdit *> itEditor(editors);
    while (itEditor.hasNext())
        itEditor.next()->setFilePath(value);
}

void ColorEditFactory::slotFilterChanged(QtProperty *property,
            const QString &filter)
{
    if (!theCreatedEditors.contains(property))
        return;

    QList<ColorEdit *> editors = theCreatedEditors[property];
    QListIterator<ColorEdit *> itEditor(editors);
    while (itEditor.hasNext())
        itEditor.next()->setFilter(filter);
}

void ColorEditFactory::slotSetValue(const QString &value)
{
    QObject *object = sender();
    QMap<ColorEdit *, QtProperty *>::ConstIterator itEditor =
                theEditorToProperty.constBegin();
    while (itEditor != theEditorToProperty.constEnd()) {
        if (itEditor.key() == object) {
            QtProperty *property = itEditor.value();
            ColorEditManager *manager = propertyManager(property);
            if (!manager)
                return;
            manager->setValue(property, value);
            return;
        }
        itEditor++;
    }
}

void ColorEditFactory::slotEditorDestroyed(QObject *object)
{
    QMap<ColorEdit *, QtProperty *>::ConstIterator itEditor =
                theEditorToProperty.constBegin();
    while (itEditor != theEditorToProperty.constEnd()) {
        if (itEditor.key() == object) {
            ColorEdit *editor = itEditor.key();
            QtProperty *property = itEditor.value();
            theEditorToProperty.remove(editor);
            theCreatedEditors[property].removeAll(editor);
            if (theCreatedEditors[property].isEmpty())
                theCreatedEditors.remove(property);
            return;
        }
        itEditor++;
    }
}

