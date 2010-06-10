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
#include <QMdiArea>

#include <QtTreePropertyBrowser>
#include <QtProperty>
#include <QtGroupPropertyManager>
#include <QtVariantPropertyManager>
#include <QtEnumPropertyManager>

#include "redshift/include/jobfile.hh"

#include "propertybrowser-helpers.hh"

#include "coloreditmanager.h"
#include "coloreditfactory.h"
#include "colorpicker.hh"

#include "volumepropertybrowser.hh"

ColorPickerColor toColorPickerColor (redshift::scenefile::Color const &c);



VolumePropertyBrowser::VolumePropertyBrowser(
                QWidget *ownerWidget_,
                QMdiArea *displayArea,
                QtTreePropertyBrowser *root,
                QtVariantPropertyManager *codeEditManager
)
: QObject(ownerWidget_)
, ownerWidget(ownerWidget_)
, displayArea(displayArea)
, root(root)
, codeEditManager(codeEditManager)
{
        initializeScene();
}



VolumePropertyBrowser::~VolumePropertyBrowser() {
}



void VolumePropertyBrowser::initializeScene() {
        groupManager = new QtGroupPropertyManager(this);

        variantManager = new QtVariantPropertyManager(this);
        variantFactory = new QtVariantEditorFactory(this);
        connect (variantManager, SIGNAL(valueChanged(QtProperty*,QVariant)),
                 this, SLOT(variantManager_valueChanged(QtProperty*,QVariant)));
        root->setFactoryForManager(variantManager, variantFactory);



        colorEditManager = new ColorEditManager (this);
        connect (colorEditManager, SIGNAL(valueChanged(QtProperty*,ColorPickerColor)),
                 this, SLOT(colorEditManager_valueChanged(QtProperty*,ColorPickerColor const&)));
        colorEditFactory = new ColorEditFactory (this, displayArea);
        root->setFactoryForManager(colorEditManager, colorEditFactory);



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
        emit sceneChanged();
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



void VolumePropertyBrowser::colorEditManager_valueChanged(
        QtProperty*,
        ColorPickerColor const&
) {
        emit sceneChanged();
}



void VolumePropertyBrowser::addVolumesToScene (
        redshift::scenefile::Scene &scene
) const {
        typedef QList<QtProperty*> Props;
        typedef QtProperty* Prop;
        using namespace redshift;


        const Props volumes = readSubProperties("volumes", root->properties());
        foreach (Prop volume, volumes) {
                using scenefile::Volume;

                const Props subs = volume->subProperties();
                const QString type = readValueText("type", subs);

                scenefile::Volume volume;

                volume.sigma_a = readColor(subs, "absorption");
                volume.sigma_s = readColor(subs, "out-scatter");
                volume.Lve     = readColor(subs, "emission");
                volume.hg      = readValue<double> ("phase-function", subs);

                // Common Properties.


                if (type == "homogeneous") {
                        volume.type = Volume::homogeneous;
                } else if (type == "exponential") {
                        volume.type = Volume::exponential;

                        Props v = readSubProperties("up", subs);
                        volume.up.x = readValue<double>("right", v);
                        volume.up.y = readValue<double>("up", v);
                        volume.up.z = readValue<double>("forward", v);

                        v = readSubProperties("min", subs);
                        volume.min.x = readValue<double>("right", v);
                        volume.min.y = readValue<double>("up", v);
                        volume.min.z = readValue<double>("forward", v);

                        volume.baseFactor = readValue<double>("base-factor", subs);
                        volume.exponentFactor = readValue<double>("exponent-factor", subs);
                        volume.epsilon = readValue<double>("epsilon", subs);
                } else {
                        throw std::runtime_error((QString() + "The volume-type '" + type + "' "
                                              "is not supported. This is probably "
                                              "an oversight by the incapable "
                                              "programmers, please report this issue.").toStdString().c_str());
                }

                scene.addVolume(volume);
        }
}



redshift::scenefile::Color
VolumePropertyBrowser::readColor (
        QList<QtProperty*> subs, QString name
) const {
        QtProperty *color = readSubProperty (name, subs);
        if (!color) return redshift::scenefile::Color();
        const ColorPickerColor c = colorEditManager->value(color);

        redshift::scenefile::Color ret;

        switch (c.mode) {
        case ColorPickerColor::Spectral:
                ret.type = redshift::scenefile::Color::Spectrum;
                foreach (SpectralSample ss, c.spectral) {
                        redshift::scenefile::WavelengthAmplitudePair wap;
                        wap.wavelength = ss.wavelength;
                        wap.amplitude = ss.amplitude;
                        ret.spectrum.samples.push_back(wap);
                }
                break;
        case ColorPickerColor::Tristimulus:
                ret.type = redshift::scenefile::Color::RGB;
                redshift::color::RGB rgb = c.rgb();
                ret.rgb.r = rgb.R;
                ret.rgb.g = rgb.G;
                ret.rgb.b = rgb.B;
                break;
        }

        return ret;
}



void VolumePropertyBrowser::variantManager_valueChanged(
        QtProperty*, QVariant const &
) {
        emit sceneChanged();
}
