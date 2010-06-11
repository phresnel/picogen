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

// Picogen.
#include "redshift/include/jobfile.hh"

// Qt.
#include <QWidget>
#include <QVariant>
#include <QMessageBox>

// PropertyBrowser.
#include <QtProperty>
#include <QtTreePropertyBrowser>


// qt-frontend.
#include "objectpropertybrowser.hh"
#include "volumepropertybrowser.hh"
#include "rendersettingspropertybrowser.hh"
#include "cameraspropertybrowser.hh"
#include "filmsettingspropertybrowser.hh"
#include "backgroundspropertybrowser.hh"
#include "propertybrowser-helpers.hh"

// Include.
#include "scenepropertybrowser.hh"



ScenePropertyBrowser::ScenePropertyBrowser(
        QWidget *ownerWidget,
        QMdiArea *displayArea,
        QtTreePropertyBrowser *root,
        QtVariantPropertyManager *codeEditManager
)
: QObject(ownerWidget)
, ownerWidget(ownerWidget)
, displayArea(displayArea)
, root(root)
, codeEditManager(codeEditManager)
, objectPropertyBrowser(0)
, volumePropertyBrowser(0)
, renderSettingsPropertyBrowser(0)
, camerasPropertyBrowser(0)
, filmSettingsPropertyBrowser(0)
, backgroundsPropertyBrowser(0)
{
        initializeBrowsers();
}



ScenePropertyBrowser::~ScenePropertyBrowser() {
        delete objectPropertyBrowser;
        delete volumePropertyBrowser;
        delete renderSettingsPropertyBrowser;
        delete camerasPropertyBrowser;
        delete filmSettingsPropertyBrowser;
        delete backgroundsPropertyBrowser;
}



void ScenePropertyBrowser::initializeBrowsers() {
        root->clear();

        connect (root, SIGNAL(currentItemChanged(QtBrowserItem*)),
                 this, SLOT(currentItemChanged(QtBrowserItem*)));
        root->setProperty("picohelp", "SimplexGUI_Property_Editor.html");

        delete filmSettingsPropertyBrowser;
        filmSettingsPropertyBrowser = new FilmSettingsPropertyBrowser(
                        ownerWidget,
                        root);
        connect(filmSettingsPropertyBrowser, SIGNAL(sceneChanged()), this, SLOT(sceneChanged_()));


        delete renderSettingsPropertyBrowser;
        renderSettingsPropertyBrowser = new RenderSettingsPropertyBrowser(
                        ownerWidget,
                        root);
        connect(renderSettingsPropertyBrowser, SIGNAL(sceneChanged()), this, SLOT(resyncRenderSettingConfig_()));
        connect(renderSettingsPropertyBrowser, SIGNAL(sceneChanged()), this, SLOT(sceneChanged_()));


        delete camerasPropertyBrowser;
        camerasPropertyBrowser = new CamerasPropertyBrowser(ownerWidget,
                                                            root);
        connect(camerasPropertyBrowser, SIGNAL(sceneChanged()), this, SLOT(resyncCameraConfig_()));
        connect(camerasPropertyBrowser, SIGNAL(sceneChanged()), this, SLOT(sceneChanged_()));


        delete objectPropertyBrowser;
        objectPropertyBrowser = new ObjectPropertyBrowser(ownerWidget,
                                                          displayArea,
                                                          root,
                                                          codeEditManager
                                                          );
        connect(objectPropertyBrowser, SIGNAL(sceneChanged()), this, SLOT(sceneChanged_()));


        delete volumePropertyBrowser;
        volumePropertyBrowser = new VolumePropertyBrowser (ownerWidget,
                                                           displayArea,
                                                           root,
                                                           codeEditManager);
        connect(volumePropertyBrowser, SIGNAL(sceneChanged()), this, SLOT(sceneChanged_()));


        delete backgroundsPropertyBrowser;
        backgroundsPropertyBrowser = new BackgroundsPropertyBrowser(
                                        ownerWidget, displayArea, root);
        connect(backgroundsPropertyBrowser, SIGNAL(sceneChanged()), this, SLOT(sceneChanged_()));
}



void ScenePropertyBrowser::loadScene (redshift::scenefile::Scene const &scene) {
        const bool signalsBlocked = blockSignals(true);

        initializeBrowsers();

        filmSettingsPropertyBrowser->setFilmSettings(scene.filmSettings());


        for (unsigned int i=0; i<scene.renderSettingsCount(); ++i)
                renderSettingsPropertyBrowser->addRenderSettings(scene.renderSettings(i));
        for (unsigned int i=0; i<scene.cameraCount(); ++i)
                camerasPropertyBrowser->addCamera(scene.camera(i));
        for (unsigned int i=0; i<scene.objectCount(); ++i)
                objectPropertyBrowser->addObject(scene.object(i));
        for (unsigned int i=0; i<scene.volumeCount(); ++i)
                volumePropertyBrowser->addVolume(scene.volume(i));
        if (scene.backgroundCount())
                backgroundsPropertyBrowser->setBackground(scene.background(0));


        foreach (QtBrowserItem *it, root->topLevelItems())
                root->setExpanded(it, false);

        blockSignals(signalsBlocked);
        emit sceneChanged();
        emit asUnchanged();
}



void ScenePropertyBrowser::currentItemChanged(QtBrowserItem *current) {

        currentBrowserItem_ = current;
        QString name = (current==0) ? "" : current->property()->propertyName();

        QtProperty *parentProp = (current==0)
                               ? 0
                               : findParent(root->properties(),
                                            current->property());
        QtProperty *parentParentProp = (parentProp==0)
                               ? 0
                               : findParent(root->properties(),
                                            parentProp);

        const bool isCode = name == "code";


        const bool isTransform      = name  == "transform";
        const bool isSubTransform   = (parentProp != 0)
                                      && (parentProp->propertyName()
                                          == "transform");
        const bool isCamera         = (parentProp != 0)
                                      && (parentProp->propertyName() == "cameras");
        const bool isRenderSetting  = (parentProp != 0)
                                      && (parentProp->propertyName()
                                          == "render-settings");

        currentCameraProperty_        = 0;
        currentTransformProperty_     = 0;
        currentRenderSettingProperty_ = 0;

        // FROB
        if (isSubTransform) {
                currentCameraProperty_    = parentParentProp;
                currentTransformProperty_ = parentProp;
        }
        if (isTransform) {
                currentCameraProperty_    = parentProp;
                currentTransformProperty_ = current->property();
        }
        if (isCamera) {
                currentCameraProperty_    = current->property();
        }
        if (isRenderSetting) {
                currentRenderSettingProperty_ = current->property();
        }
}



QtBrowserItem *ScenePropertyBrowser::currentBrowserItem () const {
        return currentBrowserItem_;
}



QtProperty *ScenePropertyBrowser::currentCameraProperty () const {
        return currentCameraProperty_;
}



QtProperty *ScenePropertyBrowser::currentTransformProperty () const {
        return currentTransformProperty_;
}



QtProperty *ScenePropertyBrowser::currentRenderSettingProperty () const {
        return currentRenderSettingProperty_;
}




void ScenePropertyBrowser::setDefaultScene() {

        const bool signalsBlocked = blockSignals(true);

        // changed = true;
        initializeBrowsers();

        // - render settings
        const bool blockRenderSettings = renderSettingsPropertyBrowser->blockSignals(true);
        {
                using namespace redshift::scenefile;

                RenderSettings rs;
                rs.width = 320;
                rs.height = 240;
                rs.min_y = 0;
                rs.max_y = 0xFFFFF;
                rs.samplesPerPixel = 1;
                rs.surfaceIntegrator.type = SurfaceIntegrator::whitted;
                rs.surfaceIntegrator.numAmbientSamples = 10;
                rs.userSeed = 0;
                rs.volumeIntegrator.type = VolumeIntegrator::none;
                rs.volumeIntegrator.cutoffDistance = 5000;
                rs.volumeIntegrator.stepSize = 400;
                rs.title = "preview";

                renderSettingsPropertyBrowser->addRenderSettings(rs);
        }
        {
                using namespace redshift::scenefile;

                RenderSettings rs;
                rs.width = 800;
                rs.height = 600;
                rs.min_y = 0;
                rs.max_y = 0xFFFFF;
                rs.samplesPerPixel = 10;
                rs.surfaceIntegrator.type = SurfaceIntegrator::path;
                rs.surfaceIntegrator.numAmbientSamples = 10;
                rs.userSeed = 0;
                rs.volumeIntegrator.type = VolumeIntegrator::none;
                rs.volumeIntegrator.cutoffDistance = 50000;
                rs.volumeIntegrator.stepSize = 70;
                rs.title = "production";

                renderSettingsPropertyBrowser->addRenderSettings(rs);
        }
        renderSettingsPropertyBrowser->blockSignals(blockRenderSettings);

        // - camera
        const bool cameraSigsBlocked = camerasPropertyBrowser->blockSignals(true);
        redshift::scenefile::Camera cam;
        cam.title = "hello-world";
        cam.type = redshift::scenefile::Camera::pinhole;
        cam.pinholeParams.front = 1;

        redshift::scenefile::Transform t;
        t.type = redshift::scenefile::Transform::move;
        t.y = 1;
        cam.transforms.push_back(t);
        t = redshift::scenefile::Transform();
        t.type = redshift::scenefile::Transform::yaw;
        cam.transforms.push_back(t);
        t.type = redshift::scenefile::Transform::pitch;
        cam.transforms.push_back(t);
        t.type = redshift::scenefile::Transform::roll;
        cam.transforms.push_back(t);

        camerasPropertyBrowser->addCamera(cam);

        camerasPropertyBrowser->blockSignals(cameraSigsBlocked);

        // - objects
        /*redshift::scenefile::Object o;
        o.type = redshift::scenefile::Object::horizon_plane;
        o.horizonPlaneParams.height = 0;
        o.horizonPlaneParams.material.color.type = redshift::scenefile::Color::RGB;
        o.horizonPlaneParams.material.color.rgb.r = 0.6;
        o.horizonPlaneParams.material.color.rgb.g = 0.5;
        o.horizonPlaneParams.material.color.rgb.b = 0.4;
        addObject(o);*/

        // - background
        const bool bgblocked = backgroundsPropertyBrowser->blockSignals(true);
        backgroundsPropertyBrowser->setBackground (redshift::scenefile::Background());
        backgroundsPropertyBrowser->blockSignals(bgblocked);


        foreach (QtBrowserItem *it, root->topLevelItems())
                root->setExpanded(it, false);

        blockSignals(signalsBlocked);
        emit sceneChanged();
        emit asUnchanged();
}



redshift::shared_ptr<redshift::scenefile::Scene>
ScenePropertyBrowser::createScene () const
{
        using namespace redshift;

        shared_ptr<scenefile::Scene> scene = shared_ptr<scenefile::Scene>(
                        new scenefile::Scene());

        filmSettingsPropertyBrowser->addFilmSettingsToScene(*scene);
        renderSettingsPropertyBrowser->addRenderSettingsToScene(*scene);
        camerasPropertyBrowser->addCamerasToScene(*scene);
        objectPropertyBrowser->addObjectsToScene(*scene);
        volumePropertyBrowser->addVolumesToScene(*scene);
        backgroundsPropertyBrowser->addBackgroundsToScene(*scene);

        return scene;
}



QString ScenePropertyBrowser::sceneToCode() {
        QString tehCodes;
        try {
                using namespace actuarius;
                using namespace redshift;

                const shared_ptr<scenefile::Scene> scene = createScene ();
                std::stringstream ss;
                OArchive (ss) & pack("scene", *scene);
                return QString::fromStdString(ss.str());
        } catch (std::exception const &e) {
                QMessageBox::critical(ownerWidget, "Critical",
                                      e.what());
                return "";
        }
}



/*void PropertyBrowser::removeCurrent() {
        if (currentCameraProperty_ != 0)
                camerasPropertyBrowser->remove(currentCameraProperty_);
        else if (currentRenderSettingProperty_ != 0)
                renderSettingsPropertyBrowser->remove(currentCameraProperty_);
}*/



void ScenePropertyBrowser::removeRenderSetting (QtProperty *property) {
        renderSettingsPropertyBrowser->remove(property);
}



void ScenePropertyBrowser::removeCamera (QtProperty *property) {
        camerasPropertyBrowser->remove(property);
}



void ScenePropertyBrowser::removeObject(QtProperty *property) {
        objectPropertyBrowser->remove(property);
}



void ScenePropertyBrowser::removeVolume(QtProperty *property) {
        volumePropertyBrowser->remove(property);
}



void ScenePropertyBrowser::removeTransform(QtProperty *transformRoot,
                                      QtProperty *subTransform
) {
        camerasPropertyBrowser->removeTransform(transformRoot, subTransform);
}



void ScenePropertyBrowser::addObject (redshift::scenefile::Object const &ob) {
        objectPropertyBrowser->addObject(ob);
}



void ScenePropertyBrowser::addVolume (redshift::scenefile::Volume const &vol) {
        volumePropertyBrowser->addVolume(vol);
}



void ScenePropertyBrowser::addTransform(QtProperty *transformRoot,
                                   redshift::scenefile::Transform const & t
) {
        camerasPropertyBrowser->addTransform(transformRoot, t);
}



void ScenePropertyBrowser::addRenderSettings (redshift::scenefile::RenderSettings const &rs) {
        renderSettingsPropertyBrowser->addRenderSettings(rs);
}



void ScenePropertyBrowser::addCamera (redshift::scenefile::Camera const &cam) {
        camerasPropertyBrowser->addCamera(cam);
}



QStringList ScenePropertyBrowser::renderSettingNames() const {
        return renderSettingsPropertyBrowser->names();
}



QStringList ScenePropertyBrowser::cameraNames() const {
        return camerasPropertyBrowser->names();
}



/*void ScenePropertyBrowser::updateUi_() {
        if (!signalsBlocked())
                emit updateUi();
}*/



void ScenePropertyBrowser::sceneChanged_() {
        if (!signalsBlocked())
                emit sceneChanged();
}


void ScenePropertyBrowser::resyncCameraConfig_() {
        if (!signalsBlocked())
                emit resyncCameraConfig();
}


void ScenePropertyBrowser::resyncRenderSettingConfig_() {
        if (!signalsBlocked())
                emit resyncRenderSettingConfig();
}

