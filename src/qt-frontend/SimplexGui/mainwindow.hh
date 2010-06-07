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
#include <QMenu>
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
class ObjectPropertyBrowser;
class VolumePropertyBrowser;
class RenderSettingsPropertyBrowser;

class MainWindow : public QMainWindow {
        Q_OBJECT
public:
        MainWindow(QString openFileName, QString initialFilename, QWidget *parent = 0);
        ~MainWindow();

protected:
        void changeEvent(QEvent *e);
        void contextMenuEvent(QContextMenuEvent *event);
        void closeEvent (QCloseEvent *);
        bool eventFilter(QObject *o, QEvent *e);

private:
        Ui::MainWindow *ui;
        void setupUi();
        void setDefaultScene();
        void loadScene (redshift::scenefile::Scene const &scene);
        void loadScene (QString const &name);

        ObjectPropertyBrowser *objectPropertyBrowser;
        VolumePropertyBrowser *volumePropertyBrowser;
        RenderSettingsPropertyBrowser *renderSettingsPropertyBrowser;

        QtLineEditFactory *lineEditFactory;

        QtVariantPropertyManager *variantManager;
        QtVariantEditorFactory *variantFactory;

        QtGroupPropertyManager *groupManager;

        QtEnumPropertyManager *enumManager;
        QtEnumEditorFactory *comboBoxFactory;

        ColorEditManager *colorEditManager;
        ColorEditFactory *colorEditFactory;

        QtProperty *camerasProperty, *filmSettingsProperty,
                   *currentCameraProperty,
                   *backgroundsProperty,
                   *currentTransformProperty,
                   *currentRenderSettingProperty;
        QtProperty *pssSunSkyProperty;
        QtEnumPropertyManager
                        *transformEnumManager,
                        *cameraTypeEnumManager;
        QtVariantPropertyManager *codeEditManager;

        void resyncCameraConfig ();
        void addCamera (redshift::scenefile::Camera const&);
        void addTransform (QtProperty *transformRoot,
                           redshift::scenefile::Transform const &);

        void initializeFilmSettings();
        void setFilmSettings(redshift::scenefile::FilmSettings const &);

        void initializeCameraSettings ();

        void initializeBackgrounds ();
        // this method won't have a long stay once multiple backgrounds
        // (consider e.g. preetham + starfield + moon) are supported
        void setBackground (redshift::scenefile::Background const &);

        void initializeScene();

        redshift::scenefile::Material readMaterial (QList<QtProperty*> subs, QString name="material") const;
        redshift::scenefile::Color readColor (QList<QtProperty*> subs, QString name="color") const;

        QtBrowserItem * currentBrowserItem;
        QString saveFilename;
        bool changed;

        QString askForNewSaveFilename() ;
        QString getAndUpdateSaveFilename();

        void refreshWindowTitle();

        redshift::shared_ptr<redshift::scenefile::Scene> createScene () const;
        QString sceneToCode();

        void render();
        void setUnchanged ();

        bool nonRecurseLock;
        QMenu settingsContextMenu, objectsMenu, volumesMenu;
private slots:
        void on_moveDownButton_clicked();
        void on_moveUpButton_clicked();
        void on_actionActionShow_Contextual_Picohelp_triggered();
        void on_actionShow_Picohelp_Browser_triggered(bool checked);
        void on_actionMaximize_triggered();
        void on_actionClose_triggered();

        void on_actionNew_Scene_triggered();
        void on_actionShow_Command_Pile_triggered(bool checked);
        void on_actionAdd_Homogeneous_Volume_triggered();
        void on_actionAdd_Exponential_Volume_triggered();
        void on_actionDelete_Volume_triggered();

        void on_actionAdd_Lazy_Quadtree_triggered();
        void on_actionAdd_Horizon_Plane_triggered();
        void on_actionAdd_Water_Plane_triggered();
        void on_actionDelete_Object_triggered();

        void on_actionNew_Camera_triggered();
        void on_actionDelete_Camera_triggered();

        void on_actionNew_Sub_Transform_triggered();
        void on_actionDelete_Sub_Transform_triggered();

        void on_actionDelete_Render_Setting_triggered();
        void on_actionNew_Render_Setting_triggered();
        void on_settings_customContextMenuRequested(QPoint pos);
        void on_actionProduction_Render_triggered();
        void on_actionLoad_triggered();
        void on_renderButton_clicked();
        void on_deleteCameraButton_clicked();
        void on_newCameraButton_clicked();
        void on_deleteRsButton_clicked();
        void on_deleteObjectButton_clicked();
        void on_deleteVolumeButton_clicked();
        void on_deleteSubTransformButton_clicked();

        void on_newHorizonPlaneButton_clicked();
        void on_newWaterPlaneButton_clicked();
        void on_newLazyQuadtreeButton_clicked();

        void on_newExponentialVolumeButton_clicked();
        void on_newHomogeneousVolumeButton_clicked();

        void on_newSubTransformButton_clicked();
        void on_newRsButton_clicked();

        void on_actionSave_as_triggered();
        void on_actionSave_copy_as_triggered();
        void on_action_Save_triggered();
        void on_actionRender_triggered();
        void on_actionShow_redshift_job_code_triggered();

        void on_settings_currentItemChanged(QtBrowserItem * current);
        void on_codeEditor_codeChanged();

        void on_actionShow_compiler_triggered();

        void on_picohelp_sceneFileClicked (QString const &);

        // non MOC
        void helpBrowserVisibilityRequested();

        void variantManager_valueChanged (QtProperty *, QVariant);
        void enumManager_valueChanged (QtProperty *, int);
        void colorEditManager_valueChanged (QtProperty *, ColorPickerColor);

        void code_valueChanged(QtProperty*, QVariant);
        void transformEnumManager_valueChanged(QtProperty*, int);
        void cameraTypeEnumManager_valueChanged(QtProperty*, int);

        void updateUi();
        void setChanged ();
        void resyncRenderSettingConfig ();
};

#endif // MAINWINDOW_HH
