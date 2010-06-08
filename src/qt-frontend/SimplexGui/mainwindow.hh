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

#include "redshift/include/smart_ptr.hh"


namespace Ui {
    class MainWindow;
}

class ScenePropertyBrowser;

class QtProperty;
class QtBrowserItem;
class QtVariantPropertyManager;



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
        void setupUi();
        void loadScene (QString const &name);

        QString askForNewSaveFilename() ;
        QString getAndUpdateSaveFilename();

        void refreshWindowTitle();

        void render();


        Ui::MainWindow *ui;

        ScenePropertyBrowser *propertyBrowser;

        QtVariantPropertyManager *codeEditManager;

        QString saveFilename;
        bool changed;


        bool nonRecurseLock; // TODO: get rid of that

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

        void code_valueChanged(QtProperty*, QVariant);

        void updateUi();
        void setChanged ();
        void setUnchanged ();
        void resyncRenderSettingConfig ();
        void resyncCameraConfig ();
};

#endif // MAINWINDOW_HH
