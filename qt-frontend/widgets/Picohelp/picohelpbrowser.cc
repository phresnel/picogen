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

#include <QWebPage>
#include <QWebFrame>

#include "picohelpbrowser.hh"
#include "ui_picohelpbrowser.h"

PicohelpBrowser::PicohelpBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PicohelpBrowser)
{
        ui->setupUi(this);
        ui->webView->page()->setLinkDelegationPolicy(
                        QWebPage::DelegateExternalLinks);
        ui->webView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
        ui->webView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);

        //ui->webView->page()->mainFrame()->setScrollBarPolicy();
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
void PicohelpBrowser::resizeEvent(QResizeEvent *) {
        recalcScrollbars();
}



void PicohelpBrowser::gotoArticle (QString const &filename) {
        ui->webView->load("help-content/" + filename);
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



void PicohelpBrowser::on_verticalScrollBar_valueChanged(int value) {
        QWebFrame &frame = *ui->webView->page()->mainFrame();
        QScrollBar &bar = *ui->verticalScrollBar;
        frame.setScrollPosition(QPoint(0,value));
}
void PicohelpBrowser::on_horizontalScrollBar_valueChanged(int value) {
        QWebFrame &frame = *ui->webView->page()->mainFrame();
        QScrollBar &bar = *ui->verticalScrollBar;
        frame.setScrollPosition(QPoint(value,0));
}



void PicohelpBrowser::on_webView_loadFinished(bool okay) {
        if (!okay)
                return;
        recalcScrollbars();
}



void PicohelpBrowser::recalcScrollbars() {
        QWebFrame &frame = *ui->webView->page()->mainFrame();
        QScrollBar &verticalBar = *ui->verticalScrollBar;
        QScrollBar &horizontalBar = *ui->horizontalScrollBar;

        const int docHeight = frame.contentsSize().height();
        const int browserHeight = ui->webView->height();
        const int pageVerticalStep = docHeight * ((double)browserHeight) / docHeight;

        if (docHeight <= browserHeight) {
                verticalBar.setVisible(false);
        } else {
                verticalBar.setVisible(true);

                verticalBar.setVisible(true);
                verticalBar.setPageStep(pageVerticalStep);

                verticalBar.setMinimum(0);
                verticalBar.setMaximum(docHeight - pageVerticalStep);
        }



        const int docWidth = frame.contentsSize().width();
        const int browserWidth = ui->webView->width();
        const int pageHorizontalStep = docWidth * ((double)browserWidth) / docWidth;

        if (docWidth <= browserWidth) {
                horizontalBar.setVisible(false);
        } else {
                horizontalBar.setVisible(true);

                horizontalBar.setVisible(true);
                horizontalBar.setPageStep(pageHorizontalStep);

                horizontalBar.setMinimum(0);
                horizontalBar.setMaximum(docWidth - pageHorizontalStep);
        }
}
