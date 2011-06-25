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

namespace picogen { namespace cosyscene {
        class QuatschPreset;
} }

namespace picogen { namespace qt4_gui {

class QuatschPresetEditor : public QWidget {
        Q_OBJECT

public:
        explicit QuatschPresetEditor(QWidget *parent = 0);
        explicit QuatschPresetEditor(bool showSourceEditor, QWidget *parent = 0);
        ~QuatschPresetEditor();

        void showSourceEditor (bool show);

        void setPreset (std::string const &str);
        void setPreset (QString const &str);
        void setPreset (const char *str);

        QString preset() const;

        std::string getPreprocessedCode() const;
        picogen::cosyscene::QuatschPreset toCosy() const;
        void fromCosy (picogen::cosyscene::QuatschPreset const &);

signals:
        void formationChanged();

private:
        Ui::QuatschPresetEditor *ui;

        std::string preset_;
        quatsch_preprocessor::Declarations declarations;

        QWidget* createWidgetForDeclaration(
                quatsch_preprocessor::Declaration const &decl,
                QWidget *parent
        );

private:
        std::map<std::string, std::string> replacements() const;

private slots:
        void on_hidePreprocessedCode_clicked();
        void on_preset_textChanged();
        void on_showPreview_clicked();
        void on_showPreprocessedCode_clicked();
        void childWidgetEditingFinished();

};

} }

#endif // QUATSCHPRESETEDITOR_HH
