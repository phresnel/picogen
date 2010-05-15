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



#include "picohelpbrowser.hh"
#include "ui_picohelpbrowser.h"

PicohelpBrowser::PicohelpBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PicohelpBrowser)
{
        ui->setupUi(this);
        ui->webView->page()->setLinkDelegationPolicy(
                        QWebPage::DelegateExternalLinks);
}



PicohelpBrowser::~PicohelpBrowser() {
        delete ui;
}



void PicohelpBrowser::changeEvent(QEvent *e) {
        QWidget::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
        default:
                break;
        }
}



void PicohelpBrowser::gotoArticle (QString const &filename) {
        ui->webView->setUrl("help-content/" + filename);
}



void PicohelpBrowser::on_gotoMainPageButton_clicked() {
        gotoArticle("Main_Page.html");
}
void PicohelpBrowser::on_gotoTutorialsButton_clicked() {
        gotoArticle("Tutorials.html");
}
void PicohelpBrowser::on_gotoReferencesButton_clicked() {
        gotoArticle("References.html");
}



void PicohelpBrowser::on_backButton_clicked() {
        ui->webView->back();
}
void PicohelpBrowser::on_forwardButton_clicked() {
        ui->webView->forward();
}



void PicohelpBrowser::on_smallerButton_clicked() {
        ui->webView->setZoomFactor(ui->webView->zoomFactor()/1.125);
}
void PicohelpBrowser::on_biggerButton_clicked() {
        ui->webView->setZoomFactor(ui->webView->zoomFactor()*1.125);
}
void PicohelpBrowser::on_resetScalingButton_clicked() {
        ui->webView->setZoomFactor(1);
}
