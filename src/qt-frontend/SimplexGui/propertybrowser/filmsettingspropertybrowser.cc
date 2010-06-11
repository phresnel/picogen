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

#include <QtVariantProperty>
#include <QtGroupPropertyManager>
#include <QtVariantPropertyManager>
#include <QtVariantEditorFactory>
#include <QtTreePropertyBrowser>

#include "redshift/include/jobfile.hh"
#include "propertybrowser-helpers.hh"

#include "filmsettingspropertybrowser.hh"



FilmSettingsPropertyBrowser::FilmSettingsPropertyBrowser(
        QWidget *ownerWidget,
        QtTreePropertyBrowser *root
)
: QObject(ownerWidget)
, ownerWidget(ownerWidget)
, root(root)
, groupManager(new QtGroupPropertyManager(this))
{
        initialize();
}



FilmSettingsPropertyBrowser::~FilmSettingsPropertyBrowser() {
        root->unsetFactoryForManager(variantManager);
        delete groupManager;
        delete variantManager;
        delete variantFactory;
}



void FilmSettingsPropertyBrowser::initialize() {

        variantManager = new QtVariantPropertyManager(this);
        variantFactory = new QtVariantEditorFactory(this);

        connect (variantManager, SIGNAL(valueChanged(QtProperty*,QVariant)),
                 this, SLOT(variantManager_valueChanged(QtProperty*,QVariant)));
        root->setFactoryForManager(variantManager, variantFactory);


        QtVariantProperty *item = 0;

        filmSettingsProperty = groupManager->addProperty(QLatin1String("film-settings"));

        // convert-to-srgb
        item = variantManager->addProperty(
                        QVariant::Bool,
                        QLatin1String("convert-to-srgb"));
        item->setValue(true);
        filmSettingsProperty->addSubProperty(item);

        // color-scale
        item = variantManager->addProperty(
                        QVariant::Double,
                        QLatin1String("color-scale"));
        item->setValue(0.013);
        item->setAttribute(QLatin1String("singleStep"), 0.01);
        item->setAttribute(QLatin1String("decimals"), 6);
        item->setAttribute(QLatin1String("minimum"), 0);
        item->setAttribute(QLatin1String("maximum"), redshift::constants::infinity);
        filmSettingsProperty->addSubProperty(item);


        root->addProperty(filmSettingsProperty);

        root->setBackgroundColor(
                        root->topLevelItem(filmSettingsProperty),
                        QColor(130,110,90));
}



void FilmSettingsPropertyBrowser::variantManager_valueChanged(QtProperty*,
                                                              QVariant const &){
        emit sceneChanged();
}



void FilmSettingsPropertyBrowser::setFilmSettings(redshift::scenefile::FilmSettings const &fs) {
        writeValue<bool>("convert-to-srgb", filmSettingsProperty, fs.convertToSrgb);
        writeValue<double>("color-scale", filmSettingsProperty, fs.colorscale);
        emit sceneChanged();
}



void FilmSettingsPropertyBrowser::addFilmSettingsToScene(
        redshift::scenefile::Scene &scene
) const {
        using namespace redshift;
        const QList<QtProperty*> filmSettings =
                        readSubProperties("film-settings", root->properties());
        scenefile::FilmSettings fs;
        fs.colorscale = readValue<double>("color-scale", filmSettings);
        fs.convertToSrgb = readValue<bool>("convert-to-srgb", filmSettings);
        scene.setFilmSettings(fs);
}
