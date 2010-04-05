#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include "qtvariantproperty.h"
#include "QtStringPropertyManager"
#include "qtpropertymanager.h"
#include "qteditorfactory.h"

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

        QtProperty *renderSettingsProperty;
        QtProperty *camerasProperty, *currentCameraProperty, *currentTransformProperty;
        QtProperty *objectsProperty;
        QtEnumPropertyManager *transformEnumManager;
        QtEnumPropertyManager *objectTypeEnumManager;
        QtVariantPropertyManager *codeEditManager;
        redshift::shared_ptr<redshift::scenefile::Scene> createScene () const;

        void addRenderSettings (std::string const &name);
        void addCamera (std::string const &name);
        void addTransform (QtProperty *transformRoot,
                           redshift::scenefile::Camera::Transform::Type type);
        void addObject ();

        QtBrowserItem * currentBrowserItem;

        bool nonRecurseLock;
private slots:
        void on_deleteObjectButton_pressed();
        void on_deleteSubTransformButton_pressed();
        void on_codeEditor_codeChanged();
        void on_newObjectButton_pressed();
        void on_newSubTransformButton_pressed();
        void on_newRsButton_pressed();
        void on_actionRender_triggered();
        void on_actionShow_redshift_job_code_triggered();
        void on_settings_currentItemChanged(QtBrowserItem * current);

        // non MOC
        void code_valueChanged(QtProperty*, QVariant);
        void transformEnumManager_valueChanged(QtProperty*, int);
        void objectTypeEnumManager_valueChanged(QtProperty*, int);
        void rsTitleManager_valueChanged (QtProperty *, const QString &);
};

#endif // MAINWINDOW_HH
