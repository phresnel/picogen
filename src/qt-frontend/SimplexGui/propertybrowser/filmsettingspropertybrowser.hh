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

#ifndef FILMSETTINGSPROPERTYBROWSER_HH
#define FILMSETTINGSPROPERTYBROWSER_HH

namespace redshift {
        namespace scenefile {
                class FilmSettings;
                class Scene;
        }
}

class QWidget;
class QtProperty;
class QtGroupPropertyManager;
class QtTreePropertyBrowser;
class QtVariantPropertyManager;
class QtVariantEditorFactory;


#include <QObject>

class FilmSettingsPropertyBrowser : public QObject
{
        Q_OBJECT
public:
        FilmSettingsPropertyBrowser(
                QWidget *,
                QtTreePropertyBrowser *
                );

        void setFilmSettings(redshift::scenefile::FilmSettings const &);
        void addFilmSettingsToScene(redshift::scenefile::Scene &) const;

signals:
        void sceneChanged();
        void updateUi();

private:
        void initialize();

        QWidget *ownerWidget;
        QtTreePropertyBrowser *root;
        QtProperty *filmSettingsProperty;
        QtGroupPropertyManager *groupManager;
        QtVariantPropertyManager *variantManager;
        QtVariantEditorFactory *variantFactory;
private slots:
        void variantManager_valueChanged(QtProperty*,QVariant const &);
};

#endif // FILMSETTINGSPROPERTYBROWSER_HH
