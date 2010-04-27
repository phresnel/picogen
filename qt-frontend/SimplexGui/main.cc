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
#include "mainwindow.hh"
#include "renderwindow.hh"
#include <QMessageBox>
#include <QFile>
#include <QCleanlooksStyle>

#include <fstream>

#include "../../redshift/include/static_init.hh"

#undef main // SDL fix

int simplexGui (int argc, char *argv[]) {
        QCleanlooksStyle *style = new QCleanlooksStyle ();
        QApplication::setStyle(style);

        redshift::static_init();
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        const int ret = a.exec();
        return ret;
}



int productionRender (int argc, char *argv[]) {
        QCleanlooksStyle *style = new QCleanlooksStyle ();
        QApplication::setStyle(style);

        redshift::static_init();
        QApplication a(argc, argv);

        const QString pathToSource =
                argc>2
                ? QString(argv[2])
                : "";
        if ("" == pathToSource) {
                QMessageBox::critical(0, "Missing argument",
                        "No source-file has been specified.");
                return -1;
        } else if (!QFile::exists(pathToSource)) {
                QMessageBox::critical(0, "File not found",
                        "The source-file \"" + pathToSource + "\" has not been found.");
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
                QMessageBox::critical(0, "Missing argument(s)",
                  "Need an index [0..n] for the render setting and camera setting "
                  "to be used."
                );
                return -1;
        }

        try {
                redshift::scenefile::Scene scene;
                std::ifstream ss(pathToSource.toStdString().c_str());
                actuarius::IArchive (ss) & actuarius::pack("scene", scene);

                redshift::shared_ptr<redshift::scenefile::Scene> pscene =
                                redshift::shared_ptr<redshift::scenefile::Scene>(
                                                new redshift::scenefile::Scene(scene)
                                );

                RenderWindow w (pscene, renderSetting, cameraSetting, 0);
                w.show();
                const int ret = a.exec();
                return ret;
        } catch (std::exception const &e){
                QMessageBox::critical(0,
                      "Error upon loading",
                      "The selected file \"" + pathToSource + "\" could not be loaded, "
                      "are you sure this is a valid picogen file?"
                      );
                return -1;
        }


}



int main(int argc, char *argv[]) {
        if (argc>1 && !strcmp ("picogen-production-render", argv[1]))
                return productionRender(argc, argv);
        return simplexGui (argc, argv);
}
