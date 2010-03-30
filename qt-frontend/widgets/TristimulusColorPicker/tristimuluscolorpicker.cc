#include "tristimuluscolorpicker.hh"
#include "ui_tristimuluscolorpicker.h"

TristimulusColorPicker::TristimulusColorPicker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TristimulusColorPicker),
    isInside_updateAllByRgb(false),
    isInside_updateAllByHsv(false)
{
        ui->setupUi(this);
}

TristimulusColorPicker::~TristimulusColorPicker() {
        delete ui;
}

void TristimulusColorPicker::changeEvent(QEvent *e) {
        QWidget::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
        default:
                break;
        }
}

void TristimulusColorPicker::on_triangle_colorChanged(const QColor & color) {
        updateAllSpins(color);
}

void TristimulusColorPicker::on_spinR_valueChanged(int) {
        updateAllByRgb();
}
void TristimulusColorPicker::on_spinG_valueChanged(int) {
        updateAllByRgb();
}
void TristimulusColorPicker::on_spinB_valueChanged(int) {
        updateAllByRgb();
}

void TristimulusColorPicker::on_spinH_valueChanged(int) {
        updateAllByHsv();
}
void TristimulusColorPicker::on_spinS_valueChanged(int) {
        updateAllByHsv();
}
void TristimulusColorPicker::on_spinV_valueChanged(int) {
        updateAllByHsv();
}



void TristimulusColorPicker::updateCmykSpins(QColor const &color) {
        ui->spinC->setValue(color.cyan());
        ui->spinM->setValue(color.magenta());
        ui->spinY->setValue(color.yellow());
        ui->spinK->setValue(color.black());
}
void TristimulusColorPicker::updateRgbSpins(QColor const &color) {
        ui->spinR->setValue(color.red());
        ui->spinG->setValue(color.green());
        ui->spinB->setValue(color.blue());
}
void TristimulusColorPicker::updateHsvSpins(QColor const &color) {
        ui->spinH->setValue(color.hsvHue());
        ui->spinS->setValue(color.hsvSaturation());
        ui->spinV->setValue(color.value());
}
void TristimulusColorPicker::updateAllSpins(QColor const &color) {
        updateRgbSpins(color);
        updateHsvSpins(color);
        updateCmykSpins(color);
}



void TristimulusColorPicker::updateAllByRgb() {
        if (isInside_updateAllByRgb)
                return;
        isInside_updateAllByRgb = true;
        const QColor newc = QColor::fromRgb(
                        ui->spinR->value(),
                        ui->spinG->value(),
                        ui->spinB->value());
        ui->triangle->setColor(newc);
        updateCmykSpins(newc);
        updateHsvSpins(newc);
        isInside_updateAllByRgb = false;
}

void TristimulusColorPicker::updateAllByHsv() {
        if (isInside_updateAllByHsv)
                return;
        isInside_updateAllByHsv = true;
        const QColor newc = QColor::fromHsv(ui->spinH->value(), ui->spinS->value(), ui->spinV->value());
        ui->triangle->setColor(newc);
        updateCmykSpins(newc);
        updateRgbSpins(newc);        
        isInside_updateAllByHsv = false;
}
