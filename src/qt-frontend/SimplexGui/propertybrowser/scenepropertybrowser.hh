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

#ifndef SCENEPROPERTYBROWSER_HH
#define SCENEPROPERTYBROWSER_HH

// Picogen.
namespace redshift {
        namespace scenefile {
                class Scene;
                class RenderSettings;
                class Camera;
                class Object;
                class Volume;
                class Transform;
        }
}

// Qt.
class QMdiArea;

// PropertyBrowser.
class QtProperty;
class QtBrowserItem;
class QtTreePropertyBrowser;
class QtVariantPropertyManager;

// qt-frontend.
class ObjectPropertyBrowser;
class VolumePropertyBrowser;
class RenderSettingsPropertyBrowser;
class CamerasPropertyBrowser;
class FilmSettingsPropertyBrowser;
class BackgroundsPropertyBrowser;

// Include.
#include "redshift/include/smart_ptr.hh"
#include <QObject>



// Type.
class ScenePropertyBrowser : public QObject {
        Q_OBJECT
public:
        ScenePropertyBrowser(QWidget *ownerWidget,
                        QMdiArea *displayArea,
                        QtTreePropertyBrowser *root,
                        QtVariantPropertyManager *codeEditManager
                        );
        ~ScenePropertyBrowser();

        void setDefaultScene();
        void loadScene (redshift::scenefile::Scene const &scene);

        redshift::shared_ptr<redshift::scenefile::Scene> createScene () const;
        QString sceneToCode();

        QStringList renderSettingNames() const;
        QStringList cameraNames() const;

        void addRenderSettings (redshift::scenefile::RenderSettings const &rs);
        void addCamera (redshift::scenefile::Camera const &cam);
        void addObject (redshift::scenefile::Object const &ob);
        void addVolume (redshift::scenefile::Volume const &vol);

        void addTransform(QtProperty *transformRoot,
                          redshift::scenefile::Transform const & t);

        void removeRenderSetting (QtProperty *);
        void removeCamera (QtProperty *);
        void removeObject (QtProperty *);
        void removeVolume (QtProperty *);

        void removeTransform(QtProperty *transformRoot,
                             QtProperty *subTransform);

        QtBrowserItem *currentBrowserItem () const;
        QtProperty *currentCameraProperty () const;
        QtProperty *currentTransformProperty () const;
        QtProperty *currentRenderSettingProperty () const;

signals:
        void sceneChanged();
        void resyncCameraConfig();
        void resyncRenderSettingConfig();
        void asUnchanged();

private:
        void initializeBrowsers();

        QWidget *ownerWidget;
        QMdiArea *displayArea;
        QtTreePropertyBrowser *root;
        QtVariantPropertyManager *codeEditManager;

        ObjectPropertyBrowser *objectPropertyBrowser;
        VolumePropertyBrowser *volumePropertyBrowser;
        RenderSettingsPropertyBrowser *renderSettingsPropertyBrowser;
        CamerasPropertyBrowser *camerasPropertyBrowser;
        FilmSettingsPropertyBrowser *filmSettingsPropertyBrowser;
        BackgroundsPropertyBrowser *backgroundsPropertyBrowser;


        QtProperty *currentCameraProperty_,
                   *currentTransformProperty_,
                   *currentRenderSettingProperty_;
        QtBrowserItem * currentBrowserItem_;

private slots:
        //void updateUi_();
        void sceneChanged_();
        void resyncCameraConfig_();
        void resyncRenderSettingConfig_();

        void currentItemChanged(QtBrowserItem * current);
};

#endif // SCENEPROPERTYBROWSER_HH
