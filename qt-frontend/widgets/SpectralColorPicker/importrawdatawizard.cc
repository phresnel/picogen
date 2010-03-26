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

#include "importrawdatawizard.hh"
#include "ui_importrawdatawizard.h"

#include <QFileDialog>
#include <QTextStream>
#include <QUrl>
#include <QDesktopServices>



namespace {
bool readDoubleIgnoreOthers (QTextStream &in, QVariant &out) {

        while(true) {
                if (in.atEnd())
                        return false;
                const qint64 initial_pos = in.pos();
                double double_val;
                in >> double_val;
                if (initial_pos != in.pos()) {
                        out = double_val;
                        return true;
                }

                QChar trash;
                in >> trash;
                if (initial_pos == in.pos())
                        return false;
        }

        /*do {
                QString str;
                in >> str;
                if (!QRegExp("[ \\n\\t]*").exactMatch(str)) {
                        out = str;
                        return true;
                }
        } while (!in.atEnd());*/

        return false;
}
}





ImportRawDataWizard::ImportRawDataWizard(QWidget *parent) :
        QWizard(parent),
        ui(new Ui::ImportRawDataWizard),
        currentPageValid(true)
{
        ui->setupUi(this);

        connect (this,
                SIGNAL(currentIdChanged(int)),
                SLOT(wizard_currentIdChanged(int)));

        ui->radioUniformRange->setEnabled(true);
        on_radioUniformRange_pressed ();

        ui->radioWavAmp->setEnabled(true);
        on_radioWavAmp_pressed ();

        ui->sourceUnitOfWavelength->setCurrentIndex(0);
        ui->sourceUnitOfAmplitude->setCurrentIndex(0);
        on_sourceUnitOfWavelength_currentIndexChanged(0);
        on_sourceUnitOfAmplitude_currentIndexChanged(0);

        ui->sourceAmplitudeCapEnable->setChecked(false);
        on_sourceAmplitudeCapEnable_toggled(
                        ui->sourceAmplitudeCapEnable->checkState());
}



ImportRawDataWizard::~ImportRawDataWizard() {
        delete ui;
}



void ImportRawDataWizard::changeEvent(QEvent *e) {
        QWizard::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
        default:
                break;
        }
}



void ImportRawDataWizard::on_openFileDialog_pressed() {
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::ExistingFile);

        dialog.setWindowTitle("Select a file");

        QList<QUrl> urls = dialog.sidebarUrls();
        urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
        urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
        dialog.setSidebarUrls(urls);

        if (dialog.exec()) {
                QFile file (dialog.selectedFiles()[0]);
                if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                         return;

                QTextStream in(&file);
                ui->rawData->setPlainText(in.readAll());
        }
}



bool ImportRawDataWizard::validateCurrentPage() {
        if (currentId() == 0) {
                if (QRegExp("[ \\n\\t]*").exactMatch(ui->rawData->toPlainText()))
                        return false;
        } else if (currentId() == 2) {
                if (ui->targetSamples->rowCount()==0)
                        return false;
        }
        return currentPageValid;
}



void ImportRawDataWizard::wizard_currentIdChanged (int id) {
        currentPageValid = true;

        if (id == 2) {

                /*
                QString str = ui->rawData->toPlainText();
                QTextStream in(&str);

                ui->sourceSamples->setRowCount(0);

                bool nothing = true;
                while (!in.atEnd()) {
                        QString sample;
                        in >> sample;
                        if (QRegExp("[ \\n\\t]*").exactMatch(sample))
                                continue;

                        const QRegExp dblregex (
                                "([0-9]*\\.[0-9]+)"
                                "|([0-9]+\\.[0-9]*)"
                                "|([0-9]+)"
                        );

                        QTableWidgetItem *item = new QTableWidgetItem(sample);
                        if (dblregex.exactMatch(sample)) {
                                item->setBackgroundColor(QColor(200,255,200));
                        } else {
                                item->setBackgroundColor(QColor(255,200,200));
                        }

                        const int currentRow = ui->sourceSamples->rowCount();
                        ui->sourceSamples->setRowCount(ui->sourceSamples->rowCount()+1);
                        ui->sourceSamples->setItem(currentRow, 1, item);

                        nothing = false;
                }

                if (nothing) {
                        currentPageValid = false;
                }
                */
                QVariantList wavelengths, amplitudes;

                QString str = ui->rawData->toPlainText();
                QTextStream in(&str);

                if (wavelengthImportMode == from_raw_data
                    && (rawLayout == wavelengths_amplitudes
                        || rawLayout == amplitudes_wavelengths)
                ) {
                        // First parse everything, then divide by two.
                        QVariantList samples;
                        while (!in.atEnd()) {
                                QString sample;
                                in >> sample;
                                if (QRegExp("[ \\n\\t]*").exactMatch(sample))
                                        continue;
                                samples.push_back(sample);
                        }

                        if (rawLayout == wavelengths_amplitudes) {
                                int s = 0;
                                for (; s<samples.size()/2; ++s)
                                        wavelengths.push_back(samples[s]);
                                for (; s<samples.size(); ++s)
                                        amplitudes.push_back(samples[s]);
                        } else {
                                int s = 0;
                                for (; s<samples.size()/2; ++s)
                                        amplitudes.push_back(samples[s]);
                                for (; s<samples.size(); ++s)
                                        wavelengths.push_back(samples[s]);
                        }
                } else {
                        while (!in.atEnd()) {
                                switch (wavelengthImportMode) {
                                case uniform_range:
                                case manual: {
                                        QVariant sample;
                                        if (!readDoubleIgnoreOthers(in, sample))
                                                goto behind_parsing;
                                        amplitudes.push_back(sample);
                                } break;
                                case from_raw_data:
                                        switch (rawLayout) {
                                        case amplitude_wavelength: {
                                                QVariant sample;
                                                if (!readDoubleIgnoreOthers(in, sample))
                                                        goto behind_parsing;
                                                amplitudes.push_back(sample);

                                                sample = "";
                                                if (!readDoubleIgnoreOthers(in, sample))
                                                        goto behind_parsing;
                                                wavelengths.push_back(sample);
                                        } break;
                                        case wavelength_amplitude: {
                                                QVariant sample;
                                                if (!readDoubleIgnoreOthers(in, sample))
                                                        goto behind_parsing;
                                                wavelengths.push_back(sample);

                                                sample = "";
                                                if (!readDoubleIgnoreOthers(in, sample))
                                                        goto behind_parsing;
                                                amplitudes.push_back(sample);
                                        } break;
                                        case amplitudes_wavelengths:
                                        case wavelengths_amplitudes:
                                                throw std::exception();
                                        }
                                        break;
                                }
                        }
                }

                behind_parsing:

                if (wavelengthImportMode == uniform_range) {
                        const size_t numSamples = amplitudes.size();
                        const double min = ui->uniformRangeMin->value();
                        const double max = ui->uniformRangeMax->value();
                        for (size_t s=0; s<numSamples; ++s) {
                                const double
                                        f = s/((double)numSamples-1),
                                        lambda = min + (max-min)*f
                                ;
                                wavelengths.push_back(lambda);
                        }
                }

                ui->sourceSamples->setRowCount(0);
                for (int s=0; s<amplitudes.size(); ++s) {
                        const int currentRow = ui->sourceSamples->rowCount();
                        ui->sourceSamples->setRowCount(ui->sourceSamples->rowCount()+1);

                        QTableWidgetItem *item = new QTableWidgetItem(amplitudes[s].toString());
                        ui->sourceSamples->setItem(currentRow, 1, item);

                        if (s<wavelengths.size()) {
                                QTableWidgetItem *item = new QTableWidgetItem(wavelengths[s].toString());
                                ui->sourceSamples->setItem(currentRow, 0, item);
                        }
                }
        }
}



void ImportRawDataWizard::on_radioUniformRange_pressed() {
        ui->wavelengthDetailOptionsStack->setCurrentIndex(0);
        wavelengthImportMode = uniform_range;
}

void ImportRawDataWizard::on_radioManual_pressed() {
        ui->wavelengthDetailOptionsStack->setCurrentIndex(1);
        wavelengthImportMode = manual;
}

void ImportRawDataWizard::on_radioFromRaw_pressed() {
        ui->wavelengthDetailOptionsStack->setCurrentIndex(2);
        wavelengthImportMode = from_raw_data;
}

void ImportRawDataWizard::on_radioAmpsWaves_pressed() {
        rawLayout = amplitudes_wavelengths;
}

void ImportRawDataWizard::on_radioWavesAmps_pressed() {
        rawLayout = wavelengths_amplitudes;
}

void ImportRawDataWizard::on_radioAmpWav_pressed() {
        rawLayout = amplitude_wavelength;
}

void ImportRawDataWizard::on_radioWavAmp_pressed() {
        rawLayout = wavelength_amplitude;
}

void ImportRawDataWizard::on_sourceUnitOfWavelengthFactor_editingFinished() {
        ui->sourceUnitOfWavelength->setCurrentIndex(
                ui->sourceUnitOfWavelength->count()-1
        );
}

void ImportRawDataWizard::on_sourceUnitOfAmplitudeFactor_editingFinished() {
        ui->sourceUnitOfAmplitude->setCurrentIndex(
                ui->sourceUnitOfAmplitude->count()-1
        );
}

void ImportRawDataWizard::on_sourceUnitOfWavelength_currentIndexChanged(int index) {
        if (0 == index) {
                ui->sourceUnitOfWavelengthFactor->setValue(1.);
                ui->sourceUnitOfWavelengthFactor->setEnabled(false);
        } else if (1 == index) {
                ui->sourceUnitOfWavelengthFactor->setValue(0.1);
                ui->sourceUnitOfWavelengthFactor->setEnabled(false);
        } else {
                ui->sourceUnitOfWavelengthFactor->setValue(1.);
                ui->sourceUnitOfWavelengthFactor->setEnabled(true);
        }

}

void ImportRawDataWizard::on_sourceUnitOfAmplitude_currentIndexChanged(int index) {
        Q_UNUSED(index)

        ui->sourceUnitOfAmplitudeFactor->setValue(1.);
        ui->sourceUnitOfAmplitudeFactor->setEnabled(true);
}

void ImportRawDataWizard::on_applyConversionButton_pressed() {
        ui->targetSamples->setRowCount(0);

        const double wavefac = ui->sourceUnitOfWavelengthFactor->value();
        const double ampfac = ui->sourceUnitOfAmplitudeFactor->value();

        const bool capInput = ui->sourceAmplitudeCapEnable->checkState();
        const double capMin = ui->sourceAmplitudeCapMin->value();
        const double capMax = ui->sourceAmplitudeCapMax->value();

        for (int y=0; y<ui->sourceSamples->rowCount(); ++y) {
                // Get from source-table.
                const QTableWidgetItem *waveit = ui->sourceSamples->item(y, 0);
                const QTableWidgetItem *ampit = ui->sourceSamples->item(y, 1);

                // Convert to double and scale.
                const QVariant wavevar(waveit->text());
                const QVariant ampvar(ampit->text());
                const double
                        wave = wavevar.value<double>() * wavefac,
                        amp_ = ampvar.value<double>() * ampfac,
                        amp = capInput
                            ? (amp_<capMin?capMin:amp_>capMax?capMax:amp_)
                            : amp_
                ;

                // Add row.
                const int currentRow = ui->targetSamples->rowCount();
                ui->targetSamples->setRowCount(currentRow+1);

                // Add non-editable items to new row.
                QTableWidgetItem *targetwavit = new QTableWidgetItem(QString::number(wave));
                targetwavit->setFlags(targetwavit->flags() & Qt::ItemIsEditable);
                QTableWidgetItem *targetampit = new QTableWidgetItem(QString::number(amp));
                targetampit->setFlags(targetampit->flags() & Qt::ItemIsEditable);

                ui->targetSamples->setItem(currentRow, 0, targetwavit);
                ui->targetSamples->setItem(currentRow, 1, targetampit);
        }
}

void ImportRawDataWizard::on_sourceAmplitudeCapEnable_toggled(bool checked) {
        ui->sourceAmplitudeCapMin->setEnabled(checked);
        ui->sourceAmplitudeCapMax->setEnabled(checked);
}
