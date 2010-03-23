#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include "qtvariantproperty.h"
#include "QtStringPropertyManager"
#include "qtpropertymanager.h"
#include "qteditorfactory.h"

#include "../../redshift/include/smart_ptr.hh"

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
        void addRenderSettings (std::string const &name);
        redshift::shared_ptr<redshift::scenefile::Scene> createScene () const;

private slots:
        void on_newRsButton_pressed();
        void on_actionRender_triggered();
        void on_actionShow_redshift_job_code_triggered();

        // non-moc
        void rsTitleManager_valueChanged ( QtProperty * property, const QString & value );
};

#endif // MAINWINDOW_HH
