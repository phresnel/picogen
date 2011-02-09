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

#include "material-ui.hh"
#include "ui_material-ui.h"

#include "cosyscene/material.hh"
#include "scopedblocksignals.hh"

Material::Material(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Material),
    material(new cosyscene::Material())
{
    ui->setupUi(this);
}

Material::~Material()
{
    delete ui;
}

void Material::on_colorPicker_colorChanged(ColorPickerColor const &cpc){

        cosyscene::Material &mat = *this->material;

        cosyscene::Color mono;
        switch (cpc.mode) {
        case ColorPickerColor::Tristimulus:
                mono.toRgb(cosyscene::Rgb(
                            cpc.tristimulus.redF(),
                            cpc.tristimulus.greenF(),
                            cpc.tristimulus.blueF()));
                break;
        case ColorPickerColor::Spectral:
                cosyscene::Spectrum spec;

                std::vector<cosyscene::WavelengthAmplitudePair> waps;
                for (int i=0; i<cpc.spectral.size(); ++i) {
                        waps.push_back(cosyscene::WavelengthAmplitudePair(
                                cpc.spectral[i].wavelength,
                                cpc.spectral[i].amplitude
                        ));
                }
                mono.toSpectrum(cosyscene::Spectrum(waps));
                break;
        }
        mat.toMonochrome(mono);

        emit materialChanged();
}


void Material::setMaterial(
        redshift::shared_ptr<cosyscene::Material> m,
        bool blockSignals
) {
        ScopedQtSignalBlock blockMe (this, blockSignals);
        this->material = m;
        const cosyscene::Material &material = *this->material;

        switch (material.kind()) {
        case cosyscene::Material::None:
                std::cerr << "In TerrainTexturing::setMaterial(), 'None' is "
                                "unhandled";
                break;
        case cosyscene::Material::Monochrome: {
                const cosyscene::Color mono = material.monochrome();
                ColorPickerColor cpc;

                switch (mono.kind()) {
                case cosyscene::Color::Rgb:
                        cpc.mode = ColorPickerColor::Tristimulus;
                        cpc.tristimulus = TristimulusColor::fromRgbf(
                                mono.rgb().r(),
                                mono.rgb().g(),
                                mono.rgb().b()
                        );
                        break;
                case cosyscene::Color::Spectrum:
                        cpc.mode = ColorPickerColor::Spectral;
                        const std::vector<cosyscene::WavelengthAmplitudePair>
                                samples = mono.spectrum().samples();
                        for (size_t i=0; i<samples.size(); ++i) {
                                cpc.spectral.push_back(SpectralSample(
                                                samples[i].wavelength(),
                                                samples[i].amplitude()));
                        }
                        break;
                }
                ui->colorPicker->setColor(cpc);
                break;
        }
        }
}
