#include "heightmapimage.h"
#include "ui_heightmapimage.h"

#include "quatsch.h"
#include "phase5/callable/to_callable.h"

#include <QPaintEvent>
#include <QPainter>
#include <QMessageBox>
#include <sstream>

HeightmapImage::HeightmapImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeightmapImage)
{
    ui->setupUi(this);
}

HeightmapImage::~HeightmapImage()
{
    delete ui;
}

void HeightmapImage::paintEvent (QPaintEvent *e)
{
        QPainter painter(this);

        const std::string code = "t";
        try {
                const quatsch::TemplatePtrList templates;
                const quatsch::ProgramContract ptype (quatsch::Typename::Float, {{"x", quatsch::Typename::Float},
                                                                                 {"z", quatsch::Typename::Float}});

                quatsch::ErrorState errors;
                quatsch::quatsch_function fun = quatsch::compile (
                                code, templates, ptype, errors,
                                quatsch::compiler::phase5::to_callable);

                if (errors) {
                        std::stringstream ss;
                        print_errors (errors, ss);
                        painter.drawText (0,0, QString::fromStdString(ss.str()));
                }

        } catch (std::exception &e) {
                QMessageBox::warning (this, "error", QString::fromStdString (e.what()));
        }
        /*QImage image(128, 128, Format::);
        image.setPixel (
        QPainter painter (this);
        painter.drawImage (0, 0, image);*/
}
