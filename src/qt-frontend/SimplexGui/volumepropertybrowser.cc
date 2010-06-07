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

#include "redshift/include/jobfile.hh"

#include "propertybrowser-helpers.hh"

#include "coloreditmanager.h"

#include "volumepropertybrowser.hh"

ColorPickerColor toColorPickerColor (redshift::scenefile::Color const &c);

VolumePropertyBrowser::VolumePropertyBrowser(
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
, codeEditManager(codeEditManager)
, volumesProperty(0)
, colorEditManager(colorEditManager)
{
        initializeScene();
}



VolumePropertyBrowser::~VolumePropertyBrowser() {
}



void VolumePropertyBrowser::initializeScene() {
        volumesProperty = groupManager->addProperty("volumes");
        root->addProperty(volumesProperty);

        root->setBackgroundColor(
                        root->topLevelItem(volumesProperty),
                        QColor(120,120,160));

        volumeTypeEnumManager = new QtEnumPropertyManager(this);
        connect(volumeTypeEnumManager, SIGNAL(valueChanged (QtProperty *, int)),
                this, SLOT(volumeTypeEnumManager_valueChanged(QtProperty*,int)));
}



void VolumePropertyBrowser::addVolume (redshift::scenefile::Volume const &v) {
        QtProperty *volume = groupManager->addProperty("---");
        volumesProperty->addSubProperty(volume);

        QtProperty *volumeType = volumeTypeEnumManager->addProperty("type");
        volume->addSubProperty(volumeType);


        QStringList enumNames;
        enumNames << "homogeneous"
                  << "exponential"
                  ;
        volumeTypeEnumManager->setEnumNames(volumeType, enumNames);

        QtProperty *tmp;

        switch (v.type) {
        case redshift::scenefile::Volume::homogeneous:
                volumeTypeEnumManager->setValue(volumeType, 0);
                volumeTypeEnumManager_valueChanged(volumeType, 0);
                break;
        case redshift::scenefile::Volume::exponential:
                volumeTypeEnumManager->setValue(volumeType, 1);
                volumeTypeEnumManager_valueChanged(volumeType, 1);

                tmp = readSubProperty("up", volume);
                writeValue ("right",   tmp, v.up.x);
                writeValue ("up",      tmp, v.up.y);
                writeValue ("forward", tmp, v.up.z);

                tmp = readSubProperty("min", volume);
                writeValue ("right",   tmp, v.min.x);
                writeValue ("up",      tmp, v.min.y);
                writeValue ("forward", tmp, v.min.z);

                writeValue ("base-factor", volume, v.baseFactor);
                writeValue ("exponent-factor", volume, v.exponentFactor);
                writeValue ("epsilon", volume, v.epsilon);
                break;
        }

        tmp = readSubProperty("absorption", volume);
        colorEditManager->setValue(tmp, toColorPickerColor(v.sigma_a));
        tmp = readSubProperty("out-scatter", volume);
        colorEditManager->setValue(tmp, toColorPickerColor(v.sigma_s));
        tmp = readSubProperty("emission", volume);
        colorEditManager->setValue(tmp, toColorPickerColor(v.Lve));
        writeValue ("phase-function", volume, v.hg);

        collapse (root, volume);
        updateUi();
}



void VolumePropertyBrowser::remove(QtProperty *property) {
        volumesProperty->removeSubProperty(property);
}



void VolumePropertyBrowser::volumeTypeEnumManager_valueChanged (
        QtProperty* prop,
        int index
) {
        emit sceneChanged();
        QtProperty *t = findParent(root->properties(), prop);
        if (!t)
                return;

        const QStringList enumNames =
                        volumeTypeEnumManager->enumNames(prop);
        const QString type = enumNames[index];

        t->setPropertyName(type);

        // Remove all properties not titled "type" to make place for the right
        // properties.
        // TODO: maybe just make them invisible if that is possible.
        foreach (QtProperty *nt, t->subProperties()){
                if (nt->propertyName() != "type") {
                        t->removeSubProperty(nt);
                }
        }


        // Common.
        t->addSubProperty(colorEditManager->addProperty("absorption"));
        t->addSubProperty(colorEditManager->addProperty("out-scatter"));
        t->addSubProperty(colorEditManager->addProperty("emission"));

        QtVariantProperty* hg = variantManager->addProperty(QVariant::Double,"phase-function");
        hg->setAttribute(QLatin1String("minimum"), -1);
        hg->setAttribute(QLatin1String("maximum"), 1);
        hg->setAttribute(QLatin1String("singleStep"), 0.1);
        hg->setAttribute(QLatin1String("decimals"), 5);
        hg->setValue(0);
        t->addSubProperty(hg);


        if (type == "homogeneous") {

        } else if (type == "exponential") {
                {
                        QtProperty *upVector = groupManager->addProperty("up");
                        QtVariantProperty *vp = variantManager->addProperty(QVariant::Double, "right");
                        vp->setValue(1);
                        vp->setAttribute(QLatin1String("singleStep"), 0.05);
                        vp->setAttribute(QLatin1String("decimals"), 5);
                        vp->setAttribute(QLatin1String("minimum"), -redshift::constants::infinity);
                        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
                        vp->setValue(0);
                        upVector->addSubProperty(vp);

                        vp= variantManager->addProperty(QVariant::Double, "up");
                        vp->setValue(1);
                        vp->setAttribute(QLatin1String("singleStep"), 0.05);
                        vp->setAttribute(QLatin1String("decimals"), 5);
                        vp->setAttribute(QLatin1String("minimum"), -redshift::constants::infinity);
                        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
                        vp->setValue(1);
                        upVector->addSubProperty(vp);

                        vp = variantManager->addProperty(QVariant::Double, "forward");
                        vp->setValue(1);
                        vp->setAttribute(QLatin1String("singleStep"), 0.05);
                        vp->setAttribute(QLatin1String("decimals"), 5);
                        vp->setAttribute(QLatin1String("minimum"), -redshift::constants::infinity);
                        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
                        vp->setValue(0);
                        upVector->addSubProperty(vp);

                        t->addSubProperty(upVector);
                }
                {
                        QtProperty *minVector = groupManager->addProperty("min");
                        QtVariantProperty *vp = variantManager->addProperty(QVariant::Double, "right");
                        vp->setValue(1);
                        vp->setAttribute(QLatin1String("singleStep"), 0.05);
                        vp->setAttribute(QLatin1String("decimals"), 5);
                        vp->setAttribute(QLatin1String("minimum"), -redshift::constants::infinity);
                        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
                        vp->setValue(0);
                        minVector->addSubProperty(vp);

                        vp= variantManager->addProperty(QVariant::Double, "up");
                        vp->setValue(1);
                        vp->setAttribute(QLatin1String("singleStep"), 0.05);
                        vp->setAttribute(QLatin1String("decimals"), 5);
                        vp->setAttribute(QLatin1String("minimum"), -redshift::constants::infinity);
                        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
                        vp->setValue(1);
                        minVector->addSubProperty(vp);

                        vp = variantManager->addProperty(QVariant::Double, "forward");
                        vp->setValue(1);
                        vp->setAttribute(QLatin1String("singleStep"), 0.05);
                        vp->setAttribute(QLatin1String("decimals"), 5);
                        vp->setAttribute(QLatin1String("minimum"), -redshift::constants::infinity);
                        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
                        vp->setValue(0);
                        minVector->addSubProperty(vp);

                        t->addSubProperty(minVector);
                }

                QtVariantProperty *vp = variantManager->addProperty(QVariant::Double, "base-factor");
                vp->setAttribute(QLatin1String("singleStep"), 0.05);
                vp->setAttribute(QLatin1String("decimals"), 5);
                vp->setAttribute(QLatin1String("minimum"), -redshift::constants::infinity);
                vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
                t->addSubProperty(vp);

                vp = variantManager->addProperty(QVariant::Double, "exponent-factor");
                vp->setAttribute(QLatin1String("singleStep"), 0.05);
                vp->setAttribute(QLatin1String("decimals"), 5);
                vp->setAttribute(QLatin1String("minimum"), -redshift::constants::infinity);
                vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
                t->addSubProperty(vp);

                vp = variantManager->addProperty(QVariant::Double, "epsilon");
                vp->setToolTip("The minimum desired density. This can then be used by picogen "
                               "for calculating the boundary and thus enable optimiziations.");
                vp->setAttribute(QLatin1String("singleStep"), 0.05);
                vp->setAttribute(QLatin1String("decimals"), 5);
                vp->setAttribute(QLatin1String("minimum"), -redshift::constants::infinity);
                vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
                t->addSubProperty(vp);
        } else {
                QMessageBox::critical(ownerWidget, "Unsupported volume",
                                      "The volume-type '" + type + "' "
                                      "is not supported. This is probably "
                                      "an oversight by the incapable "
                                      "programmers, please report this issue.");
        }
}
