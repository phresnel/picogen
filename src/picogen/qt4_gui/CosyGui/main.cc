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


#include <QtGui/QApplication>
#include <QCleanlooksStyle>
#include <QMessageBox>
#include <QFile>

#include "mainwindow.h"

#include <fstream>

#include "cosyscene/scene.hh"
#include "cosyscene/terrain.hh"
#include "cosyscene/save_load.hh"

#include "redshift/include/static_init.hh"
#include "redshift_file/scene.hh"
#include "renderwindow.h"

#undef main


#include "redshift_file/save_load.hh"


int cosy_main (int argc, char *argv[]) {
        picogen::redshift::static_init();

        QCleanlooksStyle *style = new QCleanlooksStyle ();
        QApplication::setStyle(style);

        QApplication a(argc, argv);
        picogen::qt4_gui::MainWindow w;
        w.show();
        return a.exec();
}


void showError (QString str, QString title="Error") {
        QMessageBox mb;
        mb.setIcon(QMessageBox::Critical);
        mb.setStandardButtons(QMessageBox::Ok);
        mb.setWindowTitle(title);
        mb.setText(str);
        mb.exec();
}

int production_render_main (int argc, char *argv[]) {
        QCleanlooksStyle *style = new QCleanlooksStyle ();
        QApplication::setStyle(style);

        picogen::redshift::static_init();
        QApplication a(argc, argv);

        const QString pathToSource =
                argc>2
                ? QString(argv[2])
                : "";
        if ("" == pathToSource) {
                showError("No source-file has been specified.", "Missing argument");
                return -1;
        } else if (!QFile::exists(pathToSource)) {
                showError ("The source-file \"" + pathToSource + "\" has not been found.",
                           "File not found");
                return -1;
        }

        const QString renderSettingsS =
                argc>3
                ? QString(argv[3])
                        : "";

        const QString cameraSettingsS =
                argc>4
                ? QString(argv[4])
                        : "";

        bool okayC, okayR;
        const int renderSetting = renderSettingsS.toInt(&okayC);
        const int cameraSetting = cameraSettingsS.toInt(&okayR);

        if ("" == renderSettingsS || "" == cameraSettingsS
            || cameraSetting<0 || renderSetting<0
            || !okayC || !okayR
        ) {
                showError ("Need an index [0..n] for the render setting and camera setting "
                           "to be used.",
                           "Missing argument(s)");
                return -1;
        }


        try {
                picogen::redshift_file::Scene scene;
                picogen::redshift_file::load_scene(scene, pathToSource.toStdString());

                const std::clock_t b = std::clock();
                while (!QFile::remove(pathToSource)) {
                        if ((std::clock() - b)>(CLOCKS_PER_SEC*10)) {
                                showError ("I am unable to remove the temporary file \""
                                           + pathToSource + "\". I am still able to render "
                                           "your terrain, but you should ensure to delete that file.");
                                break;
                        }
                }

                if (scene.renderSettingsCount() == 0
                    || scene.cameraCount() == 0) {
                        showError ("Invalid scene (no camera settings, "
                                   "no render settings.");
                        return 0;
                }

                picogen::redshift::shared_ptr<picogen::redshift_file::Scene> pscene =
                                picogen::redshift::shared_ptr<picogen::redshift_file::Scene>(
                                                new picogen::redshift_file::Scene(scene)
                                );
                picogen::qt4_gui::RenderWindow w (pscene, renderSetting, cameraSetting, 0, 15.);
                w.show();
                const int ret = a.exec();
                return ret;
        } catch (std::exception const &e){
                showError (
                      "The selected file \"" + pathToSource + "\" could not be loaded, "
                      "are you sure this is a valid picogen file?\n\n"
                      + QString("(exception: ")+e.what()+")"
                );
                return 0;
        } catch (...) {
                showError (
                      "The selected file \"" + pathToSource + "\" could not be loaded, "
                      "are you sure this is a valid picogen file?\n\n"
                      "(unknown error; possibly the file is not a valid picogen file)"
                );
                return 0;
        }
}

int main(int argc, char *argv[]) {
        if (argc>1 && !strcmp ("--cosygui-production-render", argv[1])) {
                return production_render_main(argc, argv);
        }
        return cosy_main(argc, argv);
}
