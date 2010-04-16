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

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include "qtvariantproperty.h"
#include "QtStringPropertyManager"
#include "qtpropertymanager.h"
#include "qteditorfactory.h"

#include "coloredit.h"
#include "coloreditmanager.h"
#include "coloreditfactory.h"

#include "redshift/include/smart_ptr.hh"
#include "redshift/include/jobfile.hh"

namespace Ui {
    class MainWindow;
}


namespace redshift {
        namespace scenefile {
                class Scene;
        }
}

class MainWindow : public QMainWindow {
        Q_OBJECT
public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();

protected:
        void changeEvent(QEvent *e);

private:
        Ui::MainWindow *ui;

        QtStringPropertyManager *rsTitleManager;

        QtLineEditFactory *lineEditFactory;

        QtVariantPropertyManager *variantManager;
        QtVariantEditorFactory *variantFactory;

        QtGroupPropertyManager *groupManager;

        QtEnumPropertyManager *enumManager;
        QtEnumEditorFactory *comboBoxFactory;

        ColorEditManager *colorEditManager;
        ColorEditFactory *colorEditFactory;

        QtProperty *renderSettingsProperty;
        QtProperty *camerasProperty,
                   *currentCameraProperty,
                   *currentTransformProperty,
                   *currentRenderSettingProperty;
        QtProperty *objectsProperty,
                   *pssSunSkyProperty;
        QtEnumPropertyManager
                        *transformEnumManager,
                        *objectTypeEnumManager,
                        *surfaceIntegratorTypeEnumManager;
        QtVariantPropertyManager *codeEditManager;
        redshift::shared_ptr<redshift::scenefile::Scene> createScene () const;

        void resyncRenderSettingConfig ();
        void resyncCameraConfig ();
        void addRenderSettings (std::string const &name);
        void addCamera (std::string const &name);
        void addTransform (QtProperty *transformRoot,
                           redshift::scenefile::Transform::Type type);
        void addObject ();

        redshift::scenefile::Material readMaterial (QList<QtProperty*> subs, QString name="material") const;

        QtBrowserItem * currentBrowserItem;
        QString saveFilename;

        QString askForNewSaveFilename() ;
        QString getAndUpdateSaveFilename();

        void refreshWindowTitle();
        QString sceneToCode();

        bool nonRecurseLock;
private slots:        
        void on_deleteCameraButton_clicked();
        void on_newCameraButton_clicked();
        void on_deleteRsButton_clicked();
        void on_deleteObjectButton_clicked();
        void on_deleteSubTransformButton_clicked();
        void on_newObjectButton_clicked();
        void on_newSubTransformButton_clicked();
        void on_newRsButton_clicked();

        void on_actionSave_as_triggered();
        void on_actionSave_copy_as_triggered();
        void on_action_Save_triggered();
        void on_actionRender_triggered();
        void on_actionShow_redshift_job_code_triggered();

        void on_settings_currentItemChanged(QtBrowserItem * current);
        void on_codeEditor_codeChanged();

        // non MOC
        void code_valueChanged(QtProperty*, QVariant);
        void transformEnumManager_valueChanged(QtProperty*, int);
        void objectTypeEnumManager_valueChanged(QtProperty*, int);
        void surfaceIntegratorTypeEnumManager_valueChanged(QtProperty*, int);
        void rsTitleManager_valueChanged (QtProperty *, const QString &);
};

#endif // MAINWINDOW_HH
