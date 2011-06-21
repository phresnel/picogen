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

#include "quatschpreseteditor.h"
#include "ui_quatschpreseteditor.h"
#include "quatsch-preprocessor/parsing.hh"
#include "cosyscene/terrain.hh"

#include <cmath>

#include "filenameedit.h"

#include <QTextEdit>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QSpacerItem>
#include <QCheckBox>
#include <QComboBox>
#include <QMessageBox>

#include <QFormLayout>


QuatschPresetEditor::QuatschPresetEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuatschPresetEditor)
{
        ui->setupUi(this);

        showSourceEditor(false);

        setPreset(
        "(($Filename:filename))\n"
        "(($Mode:enumeration={Bilinear, Cubic}))\n"
        "(frob <--(($Filename))--> [(($Mode))])"
        );
}

void QuatschPresetEditor::showSourceEditor (bool show) {
        /*ui->preprocessedCode->setVisible(show);
        ui->preprocessedCode->setEnabled(show);

        ui->showPreprocessedCode->setVisible(show);
        ui->showPreprocessedCode->setEnabled(show);

        ui->preset->setVisible(show);
        ui->preset->setEnabled(show);*/

        ui->codebox->setVisible(show);
        ui->codebox->setEnabled(show);
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

        if (decl.hasImplicitDomain()) {
                switch (decl.type()) {
                case quatsch_preprocessor::EnumerationValue:
                        throw std::runtime_error(
                        "QuatschPresetEditor::createWidgetForDeclaration(): "
                        "'EnumerationValue' is not a possible type for implicit domains."
                        );
                case quatsch_preprocessor::Filename: {
                        FilenameEdit *te = new FilenameEdit (parent);
                        widget = te;
                        QObject::connect(te, SIGNAL(textChanged()),
                                         SLOT(childWidgetEditingFinished()));
                } break;
                case quatsch_preprocessor::Boolean:
                {
                        QCheckBox *cb = new QCheckBox(parent);
                        if (decl.hasDefaultValue())
                                cb->setChecked(decl.defaultValue().scalar().asBool());
                        widget = cb;
                        QObject::connect(cb, SIGNAL(toggled(bool)),
                                         SLOT(childWidgetEditingFinished()));
                } break;
                case quatsch_preprocessor::Real:
                {
                        QDoubleSpinBox *dsb = new QDoubleSpinBox(parent);
                        dsb->setRange(-99999999,
                                      +99999999);
                        dsb->setDecimals(6);
                        if (decl.hasDefaultValue())
                                dsb->setValue(decl.defaultValue().scalar().value());
                        widget = dsb;
                        QObject::connect(dsb, SIGNAL(editingFinished()),
                                         SLOT(childWidgetEditingFinished()));
                } break;
                case quatsch_preprocessor::Integer:
                {
                        QSpinBox *dsb = new QSpinBox(parent);
                        dsb->setRange(-99999999,
                                      +99999999);
                        if (decl.hasDefaultValue())
                                dsb->setValue(decl.defaultValue().scalar().value());
                        widget = dsb;

                        QObject::connect(dsb, SIGNAL(editingFinished()),
                                         SLOT(childWidgetEditingFinished()));
                } break;
                };
        } else if (decl.domain().isLinear()) {
                switch (decl.type()) {
                case quatsch_preprocessor::Filename:
                        throw std::runtime_error(
                        "QuatschPresetEditor::createWidgetForDeclaration(): "
                        "'Filename' is not a possible type for linear domains."
                        );
                case quatsch_preprocessor::Boolean:
                        throw std::runtime_error(
                        "QuatschPresetEditor::createWidgetForDeclaration(): "
                        "'Boolean' is not a possible type for linear domains."
                        );
                case quatsch_preprocessor::EnumerationValue:
                        throw std::runtime_error(
                        "QuatschPresetEditor::createWidgetForDeclaration(): "
                        "'EnumerationValue' is not a possible type for linear domains."
                        );
                case quatsch_preprocessor::Real: {
                        QDoubleSpinBox *dsb = new QDoubleSpinBox(parent);
                        dsb->setRange(decl.domainMin().value(),
                                      decl.domainMax().value());
                        double d = std::fabs(decl.domainMin().value()
                                             -decl.domainMax().value())
                                   / 100.;
                        dsb->setSingleStep(d>1.?1.:d);
                        dsb->setDecimals(6);
                        if (decl.hasDefaultValue())
                                dsb->setValue(decl.defaultValue().scalar().value());
                        widget = dsb;
                        QObject::connect(dsb, SIGNAL(editingFinished()),
                                         SLOT(childWidgetEditingFinished()));
                } break;
                case quatsch_preprocessor::Integer: {
                        QSpinBox *dsb = new QSpinBox(parent);
                        dsb->setRange(decl.domainMin().value(),
                                      decl.domainMax().value());
                        if (decl.hasDefaultValue())
                                dsb->setValue(decl.defaultValue().scalar().value());
                        widget = dsb;

                        QObject::connect(dsb, SIGNAL(editingFinished()),
                                         SLOT(childWidgetEditingFinished()));
                } break;
                }
        } else if (decl.hasFiniteDomain()
                && decl.domainElementCount()<100)
        {
                QComboBox *cb = new QComboBox(parent);
                cb->setMaxVisibleItems(16);
                QObject::connect(cb, SIGNAL(currentIndexChanged(int)),
                                 SLOT(childWidgetEditingFinished()));

                if (decl.domain().isEnumeration()) {
                        using quatsch_preprocessor::DomainEnumeration;

                        quatsch_preprocessor::DomainEnumeration e =
                                        decl.domain().asEnumeration();
                        for (DomainEnumeration::const_iterator it=e.begin(),
                                end = e.end(); it != end; ++it)
                        {
                                cb->addItem(QString::fromStdString(*it),
                                            QString::fromStdString(*it));
                        }

                        if (decl.hasDefaultValue()) {
                                cb->setCurrentIndex(decl.defaultValue().scalar().value());
                        }
                } else {
                        typedef std::list<quatsch_preprocessor::DomainScalar>::
                                        const_iterator
                                dsiterator;
                        foreach (quatsch_preprocessor::DomainScalar ds,
                                 decl.domainElements())
                        {
                                cb->addItem(QString::fromStdString(ds.displayValue()),
                                            ds.value());
                        }
                        if (decl.hasDefaultValue()) {
                                cb->setCurrentIndex(decl.defaultValue().scalar().value());
                        }
                }
                widget = cb;
        } else {
                // TODO: build up regex
                //switch (decl.type())
                /*
                QTextEdit *te = new QTextEdit(parent);
                widget = te;
                QObject::connect(te, SIGNAL(textChanged()),
                                 SLOT(childWidgetEditingFinished()));*/
        }

        return widget;
}

void QuatschPresetEditor::childWidgetEditingFinished() {
        emit formationChanged();
}

void QuatschPresetEditor::setPreset (std::string const &str) {
        using quatsch_preprocessor::Declaration;
        using quatsch_preprocessor::Declarations;

        if (ui->preset->isEnabled()) {
                const bool blocked = ui->preset->blockSignals(true);
                ui->preset->setText(QString::fromStdString(str));
                ui->preset->blockSignals(blocked);
        }

        // Creating a whole new layout. See
        // * http://bugreports.qt.nokia.com/browse/QTBUG-15990
        // * http://bugreports.qt.nokia.com/browse/QTBUG-15991
        QFormLayout *newLayout = new QFormLayout();

        if (QLayout *oldLayout = ui->scrollAreaWidgetContents->layout()) {
                while (QLayoutItem *it = oldLayout->takeAt(0)) {
                        delete it->widget();
                        delete it;
                }
                delete oldLayout;
        }
        ui->scrollAreaWidgetContents->setLayout(newLayout);

        preset_ = str;
        declarations = quatsch_preprocessor::findDeclarations(str);
        foreach (quatsch_preprocessor::Declaration decl, declarations) {
                QWidget *widget = createWidgetForDeclaration(decl, this);
                widget->setObjectName(QString::fromStdString(decl.id()));
                newLayout->addRow(QString::fromStdString(decl.displayName()),
                                  widget);
        }
}

void QuatschPresetEditor::setPreset (QString const &str) {
        setPreset (str.toStdString());
}

void QuatschPresetEditor::setPreset (const char *str) {
        setPreset (std::string(str));
}

QString QuatschPresetEditor::preset() const {
        return QString::fromStdString(preset_);
}

void QuatschPresetEditor::fromCosy (picogen::cosyscene::QuatschPreset const &qp) {
        using std::map; using std::string;
        namespace cosyscene = picogen::cosyscene;

        setPreset (qp.preset());

        map<string,string> repls = qp.replacements();

        QFormLayout *layout = (QFormLayout*)ui->scrollAreaWidgetContents->layout();
        for (int i=0; i<layout->rowCount(); ++i) {
                QLayoutItem *item = layout->itemAt(i,
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
                } else if (QLineEdit *ed = qobject_cast<QLineEdit*>(widget)) {
                        ed->setText(var.toString());
                } else if (FilenameEdit *ed = qobject_cast<FilenameEdit*>(widget)) {
                        ed->setFilename(var.toString());
                } else {
                        QMessageBox::warning(this, "Error", "unknown editor in "
                                             "QuatschPresetEditor::fromCosy()");
                }
        }
}

std::map<std::string, std::string> QuatschPresetEditor::replacements() const {
        std::map<std::string, std::string> ret;
        QFormLayout *layout = (QFormLayout*)ui->scrollAreaWidgetContents->layout();
        if (!layout)
                return ret;
        for (int i=0; i<layout->rowCount(); ++i) {
                QLayoutItem *item = layout->itemAt(i,
                                                   QFormLayout::FieldRole);
                QWidget *widget = item->widget();
                if (!widget)
                        continue;
                const std::string name = widget->objectName().toStdString();

                if (QDoubleSpinBox *ed = qobject_cast<QDoubleSpinBox*>(widget)){
                        ret[name] = QString::number(ed->value()).toStdString();
                } else if (QSpinBox *ed = qobject_cast<QSpinBox*>(widget)){
                        ret[name] = QString::number(ed->value()).toStdString();
                } else if (QCheckBox *ed = qobject_cast<QCheckBox*>(widget)){
                        ret[name] = ed->isChecked() ? "1" : "0";
                } else if (QComboBox *ed = qobject_cast<QComboBox*>(widget)){
                        ret[name] = ed->itemData(ed->currentIndex())
                                    .toString().toStdString();
                } else if (QLineEdit *ed = qobject_cast<QLineEdit*>(widget)){
                        ret[name] = ed->text().toStdString();
                } else if (FilenameEdit *ed = qobject_cast<FilenameEdit*>(widget)){
                        ret[name] = ed->filename().toStdString();
                } else {
                        ret[name] = "???Error: Unsupported Editor???";
                }
        }
        return ret;
}

void QuatschPresetEditor::on_showPreprocessedCode_clicked() {
        ui->preprocessedCode->setVisible(true);
        ui->preprocessedCode->setText(QString::fromStdString(
                getPreprocessedCode()
        ));
}

std::string QuatschPresetEditor::getPreprocessedCode() const {
        return quatsch_preprocessor::replace(preset_, replacements());
}

picogen::cosyscene::QuatschPreset QuatschPresetEditor::toCosy() const {
        picogen::cosyscene::QuatschPreset qp;
        qp.setPreset(this->preset_);

        typedef std::map<std::string, std::string>::const_iterator iter;
        const std::map<std::string, std::string> repls = replacements();
        for (iter it=repls.begin(), end=repls.end(); it!=end; ++it) {
                qp.setReplacement(it->first, it->second);
        }
        return qp;
}

void QuatschPresetEditor::on_showPreview_clicked() {
        try {
                if (QString::fromStdString(preset_).trimmed().length() == 0) {
                        ui->preview->setStatusText("Preset is empty.");
                        return;
                }
                std::string const code = getPreprocessedCode();
                ui->preview->compileAndRun(code);
        } catch (std::exception const &ex) {
                ui->preview->setStatusText("An error occurred: " +
                                           QString::fromAscii(ex.what()));
        }
}

void QuatschPresetEditor::on_preset_textChanged() {
        setPreset (ui->preset->toPlainText());
        emit formationChanged();
}

void QuatschPresetEditor::on_hidePreprocessedCode_clicked() {
        ui->preprocessedCode->setVisible(false);
}

#include "quatschpreseteditor.moc"
