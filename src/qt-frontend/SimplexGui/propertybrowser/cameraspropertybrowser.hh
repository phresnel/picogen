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

#ifndef CAMERASPROPERTYBROWSER_HH
#define CAMERASPROPERTYBROWSER_HH

namespace redshift {
        namespace scenefile {
                class Camera;
                class Transform;
                class Scene;
        }
}
class QtTreePropertyBrowser;
class QtGroupPropertyManager;
class QtProperty;
class QtEnumPropertyManager;
class QtEnumEditorFactory;
class QtVariantPropertyManager;
class QtVariantEditorFactory;
class QtStringPropertyManager;
class ColorEditManager;

#include <QStringList>
#include <QObject>

class CamerasPropertyBrowser : public QObject {
        Q_OBJECT
public:
        CamerasPropertyBrowser(QWidget *,
                               QtTreePropertyBrowser *
                              );
        ~CamerasPropertyBrowser();

        void addCamera (redshift::scenefile::Camera const&);
        void addTransform (QtProperty *transformRoot,
                           redshift::scenefile::Transform const & t);
        void remove (QtProperty *);
        void removeTransform (QtProperty *transformRoot,
                              QtProperty *subTransform);

        void addCamerasToScene (
                redshift::scenefile::Scene &scene
        ) const;

        QStringList names() const;

signals:
        void updateUi();
        void sceneChanged();

private:
        void initializeScene();

        QWidget *ownerWidget;
        QtTreePropertyBrowser *root;
        QtGroupPropertyManager *groupManager;
        QtVariantPropertyManager *variantManager;
        QtVariantEditorFactory *variantFactory;
        QtProperty *camerasProperty;

        QtEnumEditorFactory *comboBoxFactory;

        QtEnumPropertyManager
                        *transformEnumManager,
                        *cameraTypeEnumManager;

        QtStringPropertyManager *titleManager;

private slots:
        void cameraTypeEnumManager_valueChanged(QtProperty*, int);
        void titleManager_valueChanged (QtProperty *, const QString &);
        void transformEnumManager_valueChanged(QtProperty*, int);
        void variantManager_valueChanged(QtProperty*, QVariant const &);
};

#endif // CAMERASPROPERTYBROWSER_HH
