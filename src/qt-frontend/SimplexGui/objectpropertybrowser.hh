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
                class Scene;

                class Material;
                class Color;
        }
}
class QMdiArea;
class QtTreePropertyBrowser;
class QtGroupPropertyManager;
class QtProperty;
class QtEnumPropertyManager;
class QtVariantPropertyManager;
class QtVariantEditorFactory;
class ColorEditManager;
class ColorEditFactory;
class ColorPickerColor;



#include <QObject>
#include <QList>



class ObjectPropertyBrowser : public QObject
{
        Q_OBJECT
public:
        ObjectPropertyBrowser(QWidget *ownerWidget,
                              QMdiArea *displayArea,
                              QtTreePropertyBrowser *root,
                              QtVariantPropertyManager *codeEditManager
                             );
        ~ObjectPropertyBrowser();

        void addObject (redshift::scenefile::Object const &);
        void remove (QtProperty *);

        void addObjectsToScene (redshift::scenefile::Scene &Scene) const;

signals:
        void updateUi();
        void sceneChanged();

private:
        void initializeScene();

        redshift::scenefile::Material readMaterial (QList<QtProperty*> subs, QString name="material") const;
        redshift::scenefile::Color readColor (QList<QtProperty*> subs, QString name="color") const;

        QWidget *ownerWidget;
        QMdiArea *displayArea;

        QtTreePropertyBrowser *root;
        QtEnumPropertyManager *objectTypeEnumManager;
        QtGroupPropertyManager *groupManager;
        QtVariantPropertyManager *variantManager, *codeEditManager;
        QtVariantEditorFactory *variantFactory;
        QtProperty *objectsProperty;

        ColorEditManager *colorEditManager;
        ColorEditFactory *colorEditFactory;

private slots:
        void objectTypeEnumManager_valueChanged(QtProperty*, int);
        void colorEditManager_valueChanged(QtProperty*,ColorPickerColor const&);
};

#endif // OBJECTPROPERTYBROWSER_HH
