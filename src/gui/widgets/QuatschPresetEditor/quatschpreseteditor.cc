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
#include "cosyscene/terrain.hh"

#include <QTextEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QSpacerItem>
#include <QCheckBox>
#include <QComboBox>
#include <QMessageBox>

//-----------------------------------------------------------------------------
namespace {



} // namespace {
//-----------------------------------------------------------------------------

QuatschPresetEditor::QuatschPresetEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuatschPresetEditor)
{
        ui->setupUi(this);
        /*setPreset ("(($foobar:real = {1,2,3}))(($frob:real = {1,2,3}))(($ExtraNoise:boolean))\n"
                   "(sin (* (($frob)) x))");*/
        ui->preprocessedCode->setVisible(false);
        ui->preprocessedCode->setEnabled(false);
        ui->showPreprocessedCode->setVisible(false);
        ui->showPreprocessedCode->setEnabled(false);
}

QuatschPresetEditor::~QuatschPresetEditor() {
        delete ui;
}

QWidget* QuatschPresetEditor::createWidgetForDeclaration (
        quatsch_preprocessor::Declaration const &decl,
        QWidget *parent
)
{
        QWidget *widget = 0;

        if (decl.domain().isLinear()) {
                switch (decl.type()) {
                case quatsch_preprocessor::Real:
                {
                        QDoubleSpinBox *dsb = new QDoubleSpinBox(parent);
                        dsb->setMinimum(decl.domainMin().value());
                        dsb->setMaximum(decl.domainMax().value());
                        widget = dsb;
                        QObject::connect(dsb, SIGNAL(editingFinished()),
                                         SLOT(childWidgetEditingFinished()));
                }
                break;
                case quatsch_preprocessor::Integer:
                {
                        QSpinBox *dsb = new QSpinBox(parent);
                        dsb->setMinimum(decl.domainMin().value());
                        dsb->setMaximum(decl.domainMax().value());
                        widget = dsb;

                        QObject::connect(dsb, SIGNAL(editingFinished()),
                                         SLOT(childWidgetEditingFinished()));
                }
                break;
                case quatsch_preprocessor::Boolean:
                {
                        QCheckBox *cb = new QCheckBox(parent);
                        widget = cb;
                        QObject::connect(cb, SIGNAL(toggled(bool)),
                                         SLOT(childWidgetEditingFinished()));
                }
                break;
                }
        } else if (decl.hasFiniteDomain()
                && decl.domainElementCount()<100)
        {
                QComboBox *cb = new QComboBox(parent);
                QObject::connect(cb, SIGNAL(currentIndexChanged(int)),
                                 SLOT(childWidgetEditingFinished()));
                typedef std::list<quatsch_preprocessor::DomainScalar>::
                                const_iterator
                        dsiterator;
                foreach (quatsch_preprocessor::DomainScalar ds,
                         decl.domainElements())
                {
                        cb->addItem(QString::fromStdString(ds.displayValue()),
                                    ds.value());
                }
                widget = cb;
        } else {
                QTextEdit *te = new QTextEdit(parent);
                widget = te;
                QObject::connect(te, SIGNAL(textChanged()),
                                 SLOT(childWidgetEditingFinished()));
        }

        return widget;
}

void QuatschPresetEditor::childWidgetEditingFinished() {
        emit formationChanged();
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
        const bool block = ui->layout->blockSignals(true);
        while (ui->layout->rowCount() > 0) {
                QLayoutItem *child = ui->layout->takeAt(0);
                if (!child) break;
                delete child;
        }

        foreach (quatsch_preprocessor::Declaration decl, declarations) {
                QWidget *widget = createWidgetForDeclaration(decl, this);

                widget->setObjectName(QString::fromStdString(decl.id()));

                ui->layout->addRow(QString::fromStdString(decl.displayName()),
                                   widget);
        }
        ui->layout->blockSignals(block);
}

void QuatschPresetEditor::setPreset (QString const &str) {
        setPreset (str.toStdString());
}

void QuatschPresetEditor::setPreset (const char *str) {
        setPreset (std::string(str));
}

void QuatschPresetEditor::fromCosy (cosyscene::QuatschPreset const &qp) {
        using std::map; using std::string;

        setPreset (qp.preset());

        map<string,string> repls = qp.replacements();

        for (int i=0; i<ui->layout->rowCount(); ++i) {
                QLayoutItem *item = ui->layout->itemAt(i,
                                                       QFormLayout::FieldRole);
                QWidget *widget = item->widget();
                if (!widget)
                        continue;
                const std::string name = widget->objectName().toStdString();

                if (repls.find(name) == repls.end())
                        continue;

                const QVariant var = QString::fromStdString(repls[name]);
                if (QDoubleSpinBox *ed = qobject_cast<QDoubleSpinBox*>(widget)) {
                        ed->setValue(var.toDouble());
                } else if (QSpinBox *ed = qobject_cast<QSpinBox*>(widget)) {
                        ed->setValue(var.toDouble());
                } else if (QCheckBox *ed = qobject_cast<QCheckBox*>(widget)) {
                        ed->setChecked(var.toBool());
                } else if (QComboBox *ed = qobject_cast<QComboBox*>(widget)) {
                        int d = ed->findData(var);
                        ed->setCurrentIndex(d);
                } else {
                        QMessageBox::warning(this, "Error", "unknown editor in "
                                             "QuatschPresetEditor::fromCosy()");
                }
        }
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
                else if (QComboBox *ed = qobject_cast<QComboBox*>(widget))
                        ret[name] = ed->itemData(ed->currentIndex())
                                    .toString().toStdString();
                else
                        ret[name] = "???Error: Unsupported Editor???";
        }
        return ret;
}

void QuatschPresetEditor::on_showPreprocessedCode_clicked() {
        ui->preprocessedCode->setText(QString::fromStdString(
                getPreprocessedCode()
        ));
}

std::string QuatschPresetEditor::getPreprocessedCode() const {
        return quatsch_preprocessor::replace(preset, replacements());
}

cosyscene::QuatschPreset QuatschPresetEditor::toCosy() const {
        cosyscene::QuatschPreset qp;
        qp.setPreset(this->preset);

        typedef std::map<std::string, std::string>::const_iterator iter;
        const std::map<std::string, std::string> repls = replacements();
        for (iter it=repls.begin(), end=repls.end(); it!=end; ++it) {
                qp.setReplacement(it->first, it->second);
        }
        return qp;
}

void QuatschPresetEditor::on_showPreview_clicked() {
        ui->preview->compileAndRun(getPreprocessedCode());
}
