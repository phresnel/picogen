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

#ifndef QUATSCHPRESETEDITOR_HH
#define QUATSCHPRESETEDITOR_HH

#include <QWidget>
#include <string>
#include <map>

#include "quatsch-preprocessor/meta.hh"

namespace Ui {
        class QuatschPresetEditor;
}

class QuatschPresetEditor : public QWidget {
        Q_OBJECT

public:
        explicit QuatschPresetEditor(QWidget *parent = 0);
        ~QuatschPresetEditor();

        void setPreset (std::string const &str);
        void setPreset (QString const &str);
        void setPreset (const char *str);
private:
        Ui::QuatschPresetEditor *ui;

        std::string preset;
        quatsch_preprocessor::Declarations declarations;

private:
        std::map<std::string, std::string> replacements() const;

private slots:
    void on_showPreprocessedCode_clicked();
};

#endif // QUATSCHPRESETEDITOR_HH
