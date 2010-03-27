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



#ifndef IMPORTRAWDATAWIZARD_HH
#define IMPORTRAWDATAWIZARD_HH

#include <QWizard>

namespace Ui {
        class ImportRawDataWizard;
}

class ImportRawDataWizard : public QWizard {
        Q_OBJECT
public:
        ImportRawDataWizard(QWidget *parent = 0);
        ~ImportRawDataWizard();

        bool validateCurrentPage();

        QVector<QPair<double, double> > const &converted() const ;
        QVector<double> const &amplitudes() const ;
        QVector<double> const &wavelengths() const ;

protected:
        void changeEvent(QEvent *e);

private:
        Ui::ImportRawDataWizard *ui;

        bool currentPageValid;


        enum { uniform_range, manual, from_raw_data } wavelengthImportMode;
        enum { amplitude_wavelength, wavelength_amplitude,
               amplitudes_wavelengths, wavelengths_amplitudes } rawLayout;

        QVector<QPair<double,double> > converted_;
        QVector<double> amplitudes_;
        QVector<double> wavelengths_;

private slots:
        void on_sourceAmplitudeCapEnable_toggled(bool checked);
        void on_applyConversionButton_pressed();
        void on_sourceUnitOfAmplitude_currentIndexChanged(int);
        void on_sourceUnitOfWavelength_currentIndexChanged(int);
        void on_sourceUnitOfAmplitudeFactor_editingFinished();
        void on_sourceUnitOfWavelengthFactor_editingFinished();
        void on_radioWavAmp_pressed();
        void on_radioAmpWav_pressed();
        void on_radioWavesAmps_pressed();
        void on_radioAmpsWaves_pressed();
        void on_radioUniformRange_pressed();
        void on_radioManual_pressed();
        void on_radioFromRaw_pressed();

        void on_openFileDialog_pressed();

        // hmm, does not work automagically with on_ImportRawDataWizard_currentIdChanged
        void wizard_currentIdChanged (int id);
};


#endif // IMPORTRAWDATAWIZARD_HH
