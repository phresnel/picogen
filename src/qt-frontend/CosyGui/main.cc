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

#include <fstream>

#include "cosyscene/scene.hh"
#include "cosyscene/terrain.hh"
#include "cosyscene/save_load.hh"

int main(int argc, char *argv[])
{
        /*Stash<int> foo;
        foo.stash(5);
        foo.stash(7);
        foo.stash(1);*/


        /*
        cosyscene::Terrain t;
        t.stash();
        std::ofstream fs("test.cosygui");
        OArchive (fs) & pack("terrain", t);
        */
        if (0) {
                cosyscene::Scene s;
                s.terrain()->toQuatschSource(cosyscene::QuatschSource("/*hello world*/ 0"));
                s.terrain()->stash();
                s.terrain()->toQuatschSource(cosyscene::QuatschSource("/*new try*/ 1"));
                std::ofstream fs("test.cosygui");
                save_scene (s, fs);
        } else {
                cosyscene::Scene s;
                {
                        std::ifstream fs("test.cosygui");
                        load_scene (s, fs);
                }
                {
                        std::ofstream fs("testtest.cosygui");
                        save_scene (s, fs);
                }
        }

        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
}
