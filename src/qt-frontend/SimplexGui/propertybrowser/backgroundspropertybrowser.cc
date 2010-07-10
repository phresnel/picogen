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

#include <QWidget>
#include <QMdiArea>
#include <QtTreePropertyBrowser>
#include <QtProperty>
#include <QtVariantPropertyManager>
#include <QtVariantEditorFactory>
#include <QtGroupPropertyManager>
#include "propertybrowser-helpers.hh"

#include "redshift/include/jobfile.hh"

#include "coloreditmanager.h"
#include "coloreditfactory.h"
#include "colorpicker.hh"

#include "backgroundspropertybrowser.hh"




BackgroundsPropertyBrowser::BackgroundsPropertyBrowser(
        QWidget * ownerWidget,
        QMdiArea * displayArea,
        QtTreePropertyBrowser * root
)
: QObject(ownerWidget)
, ownerWidget(ownerWidget)
, displayArea(displayArea)
, root(root)
, groupManager(new QtGroupPropertyManager(this))
{
        initialize();
}



BackgroundsPropertyBrowser::~BackgroundsPropertyBrowser() {
        root->unsetFactoryForManager(variantManager);
        root->unsetFactoryForManager(colorEditManager);

        delete groupManager;
        delete variantManager;
        delete variantFactory;

        delete colorEditManager;
        delete colorEditFactory;
}



void BackgroundsPropertyBrowser::initialize() {
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


        backgroundsProperty = groupManager->addProperty("backgrounds");
        root->addProperty(backgroundsProperty);

        root->setBackgroundColor(
                root->topLevelItem(backgroundsProperty),
                QColor(90,90,130));
}



void BackgroundsPropertyBrowser::setBackground (
        redshift::scenefile::Background const &b
) {
        pssSunSkyProperty = groupManager->addProperty("pss-sunsky");
        backgroundsProperty->addSubProperty(pssSunSkyProperty);

        // Sun Direction.
        QtProperty *sunDir = groupManager->addProperty("sun-direction");
        pssSunSkyProperty->addSubProperty(sunDir);

        // TODO: write custom property for vectors
        QtVariantProperty *vp = variantManager->addProperty(QVariant::Double, "right");
        vp->setValue(1);
        vp->setAttribute(QLatin1String("singleStep"), 0.05);
        vp->setAttribute(QLatin1String("decimals"), 5);
        vp->setAttribute(QLatin1String("minimum"), -redshift::constants::infinity);
        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
        vp->setValue(b.sunDirection.x);
        sunDir->addSubProperty(vp);

        vp= variantManager->addProperty(QVariant::Double, "up");
        vp->setValue(1);
        vp->setAttribute(QLatin1String("singleStep"), 0.05);
        vp->setAttribute(QLatin1String("decimals"), 5);
        vp->setAttribute(QLatin1String("minimum"), -redshift::constants::infinity);
        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
        vp->setValue(b.sunDirection.y);
        sunDir->addSubProperty(vp);

        vp = variantManager->addProperty(QVariant::Double, "forward");
        vp->setValue(1);
        vp->setAttribute(QLatin1String("singleStep"), 0.05);
        vp->setAttribute(QLatin1String("decimals"), 5);
        vp->setAttribute(QLatin1String("minimum"), -redshift::constants::infinity);
        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
        vp->setValue(b.sunDirection.z);
        sunDir->addSubProperty(vp);


        // Turbidity.
        vp = variantManager->addProperty(QVariant::Double, "turbidity");
        vp->setValue(1);
        vp->setAttribute(QLatin1String("singleStep"), 0.1);
        vp->setAttribute(QLatin1String("decimals"), 5);
        vp->setAttribute(QLatin1String("minimum"), 0);
        vp->setAttribute(QLatin1String("maximum"), 30);
        vp->setToolTip("For realism, use values between 2 and 10. But for artistic reasons, use any.");
        vp->setValue(b.turbidity);
        pssSunSkyProperty->addSubProperty(vp);

        // Sun size.
        vp = variantManager->addProperty(QVariant::Double, "sun-size-factor");
        vp->setValue(1);
        vp->setAttribute(QLatin1String("singleStep"), 1.0);
        vp->setAttribute(QLatin1String("decimals"), 5);
        vp->setAttribute(QLatin1String("minimum"), 0);
        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
        vp->setValue(b.sunSizeFactor);
        pssSunSkyProperty->addSubProperty(vp);

        // Sun brightness factor.
        vp = variantManager->addProperty(QVariant::Double, "sun-brightness-factor");
        vp->setValue(1);
        vp->setAttribute(QLatin1String("singleStep"), 0.1);
        vp->setAttribute(QLatin1String("decimals"), 5);
        vp->setAttribute(QLatin1String("minimum"), 0);
        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
        vp->setValue(b.sunBrightnessFactor);
        pssSunSkyProperty->addSubProperty(vp);


        // Atmosphere brightness factor.
        vp = variantManager->addProperty(QVariant::Double, "atmosphere-brightness-factor");
        vp->setValue(1);
        vp->setAttribute(QLatin1String("singleStep"), 0.1);
        vp->setAttribute(QLatin1String("decimals"), 5);
        vp->setAttribute(QLatin1String("minimum"), 0);
        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
        vp->setValue(b.atmosphereBrightnessFactor);
        pssSunSkyProperty->addSubProperty(vp);


        // Atmosphere effects distance factor.
        vp = variantManager->addProperty(QVariant::Double, "atmospheric-effects-factor");
        vp->setValue(1);
        vp->setAttribute(QLatin1String("singleStep"), 0.05);
        vp->setAttribute(QLatin1String("decimals"), 1);
        vp->setAttribute(QLatin1String("minimum"), 0);
        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
        vp->setValue(b.atmosphericFxFactor);
        pssSunSkyProperty->addSubProperty(vp);


        // Overcast.
        vp = variantManager->addProperty(QVariant::Double, "overcast");
        vp->setValue(0);
        vp->setAttribute(QLatin1String("singleStep"), 0.05);
        vp->setAttribute(QLatin1String("decimals"), 5);
        vp->setAttribute(QLatin1String("minimum"), 0);
        vp->setAttribute(QLatin1String("maximum"), 1);
        vp->setValue(b.overcast);
        pssSunSkyProperty->addSubProperty(vp);

        // Atmospheric Effects.
        vp = variantManager->addProperty(QVariant::Bool, "atmospheric-effects");
        vp->setValue(b.atmosphericEffects ? 1 : 0);
        pssSunSkyProperty->addSubProperty(vp);

        collapse (root, pssSunSkyProperty);
        emit sceneChanged();
}



void BackgroundsPropertyBrowser::variantManager_valueChanged(
        QtProperty*, QVariant const &
) {
        emit sceneChanged();
}



redshift::scenefile::Color
BackgroundsPropertyBrowser::readColor (
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



void BackgroundsPropertyBrowser::addBackgroundsToScene (
        redshift::scenefile::Scene &scene
) const {
        typedef QList<QtProperty*> Props;
        typedef QtProperty* Prop;
        using namespace redshift;

        const Props backgrounds = readSubProperties("backgrounds", root->properties());
        foreach (Prop bg, backgrounds) {
                if (bg->propertyName() == "pss-sunsky") {
                        scenefile::Background sunsky;
                        const Props sunDir = readSubProperties("sun-direction", bg);

                        sunsky.sunDirection.x = readValue<double>("right", sunDir);
                        sunsky.sunDirection.y = readValue<double>("up", sunDir);
                        sunsky.sunDirection.z = readValue<double>("forward", sunDir);
                        sunsky.sunSizeFactor = readValue<double>("sun-size-factor", bg);
                        sunsky.sunBrightnessFactor = readValue<double>("sun-brightness-factor", bg);

                        sunsky.atmosphericEffects = readValue<bool>("atmospheric-effects", bg);
                        sunsky.atmosphereBrightnessFactor = readValue<double>("atmosphere-brightness-factor", bg);
                        sunsky.atmosphericFxFactor = readValue<double>("atmospheric-effects-factor", bg);

                        sunsky.turbidity = readValue<double>("turbidity", bg);
                        sunsky.overcast = readValue<double>("overcast", bg);
                        scene.addBackground(sunsky);
                }
        }
}



void BackgroundsPropertyBrowser::colorEditManager_valueChanged(
        QtProperty*,
        ColorPickerColor const&
) {
        emit sceneChanged();
}
