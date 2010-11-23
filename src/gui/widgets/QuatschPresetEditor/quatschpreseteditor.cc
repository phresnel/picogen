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

#include "quatschpreseteditor.hh"
#include "ui_quatschpreseteditor.h"
#include "quatsch-preprocessor/parsing.hh"

#include <QTextEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QSpacerItem>
#include <QCheckBox>
#include <QComboBox>

QuatschPresetEditor::QuatschPresetEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuatschPresetEditor)
{
        ui->setupUi(this);
        setPreset ("(($foobar:integer = {1,2,3}))(($frob:real = {1,2,3}))(($ExtraNoise:boolean))\n"
                   "(+ (($foobar)) (($frob)) (if (($ExtraNoise)) 1 0))");
}

QuatschPresetEditor::~QuatschPresetEditor() {
        delete ui;
}

void QuatschPresetEditor::setPreset (std::string const &str) {
        using quatsch_preprocessor::Declaration;
        using quatsch_preprocessor::Declarations;

        preset = str;
        declarations = quatsch_preprocessor::findDeclarations(str);

        // According to documentation for QLayoutItem*QLayout::takeAt(int index)
        // (Qt 4.7) this is the safe way to remove all items from a QLayout.
        // (note: takeAt(0) yields a warning if there is no item, so we first
        //        check the item count to not get that warning)
        if (0) while (ui->layout->rowCount() > 0) {
                QLayoutItem *child = ui->layout->takeAt(0);
                delete child;
        }
        foreach (quatsch_preprocessor::Declaration decl, declarations) {
                QWidget *widget = 0;

                if (decl.domain().isLinear()) {
                        switch (decl.type()) {
                        case quatsch_preprocessor::Real:
                        {
                                QDoubleSpinBox *dsb = new QDoubleSpinBox(this);
                                dsb->setMinimum(decl.domainMin().value());
                                dsb->setMaximum(decl.domainMax().value());
                                widget = dsb;
                        }
                        break;
                        case quatsch_preprocessor::Integer:
                        {
                                QSpinBox *dsb = new QSpinBox(this);
                                dsb->setMinimum(decl.domainMin().value());
                                dsb->setMaximum(decl.domainMax().value());
                                widget = dsb;
                        }
                        break;
                        case quatsch_preprocessor::Boolean:
                                widget = new QCheckBox(this);
                                break;
                        }
                } else if (decl.hasFiniteDomain()
                        && decl.domainElementCount()<100)
                {
                        QComboBox *cb = new QComboBox(this);
                        typedef std::list<quatsch_preprocessor::DomainScalar>::
                                        const_iterator
                                dsiterator;
                        foreach (quatsch_preprocessor::DomainScalar ds,
                                 decl.domainElements())
                        {
                                cb->addItem(QString::fromStdString(
                                                ds.displayValue()),
                                            ds.value());
                        }
                        widget = cb;
                } else {
                        widget = new QTextEdit(this);
                }

                widget->setObjectName(QString::fromStdString(decl.id()));

                ui->layout->addRow(QString::fromStdString(decl.displayName()),
                                   widget);
        }
}

void QuatschPresetEditor::setPreset (QString const &str) {
        setPreset (str.toStdString());
}

void QuatschPresetEditor::setPreset (const char *str) {
        setPreset (std::string(str));
}

std::map<std::string, std::string> QuatschPresetEditor::replacements() const {
        std::map<std::string, std::string> ret;
        for (int i=0; i<ui->layout->rowCount(); ++i) {
                QLayoutItem *item = ui->layout->itemAt(i,
                                                       QFormLayout::FieldRole);
                QWidget *widget = item->widget();
                if (!widget)
                        continue;
                const std::string name = widget->objectName().toStdString();

                if (QDoubleSpinBox *ed = qobject_cast<QDoubleSpinBox*>(widget))
                        ret[name] = QString::number(ed->value()).toStdString();
                else if (QSpinBox *ed = qobject_cast<QSpinBox*>(widget))
                        ret[name] = QString::number(ed->value()).toStdString();
                else if (QCheckBox *ed = qobject_cast<QCheckBox*>(widget))
                        ret[name] = ed->isChecked() ? "1" : "0";
                else
                        ret[name] = "???";
        }
        return ret;
}

void QuatschPresetEditor::on_showPreprocessedCode_clicked() {
        ui->preprocessedCode->setText(QString::fromStdString(
                quatsch_preprocessor::replace(preset, replacements())
        ));
}
