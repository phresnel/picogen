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

#include <QStringList>
#include <QMessageBox>

#include <QtTreePropertyBrowser>
#include <QtProperty>
#include <QtGroupPropertyManager>
#include <QtVariantPropertyManager>
#include <QtEnumPropertyManager>
#include <QtEnumEditorFactory>
#include <QList>

#include "redshift/include/jobfile.hh"
#include "propertybrowser-helpers.hh"

#include "cameraspropertybrowser.hh"

CamerasPropertyBrowser::CamerasPropertyBrowser(
        QWidget *ownerWidget_,
        QtTreePropertyBrowser *root,
        QtGroupPropertyManager *groupManager,
        QtVariantPropertyManager *variantManager,
        QtVariantPropertyManager *codeEditManager,
        ColorEditManager *colorEditManager
        )
: QObject(ownerWidget_)
, ownerWidget(ownerWidget_)
, root(root)
, groupManager(groupManager)
, variantManager(variantManager)
, camerasProperty(0)
{
        initializeScene();
}



QList<QtProperty*> CamerasPropertyBrowser::subProperties() {
        return camerasProperty->subProperties();
}



void CamerasPropertyBrowser::initializeScene() {
        comboBoxFactory = new QtEnumEditorFactory(this);


        transformEnumManager = new QtEnumPropertyManager(this);
        connect(transformEnumManager, SIGNAL(valueChanged (QtProperty *, int)),
                this, SLOT(transformEnumManager_valueChanged(QtProperty*,int)));

        cameraTypeEnumManager = new QtEnumPropertyManager(this);
        connect(cameraTypeEnumManager, SIGNAL(valueChanged (QtProperty *, int)),
                this, SLOT(cameraTypeEnumManager_valueChanged(QtProperty*,int)));

        root->setFactoryForManager(transformEnumManager, comboBoxFactory);
        root->setFactoryForManager(cameraTypeEnumManager, comboBoxFactory);


        camerasProperty = groupManager->addProperty("cameras");
        root->addProperty(camerasProperty);
        root->setBackgroundColor(
                        root->topLevelItem(camerasProperty),
                        QColor(110,110,110));
}



void CamerasPropertyBrowser::remove(QtProperty *property) {
        camerasProperty->removeSubProperty(property);
}



void CamerasPropertyBrowser::addCamera(redshift::scenefile::Camera const& c) {
        using redshift::scenefile::Camera;

        QtProperty *camera = groupManager->addProperty(c.title.c_str());
        camerasProperty->addSubProperty(camera);

        QtProperty *cameraType = cameraTypeEnumManager->addProperty("type");
        QStringList enumNames;
        enumNames << "pinhole"
                  << "cubemap_left"
                  << "cubemap_right"
                  << "cubemap_bottom"
                  << "cubemap_top"
                  << "cubemap_front"
                  << "cubemap_back"
                  ;
        cameraTypeEnumManager->setEnumNames(cameraType, enumNames);
        camera->addSubProperty(cameraType);

        // TODO: should not be hardcoded, enumNames should directly correspond
        //       to redshift types
        switch (c.type) {
        case Camera::pinhole:
                cameraTypeEnumManager->setValue(cameraType, 0);
                cameraTypeEnumManager_valueChanged(cameraType, 0);
                writeValue("front", camera, c.pinholeParams.front);
                break;
        case Camera::cubemap_left:
                cameraTypeEnumManager->setValue(cameraType, 1);
                cameraTypeEnumManager_valueChanged(cameraType, 1);
                break;
        case Camera::cubemap_right:
                cameraTypeEnumManager->setValue(cameraType, 2);
                cameraTypeEnumManager_valueChanged(cameraType, 2);
                break;
        case Camera::cubemap_bottom:
                cameraTypeEnumManager->setValue(cameraType, 3);
                cameraTypeEnumManager_valueChanged(cameraType, 3);
                break;
        case Camera::cubemap_top:
                cameraTypeEnumManager->setValue(cameraType, 4);
                cameraTypeEnumManager_valueChanged(cameraType, 4);
                break;
        case Camera::cubemap_front:
                cameraTypeEnumManager->setValue(cameraType, 5);
                cameraTypeEnumManager_valueChanged(cameraType, 5);
                break;
        case Camera::cubemap_back:
                cameraTypeEnumManager->setValue(cameraType, 6);
                cameraTypeEnumManager_valueChanged(cameraType, 6);
                break;
        default:
                QMessageBox::warning(ownerWidget,
                  "Unsupported Camera Type",
                  "It has been tried to set a camera type "
                  "\""
                  + QString::fromStdString(redshift::scenefile::Camera::Typenames[c.type])
                  + "\", but this type is currently not supported here."
                );
                cameraTypeEnumManager->setValue(cameraType, 0);
                cameraTypeEnumManager_valueChanged(cameraType, 0);
                break;
        };

        QtProperty *transformRoot = groupManager->addProperty("transform");
        camera->addSubProperty(transformRoot);

        collapse (root, camera);

        for (int i=0; i<c.transforms.size(); ++i) {
                addTransform (transformRoot, c.transforms[i]);
        }

        collapse (root, transformRoot);

        emit updateUi();
}




void CamerasPropertyBrowser::addTransform (QtProperty *transformRoot,
                               redshift::scenefile::Transform const & t) {
        typedef redshift::scenefile::Transform Xf;

        QtProperty *transform = groupManager->addProperty("---");
        transformRoot->addSubProperty(transform);


        QtProperty *transformType = transformEnumManager->addProperty("type");
        transform->addSubProperty(transformType);

        QStringList enumNames;
        int i = 0, def = 0;
        enumNames << "move";
        if (t.type == Xf::move) def = i; i++;
        enumNames << "move-left";
        if (t.type == Xf::move_left) def = i; i++;
        enumNames << "move-right";
        if (t.type == Xf::move_right) def = i; i++;
        enumNames << "move-up";
        if (t.type == Xf::move_up) def = i; i++;
        enumNames << "move-down";
        if (t.type == Xf::move_down) def = i; i++;
        enumNames << "move-backward";
        if (t.type == Xf::move_backward) def = i; i++;
        enumNames << "move-forward";
        if (t.type == Xf::move_forward) def = i; i++;
        enumNames << "yaw";
        if (t.type == Xf::yaw) def = i; i++;
        enumNames << "pitch";
        if (t.type == Xf::pitch) def = i; i++;
        enumNames << "roll";
        if (t.type == Xf::roll) def = i; i++;
        transformEnumManager->setEnumNames(transformType, enumNames);
        transformEnumManager->setValue(transformType, def);

        switch (t.type) {
        case Xf::move:
                writeValue("right", transform, t.x);
                writeValue("up", transform, t.y);
                writeValue("forward", transform, t.z);
                break;
        case Xf::move_left: case Xf::move_right:
                writeValue("distance", transform, t.x);
                break;
        case Xf::move_down: case Xf::move_up:
                writeValue("distance", transform, t.y);
                break;
        case Xf::move_backward: case Xf::move_forward:
                writeValue("distance", transform, t.z);
                break;
        case Xf::yaw: case Xf::pitch: case Xf::roll:
                writeValue("angle", transform, t.angle);
                break;
        }

        emit updateUi();
}




void CamerasPropertyBrowser::cameraTypeEnumManager_valueChanged(
        QtProperty* prop, int index
) {
        emit sceneChanged();
        QtProperty *t = findParent(root->properties(), prop);
        if (!t)
                return;

        const QStringList enumNames = cameraTypeEnumManager->enumNames(prop);
        const QString type = enumNames[index];

        // Remove all properties not titled "type"/"transform" to make place for
        // the right properties.
        // TODO: maybe just make them invisible if that is possible.
        foreach (QtProperty *nt, t->subProperties()){
                if (nt->propertyName() != "type"
                    && nt->propertyName() != "transform"
                ) {
                        t->removeSubProperty(nt);
                }
        }

        if (type == "pinhole") {
                QtVariantProperty* front = variantManager->addProperty(QVariant::Double,"front");
                front->setAttribute(QLatin1String("minimum"), 0.0001);
                front->setAttribute(QLatin1String("singlestep"), 0.1);
                front->setValue(1.);
                t->addSubProperty(front);
        } else if (type == "cubemap_left"
                   || type == "cubemap_right"
                   || type == "cubemap_bottom"
                   || type == "cubemap_top"
                   || type == "cubemap_front"
                   || type == "cubemap_back"
        ) {
                // do nothing
        } else {
                QMessageBox::critical(ownerWidget, "Unsupported camera type",
                                      "The camera-type '" + type + "' "
                                      "is not supported. This is probably "
                                      "an oversight by the incapable "
                                      "programmers, please report this issue.");
        }
}



void CamerasPropertyBrowser::transformEnumManager_valueChanged(
        QtProperty* prop,
        int index
) {
        emit sceneChanged();
        if (QtProperty *t = findParent(root->properties(), prop)) {
                const QStringList enumNames =
                                transformEnumManager->enumNames(prop);
                const QString type = enumNames[index];

                t->setPropertyName(type);

                // Remove all properties not titled "type".
                foreach (QtProperty *nt, t->subProperties()){
                        if (nt->propertyName() != "type") {
                                t->removeSubProperty(nt);
                        }
                }

                // When tweaking here, also look at XCVBN
                if (type == "move") {
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"right"));
                        QtVariantProperty *tmp = variantManager->addProperty(QVariant::Double,"up");
                        tmp->setValue(1.);
                        t->addSubProperty(tmp);
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"forward"));
                } else if (type == "move-left") {
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"distance"));
                } else if (type == "move-right") {
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"distance"));
                } else if (type == "move-up") {
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"distance"));
                } else if (type == "move-down") {
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"distance"));
                } else if (type == "move-forward") {
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"distance"));
                } else if (type == "move-backward") {
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"distance"));
                } else if (type == "yaw") {
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"angle"));
                } else if (type == "pitch") {
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"angle"));
                } else if (type == "roll") {
                        t->addSubProperty(variantManager->addProperty(QVariant::Double,"angle"));
                } else QMessageBox::critical(ownerWidget, "Unsupported object",
                                                  "The transform '" + type + "' "
                                                  "is not supported. This is probably "
                                                  "an oversight by the incapable "
                                                  "programmers, please report this issue.");

        }

}

