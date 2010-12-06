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

#include <limits>
#include "redshift/include/basictypes/quatsch-height-function.hh"
#include "quatschpreview.hh"
#include "ui_quatschpreview.h"

QuatschPreview::QuatschPreview(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::QuatschPreview)
{
        ui->setupUi(this);
        ui->status->setText("");
}

QuatschPreview::~QuatschPreview() {
        delete ui;
}

void QuatschPreview::compileAndRun() {
        std::stringstream errors;

        try {
                ui->status->setText("");
                redshift::QuatschHeightFunction q (code, errors);
                const unsigned int width = ui->imageSize->value(), height = width;
                QImage image (width, height, QImage::Format_RGB888);


                const float centerX = ui->centerX->value();
                const float centerY = ui->centerY->value();
                const float edgeSize = ui->edgeSize->value();

                float min = std::numeric_limits<float>::infinity(),
                      max = -std::numeric_limits<float>::infinity();
                std::vector<float> values(width*height);
                for (unsigned int y=0; y<height; ++y) {
                        const float v_ = (y/(float)height)-0.5f,
                                    v = (v_*edgeSize) + centerY;

                        for (unsigned int x=0; x<width; ++x) {
                                const float u_ = (x/(float)width)-0.5f,
                                            u = (u_*edgeSize) + centerX;
                                const float h = q(u,v);
                                if (h<min) min=h;
                                if (h>max) max=h;
                                values[x+y*width] = h;
                        }
                }
                const float range = 1 / (max - min);
                for (unsigned int y=0; y<height; ++y) {
                        for (unsigned int x=0; x<width; ++x) {
                                const float h = (values[x+y*width]-min) * range;
                                const int hi_ = h * 255,
                                          hi = hi_<0?0:hi_>255?255:hi_;
                                image.setPixel(x, y, QColor(hi,hi,hi).rgb());
                        }
                }
                //ui->status->setScaledContents(true);
                ui->status->setWordWrap(false);
                ui->status->setPixmap(QPixmap::fromImage(image));

        } catch (quatsch::general_exception const &ex) {
                ui->status->setWordWrap(true);
                ui->status->setText (QString::fromStdString(
                        ex.getMessage() + ".\n\n"
                        + errors.str()));
        } catch (std::exception const &e) {
                ui->status->setWordWrap(true);
                ui->status->setText(e.what());
        } catch (...) {
                ui->status->setWordWrap(true);
                ui->status->setText("some weird exception occured");
        }
}

void QuatschPreview::setCode (std::string const &str) {
        code = str;
}

void QuatschPreview::setCode (QString str) {
        setCode (str.toStdString());
}

void QuatschPreview::compileAndRun(std::string const &str) {
        setCode (str);
        compileAndRun();
}

void QuatschPreview::compileAndRun(QString str) {
        setCode (str);
        compileAndRun();
}
