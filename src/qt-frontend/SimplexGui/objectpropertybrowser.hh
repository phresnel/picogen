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

#ifndef OBJECTPROPERTYBROWSER_HH
#define OBJECTPROPERTYBROWSER_HH

namespace redshift {
        namespace scenefile {
                class Object;
        }
}
class QtTreePropertyBrowser;
class QtGroupPropertyManager;
class QtProperty;
class QtEnumPropertyManager;
class QtVariantPropertyManager;
class ColorEditManager;

#include <QObject>


class ObjectPropertyBrowser : public QObject
{
        Q_OBJECT
public:
        ObjectPropertyBrowser(QWidget *,
                              QtTreePropertyBrowser *,
                              QtGroupPropertyManager *,
                              QtVariantPropertyManager *,
                              QtVariantPropertyManager *codeEd,
                              ColorEditManager *
                             );
        ~ObjectPropertyBrowser();
        void addObject (redshift::scenefile::Object const &);
        void remove (QtProperty *);

signals:
        void updateUi();
        void sceneChanged();

private:
        void initializeScene();

        QWidget *ownerWidget;
        QtTreePropertyBrowser *root;
        QtEnumPropertyManager *objectTypeEnumManager;
        QtGroupPropertyManager *groupManager;
        QtVariantPropertyManager *variantManager, *codeEditManager;
        QtProperty *objectsProperty;
        ColorEditManager *colorEditManager;

private slots:
        void objectTypeEnumManager_valueChanged(QtProperty*, int);
};

#endif // OBJECTPROPERTYBROWSER_HH
