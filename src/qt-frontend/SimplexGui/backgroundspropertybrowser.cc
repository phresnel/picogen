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
#include <QtTreePropertyBrowser>
#include <QtProperty>
#include <QtVariantPropertyManager>
#include <QtVariantEditorFactory>
#include <QtGroupPropertyManager>
#include "propertybrowser-helpers.hh"

#include "redshift/include/jobfile.hh"

#include "backgroundspropertybrowser.hh"




BackgroundsPropertyBrowser::BackgroundsPropertyBrowser(
        QWidget * ownerWidget,
        QtTreePropertyBrowser * root
)
: QObject(ownerWidget)
, ownerWidget(ownerWidget)
, root(root)
, groupManager(new QtGroupPropertyManager(this))
{
        initialize();
}



void BackgroundsPropertyBrowser::initialize() {
        variantManager = new QtVariantPropertyManager(this);
        variantFactory = new QtVariantEditorFactory(this);

        connect (variantManager, SIGNAL(valueChanged(QtProperty*,QVariant)),
                 this, SLOT(variantManager_valueChanged(QtProperty*,QVariant)));
        root->setFactoryForManager(variantManager, variantFactory);


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
        vp = variantManager->addProperty(QVariant::Double, "atmospheric-effects-distance-factor");
        vp->setValue(1);
        vp->setAttribute(QLatin1String("singleStep"), 0.1);
        vp->setAttribute(QLatin1String("decimals"), 5);
        vp->setAttribute(QLatin1String("minimum"), 0);
        vp->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
        vp->setValue(b.atmosphericFxDistanceFactor);
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



QList<QtProperty*> BackgroundsPropertyBrowser::subProperties() {
        return backgroundsProperty->subProperties();
}
