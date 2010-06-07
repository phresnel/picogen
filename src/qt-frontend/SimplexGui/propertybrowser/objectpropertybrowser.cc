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

#include "objectpropertybrowser.hh"

ColorPickerColor toColorPickerColor (redshift::scenefile::Color const &c);



ObjectPropertyBrowser::ObjectPropertyBrowser(
                QWidget *ownerWidget_,
                QMdiArea *displayArea,
                QtTreePropertyBrowser *root,
                QtVariantPropertyManager *codeEditManager
                )
: QObject(ownerWidget_)
, ownerWidget(ownerWidget_)
, displayArea(displayArea)
, root(root)
, groupManager(new QtGroupPropertyManager(this))
, codeEditManager(codeEditManager)
, objectsProperty(0)
{
        initializeScene();
}



ObjectPropertyBrowser::~ObjectPropertyBrowser() {
}



void ObjectPropertyBrowser::initializeScene() {

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



        objectsProperty = groupManager->addProperty("objects");
        root->addProperty(objectsProperty);

        root->setBackgroundColor(
                root->topLevelItem(objectsProperty),
                QColor(90,130,90));

        objectTypeEnumManager = new QtEnumPropertyManager(this);
        connect(objectTypeEnumManager, SIGNAL(valueChanged (QtProperty*, int)),
                this, SLOT(objectTypeEnumManager_valueChanged(QtProperty*,int)));
}



void ObjectPropertyBrowser::remove(QtProperty *property) {
        objectsProperty->removeSubProperty(property);
        emit sceneChanged();
}



void ObjectPropertyBrowser::addObject (redshift::scenefile::Object const &o) {
        QtProperty *object = groupManager->addProperty("---");
        objectsProperty->addSubProperty(object);

        QtProperty *objectType = objectTypeEnumManager->addProperty("type");
        object->addSubProperty(objectType);


        QStringList enumNames;
        enumNames << "horizon-plane"
                  << "water-plane"
                  << "lazy-quadtree"
                  ;
        objectTypeEnumManager->setEnumNames(objectType, enumNames);

        QtProperty *tmp;

        switch (o.type) {
        case redshift::scenefile::Object::horizon_plane:
                objectTypeEnumManager->setValue(objectType, 0);
                objectTypeEnumManager_valueChanged(objectType, 0);

                writeValue ("height", object, o.horizonPlaneParams.height);

                tmp = readSubProperty("material", object);
                tmp = readSubProperty("color", tmp);
                colorEditManager->setValue(tmp, toColorPickerColor(o.horizonPlaneParams.material.color));
                break;
        case redshift::scenefile::Object::water_plane:
                objectTypeEnumManager->setValue(objectType, 1);
                objectTypeEnumManager_valueChanged(objectType, 1);

                writeValue ("height", object, o.waterPlaneParams.height);
                writeValue ("code", object, QString::fromStdString(o.waterPlaneParams.code));

                tmp = readSubProperty("material", object);
                tmp = readSubProperty("color", tmp);
                colorEditManager->setValue(tmp, toColorPickerColor(o.waterPlaneParams.material.color));
                break;
        case redshift::scenefile::Object::lazy_quadtree:
                objectTypeEnumManager->setValue(objectType, 2);
                objectTypeEnumManager_valueChanged(objectType, 2);

                writeValue ("code", object, QString::fromStdString(o.lazyQuadtreeParams.code));
                writeValue ("max-recursion", object, o.lazyQuadtreeParams.maxRecursion);
                writeValue ("lod-factor", object, o.lazyQuadtreeParams.lodFactor);
                writeValue ("size", object, o.lazyQuadtreeParams.size);

                tmp = readSubProperty("material", object);
                tmp = readSubProperty("color", tmp);
                colorEditManager->setValue(tmp, toColorPickerColor(o.lazyQuadtreeParams.material.color));
                break;
        }

        collapse (root, object);
        emit updateUi();
}



void ObjectPropertyBrowser::objectTypeEnumManager_valueChanged (
        QtProperty* prop,
        int index
) {
        emit sceneChanged();
        // parent
        //   |
        //   +-----type
        //   |
        //   +-----param*

        // We have 'type', now find 'parent'
        QtProperty *t = findParent(root->properties(), prop);
        if (!t)
                return;

        const QStringList enumNames =
                        objectTypeEnumManager->enumNames(prop);
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

        if (type == "horizon-plane") {
                QtVariantProperty* it = variantManager->addProperty(QVariant::Double,"height");
                t->addSubProperty(it);
                it->setAttribute(QLatin1String("decimals"), 6);

                QtProperty *colorTerms = groupManager->addProperty("material");
                t->addSubProperty(colorTerms);
                QtProperty *cit = colorEditManager->addProperty("color");
                colorTerms->addSubProperty(cit);

                /*QtProperty *tit = variantManager->addProperty(QVariant::Color, "color");
                colorTerms->addSubProperty(tit);*/

                /*it->setAttribute(QLatin1String("color"), 6);*/
        } else if (type == "water-plane") {
                QtVariantProperty* height = variantManager->addProperty(QVariant::Double,"height");
                t->addSubProperty(height);
                height->setAttribute(QLatin1String("decimals"), 6);
                QtProperty* code = codeEditManager->addProperty(QVariant::String, "code");
                t->addSubProperty(code);

                QtProperty *colorTerms = groupManager->addProperty("material");
                t->addSubProperty(colorTerms);
                QtProperty *cit = colorEditManager->addProperty("color");
                colorTerms->addSubProperty(cit);
        } else if (type == "lazy-quadtree") {
                //t->addSubProperty(variantManager->addProperty(QVariant::UserType,"color"));

                QtProperty* code = codeEditManager->addProperty(QVariant::String, "code");
                t->addSubProperty(code);

                QtVariantProperty* maxrec = variantManager->addProperty(QVariant::Int,"max-recursion");
                maxrec->setToolTip("Be careful. Slowly approach values beyond 10. For preview renderings, values less than 10 seem to work well.");
                maxrec->setAttribute(QLatin1String("minimum"), 1);
                maxrec->setAttribute(QLatin1String("maximum"), 20);
                maxrec->setValue(7);
                t->addSubProperty(maxrec);

                QtVariantProperty* lodfac = variantManager->addProperty(QVariant::Double,"lod-factor");
                lodfac->setToolTip("Very sensible factor. The smaller, the less detail-loss upon increasing distance.");
                lodfac->setAttribute(QLatin1String("minimum"), 0.000001);
                lodfac->setAttribute(QLatin1String("maximum"), 1);
                lodfac->setAttribute(QLatin1String("singleStep"), 0.00001);
                lodfac->setAttribute(QLatin1String("decimals"), 6);
                lodfac->setValue(0.000125);
                t->addSubProperty(lodfac);

                QtVariantProperty* size = variantManager->addProperty(QVariant::Double,"size");
                lodfac->setToolTip("Picogen's quadtree scales quite well. Feel free to try out very large values of 100k and bigger.");
                size->setAttribute(QLatin1String("minimum"), 1);
                size->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
                size->setAttribute(QLatin1String("singleStep"), 1000);
                size->setAttribute(QLatin1String("decimals"), 1);
                size->setValue(50000);
                t->addSubProperty(size);


                QtProperty *colorTerms = groupManager->addProperty("material");
                t->addSubProperty(colorTerms);
                QtProperty *cit = colorEditManager->addProperty("color");
                colorTerms->addSubProperty(cit);

                //t->addSubProperty(variantManager->addProperty(QVariant::V,"color"));
        } else {
                QMessageBox::critical(ownerWidget, "Unsupported object",
                                      "The object-type '" + type + "' "
                                      "is not supported. This is probably "
                                      "an oversight by the incapable "
                                      "programmers, please report this issue.");
        }
}



void ObjectPropertyBrowser::colorEditManager_valueChanged(
        QtProperty*,
        ColorPickerColor const&
) {
        emit sceneChanged();
}



void ObjectPropertyBrowser::addObjectsToScene (
        redshift::scenefile::Scene &scene
) const {
        typedef QList<QtProperty*> Props;
        typedef QtProperty* Prop;
        using namespace redshift;


        const Props objects = objectsProperty->subProperties();
        //readSubProperties("objects", topProps);
        foreach (Prop object, objects) {
                using scenefile::Object;

                const Props subs = object->subProperties();
                const QString type = readValueText("type", subs);

                scenefile::Object object;

                if (type == "water-plane") {
                        object.type = Object::water_plane;
                        const std::string tmp = readValue<QString>("code", subs).toStdString();
                        if(!tmp.empty())
                                object.waterPlaneParams.code = tmp;
                        //object.waterPlaneParams.color
                        object.waterPlaneParams.height = readValue<double>("height", subs);
                        object.waterPlaneParams.material = readMaterial (subs);
                } else if (type == "horizon-plane") {
                        object.type = Object::horizon_plane;
                        //object.horizonPlaneParams.color
                        object.horizonPlaneParams.height = readValue<double>("height", subs);
                        object.horizonPlaneParams.material = readMaterial (subs);
                } else if (type == "lazy-quadtree") {
                        object.type = Object::lazy_quadtree;
                        const std::string tmp = readValue<QString>("code", subs).toStdString();
                        if(!tmp.empty())
                                object.lazyQuadtreeParams.code = tmp;
                        //object.lazyQuadtreeParams.color;
                        object.lazyQuadtreeParams.lodFactor = readValue<double>("lod-factor", subs);
                        object.lazyQuadtreeParams.maxRecursion = readValue<unsigned int>("max-recursion", subs);
                        object.lazyQuadtreeParams.size = readValue<double>("size", subs);
                        object.lazyQuadtreeParams.material = readMaterial (subs);

                } else {
                        throw std::runtime_error((QString() + "The object-type '" + type + "' "
                                              "is not supported. This is probably "
                                              "an oversight by the incapable "
                                              "programmers, please report this issue.").toStdString().c_str());
                }

                scene.addObject(object);
        }
}



redshift::scenefile::Color
ObjectPropertyBrowser::readColor (
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
                QColor rgb = c.toQColor();
                ret.rgb = redshift::scenefile::Rgb(rgb.redF(),
                                                   rgb.greenF(),
                                                   rgb.blueF());
                break;
        }

        return ret;
}



redshift::scenefile::Material
ObjectPropertyBrowser::readMaterial (
        QList<QtProperty*> subs, QString name
) const {
        QtProperty *material = readSubProperty(name, subs);
        if (!material) return redshift::scenefile::Material();

        redshift::scenefile::Material mat;
        mat.color = readColor (material->subProperties(), "color");
        return mat;
}
