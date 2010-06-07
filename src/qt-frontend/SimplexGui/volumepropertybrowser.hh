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

#ifndef VOLUMEPROPERTYBROWSER_HH
#define VOLUMEPROPERTYBROWSER_HH


namespace redshift {
        namespace scenefile {
                class Volume;
        }
}
class QtTreePropertyBrowser;
class QtGroupPropertyManager;
class QtProperty;
class QtEnumPropertyManager;
class QtVariantPropertyManager;
class ColorEditManager;

#include <QObject>

class VolumePropertyBrowser : public QObject
{
        Q_OBJECT
public:
        VolumePropertyBrowser(QWidget *,
                              QtTreePropertyBrowser *,
                              QtGroupPropertyManager *,
                              QtVariantPropertyManager *,
                              QtVariantPropertyManager *codeEd,
                              ColorEditManager *
                             );
        ~VolumePropertyBrowser();

        void addVolume (redshift::scenefile::Volume const &);
        void remove (QtProperty *);

signals:
        void updateUi();
        void sceneChanged();

private:
        void initializeScene ();

        QWidget *ownerWidget;
        QtTreePropertyBrowser *root;
        QtEnumPropertyManager *volumeTypeEnumManager;
        QtGroupPropertyManager *groupManager;
        QtVariantPropertyManager *variantManager, *codeEditManager;
        QtProperty *volumesProperty;
        ColorEditManager *colorEditManager;

private slots:
        void volumeTypeEnumManager_valueChanged(QtProperty*, int);
};

#endif // VOLUMEPROPERTYBROWSER_HH
