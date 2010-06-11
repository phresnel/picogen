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

#ifndef RENDERSETTINGSPROPERTYBROWSER_HH
#define RENDERSETTINGSPROPERTYBROWSER_HH

namespace redshift {
        namespace scenefile {
                class RenderSettings;
                class Scene;
        }
}
class QtTreePropertyBrowser;
class QtGroupPropertyManager;
class QtProperty;
class QtEnumPropertyManager;
class QtVariantPropertyManager;
class QtVariantEditorFactory;
class QtLineEditFactory;
class QtStringPropertyManager;
class QtEnumEditorFactory;


#include <QObject>
#include <QStringList>

class RenderSettingsPropertyBrowser : public QObject
{
        Q_OBJECT
public:
        RenderSettingsPropertyBrowser(QWidget *,
                                      QtTreePropertyBrowser *);
        ~RenderSettingsPropertyBrowser();

        void addRenderSettings (redshift::scenefile::RenderSettings const &rs);
        void remove (QtProperty *);

        void addRenderSettingsToScene (redshift::scenefile::Scene &Scene) const;
        QStringList names () const ;

signals:
        void updateUi();
        void sceneChanged();

private:
        void initializeScene();

        QWidget *ownerWidget;
        QtTreePropertyBrowser *root;
        QtEnumPropertyManager
                *volumeTypeEnumManager,
                *surfaceIntegratorTypeEnumManager,
                *enumManager;
        QtGroupPropertyManager *groupManager;
        QtVariantPropertyManager *variantManager;
        QtVariantEditorFactory *variantFactory;
        QtProperty *renderSettingsProperty;

        QtLineEditFactory *lineEditFactory;
        QtEnumEditorFactory *comboBoxFactory;

        QtStringPropertyManager *rsTitleManager;

private slots:
        void enumManager_valueChanged (QtProperty*, int);
        void rsTitleManager_valueChanged (QtProperty *, const QString &);
        void surfaceIntegratorTypeEnumManager_valueChanged(QtProperty*, int);
        void variantManager_valueChanged(QtProperty*,QVariant const &);
};

#endif // RENDERSETTINGSPROPERTYBROWSER_HH
