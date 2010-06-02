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

#include <QEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QFile>

#include <QUrl>
#include <QMessageBox>

#include "picohelpbrowser.hh"
#include "ui_picohelpbrowser.h"

#include <cmath>

namespace {
        struct Index {
                QStringList searchTerms;
                QString url;

                Index (QStringList terms, QString url)
                        : url(url)
                {
                        foreach (QString s, terms)
                                searchTerms = searchTerms << s.toLower();
                }

                Index () {}

                Index &operator = (Index const &rhs) {
                        searchTerms = rhs.searchTerms;
                        url = rhs.url;
                        return *this;
                }

                QString findUrl (QString searchTerm) const {
                        for (int i=0; i<searchTerms.size(); ++i) {
                                if (searchTerms[i] == searchTerm)
                                        return url;
                        }
                        return "";
                }
        };

        class Indices {
                QVector<Index> indices;
        public:
                void push_back (Index const &index) {
                        indices.push_back(index);
                }

                QString findUrl (QString searchTerm) const {
                        searchTerm = searchTerm.toLower();
                        foreach (Index i, indices) {
                                const QString t = i.findUrl (searchTerm);
                                if ("" != t)
                                        return t;
                        }
                        return "";
                }
        };
        //-----------------------------------------


        Indices createQuatschIndices () {
                Indices ret;

                ret.push_back(Index(QStringList()
                                    <<"+"<<"-"<<"*"<<"/"<<"^"
                                    <<"sqrt"<<"min"<<"max"<<"/"<<"lerp"
                                    <<"log"<<"log10"<<"exp"
                             ,"Quatsch-Jux_Reference.html#Basics"));

                ret.push_back(Index(QStringList()
                                    <<"<"<<"<="<<">"<<">="
                                    <<"="<<"!="
                             ,"Quatsch-Jux_Reference.html#Predicates_.28Comparison.29"));

                ret.push_back(Index(QStringList()
                                    <<"[]"<<"[["<<"]["<<"]]"
                             ,"Quatsch-Jux_Reference.html#Range-Predicates"));

                ret.push_back(Index(QStringList()
                                    <<"and"<<"or"<<"xor"<<"not"
                             ,"Quatsch-Jux_Reference.html#Predicate_Combiners.2FMutators"));

                ret.push_back(Index(QStringList()
                                    <<"sin"<<"cos"
                             ,"Quatsch-Jux_Reference.html#Trigonometric_Functions"));

                ret.push_back(Index(QStringList()
                                    <<"inv"<<"floor"<<"trunc"
                                    <<"abs"<<"frac"<<"neg"
                             ,"Quatsch-Jux_Reference.html#Other_Math_Functions"));

                ret.push_back(Index(QStringList()
                                    <<"if"
                                    <<"defun"
                             ,"Quatsch-Jux_Reference.html#Control_Flow"));

                ret.push_back(Index(QStringList()<<"Libnoise", "Quatsch-Jux_Reference_-_Configurable_Functions.html#Libnoise_Support"));
                ret.push_back(Index(QStringList()<<"LibnoisePerlin", "Quatsch-Jux_Reference_-_Configurable_Functions.html#Perlin"));
                ret.push_back(Index(QStringList()<<"LibnoiseBillow", "Quatsch-Jux_Reference_-_Configurable_Functions.html#Billow"));
                ret.push_back(Index(QStringList()<<"LibnoiseRidgedMulti", "Quatsch-Jux_Reference_-_Configurable_Functions.html#Ridged_Multifractal_Perlin_Noise"));
                ret.push_back(Index(QStringList()<<"LibnoiseVoronoi", "Quatsch-Jux_Reference_-_Configurable_Functions.html#Voronoi"));
                ret.push_back(Index(QStringList()<<"LibnoiseCylinders", "Quatsch-Jux_Reference_-_Configurable_Functions.html#Cylinders"));
                ret.push_back(Index(QStringList()<<"LibnoiseSpheres", "Quatsch-Jux_Reference_-_Configurable_Functions.html#Spheres"));
                ret.push_back(Index(QStringList()<<"Heightmap", "Quatsch-Jux_Reference_-_Configurable_Functions.html#Heightmap"));

                return ret;
        }

        const Indices quatschIndices = createQuatschIndices();
}




PicohelpBrowser::PicohelpBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PicohelpBrowser),
    helpRoot("file:///usr/share/picogen/help-content/")
{
        ui->setupUi(this);
        ui->webView->page()->setLinkDelegationPolicy(
                        QWebPage::DelegateAllLinks);
        ui->webView->page()->mainFrame()->setScrollBarPolicy(
                        Qt::Horizontal, Qt::ScrollBarAlwaysOff);
        ui->webView->page()->mainFrame()->setScrollBarPolicy(
                        Qt::Vertical, Qt::ScrollBarAlwaysOff);

        ui->webView->installEventFilter(this);

#ifdef _WIN32
        setHelpRoot("file:///" + QApplication::applicationDirPath() + "/help-content/");
#elif defined(linux)
        if (QFile::exists(QApplication::applicationDirPath() + "/help-content/")) {
                setHelpRoot("file:///" + QApplication::applicationDirPath() + "/help-content/");
        } else if (QFile::exists("/usr/share/picogen/help-content/"))  {
                setHelpRoot("file:///usr/share/picogen/help-content/");
        } else {
                ui->webView->setHtml("No help-content found.");
        }
#else
#error
#endif
}



PicohelpBrowser::~PicohelpBrowser() {
        delete ui;
}



void PicohelpBrowser::setHelpRoot (QString str) {
        helpRoot = str;
        gotoArticle("Main_Page.html");
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
void PicohelpBrowser::keyPressEvent(QKeyEvent *k) {
        if (k->key() == Qt::Key_PageUp) {
                ui->verticalScrollBar->setValue(
                  ui->verticalScrollBar->value()
                  - ui->verticalScrollBar->pageStep());
                k->accept();
        } else if (k->key() == Qt::Key_PageDown) {
                ui->verticalScrollBar->setValue(
                  ui->verticalScrollBar->value()
                  + ui->verticalScrollBar->pageStep());
                k->accept();
        } else if (k->modifiers().testFlag(Qt::ControlModifier)) {
                if (k->key() == Qt::Key_Plus) {
                        on_biggerButton_clicked();
                } else if (k->key() == Qt::Key_Minus) {
                        on_smallerButton_clicked();
                } else if (k->key() == Qt::Key_0) {
                        on_resetScalingButton_clicked();
                } else if (k->key() == Qt::Key_Home) {
                        ui->verticalScrollBar->setValue(0);
                } else if (k->key() == Qt::Key_End) {
                        ui->verticalScrollBar->setValue(
                                ui->verticalScrollBar->maximum());
                }
        } else if (k->key() == Qt::Key_F1) {
                gotoArticle ("Picohelp.html");
        }
}
bool PicohelpBrowser::eventFilter(QObject */*o*/, QEvent *e) {
        if (e->type() == QEvent::Wheel) {
                QWheelEvent *w = (QWheelEvent *)e;
                if (w->modifiers().testFlag(Qt::ControlModifier)) {
                        ui->webView->setZoomFactor(
                          ui->webView->zoomFactor() *
                          (1 + 0.001*w->delta())
                        );
                        recalcScrollbars();
                } else {
                        if (w->orientation() == Qt::Vertical) {
                                ui->verticalScrollBar->setValue(
                                  ui->verticalScrollBar->value()
                                - w->delta());
                        } else {
                                ui->horizontalScrollBar->setValue(
                                  ui->horizontalScrollBar->value()
                                - w->delta());
                        }
                }
                return true;
        } else if (e->type() == QEvent::KeyPress) {
                QKeyEvent *k = (QKeyEvent *)e;
                if (k->key() == Qt::Key_PageUp
                    || k->key() == Qt::Key_PageDown
                    || (k->modifiers().testFlag(Qt::ControlModifier) &&
                        (k->key() == Qt::Key_Plus ||
                         k->key() == Qt::Key_Minus||
                         k->key() == Qt::Key_0    ||
                         k->key() == Qt::Key_End  ||
                         k->key() == Qt::Key_Home))
                ) {
                        keyPressEvent(k);
                        return true;
                } else if (k->key() == Qt::Key_Up) {
                        ui->verticalScrollBar->setValue(
                          ui->verticalScrollBar->value()
                          - ui->verticalScrollBar->singleStep());
                } else if (k->key() == Qt::Key_Down) {
                        ui->verticalScrollBar->setValue(
                          ui->verticalScrollBar->value()
                          + ui->verticalScrollBar->singleStep());
                } else if (k->key() == Qt::Key_Left) {
                        ui->horizontalScrollBar->setValue(
                          ui->horizontalScrollBar->value()
                          - ui->horizontalScrollBar->singleStep());
                } else if (k->key() == Qt::Key_Right) {
                        ui->horizontalScrollBar->setValue(
                          ui->horizontalScrollBar->value()
                          + ui->horizontalScrollBar->singleStep());
                }
                return false;
        }
        return false;
}
void PicohelpBrowser::on_webView_urlChanged (QUrl const & /*url*/) {
        ui->verticalScrollBar->setValue(
          ui->webView->page()->mainFrame()->scrollPosition().y()
        );
        ui->horizontalScrollBar->setValue(
          ui->webView->page()->mainFrame()->scrollPosition().x()
        );
}
void PicohelpBrowser::on_webView_loadFinished(bool okay) {
        if (!okay)
                return;
        recalcScrollbars();
        on_webView_urlChanged (ui->webView->url()); // dirty
}
void PicohelpBrowser::on_webView_linkClicked (const QUrl &url) {
        const QString local = url.toLocalFile();
        if (!QFile::exists(local))
                return;
        if (local.endsWith(".picogen")) {
                emit sceneFileClicked(local);
        } else {
                ui->webView->load(url);
        }
}


void PicohelpBrowser::gotoArticle (QString const &filename) {
        ui->webView->load(helpRoot + filename);
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



void PicohelpBrowser::gotoQuatsch(QuatschHelp q) {
        switch (q) {
        case QuatschReferencesOverview:
                gotoArticle("References.html"); break;
        case QuatschInterestingPrograms:
                gotoArticle("Interesting_quatsch_programs_-_dumps.html"); break;
        }
}
bool PicohelpBrowser::gotoQuatsch(QString searchTerm) {
        const QString url = quatschIndices.findUrl(searchTerm);
        if ("" == url)
                return false;
        gotoArticle(url);
        return true;
}
bool PicohelpBrowser::gotoSimplexGuiPropertyEdit(QString searchTerm) {
        if ("" == searchTerm) {
                gotoArticle("SimplexGUI_Property_Editor.html");
                return true;
        }
        return false;
}
bool PicohelpBrowser::gotoSimplexGuiPropertyEdit (
        QString a, QString b, QString c, QString d
) {
        QString terms[4] = {a, b, c, d};
        for (int i=0; i<4; ++i){
                const QString base = "SimplexGUI_Property_Editor.html#";
                if (terms[i] == "film-settings") {
                        gotoArticle(base+"film-settings");
                        return true;
                }
                if (terms[i] == "render-settings") {
                        gotoArticle(base+"render-settings");
                        return true;
                }
                if (terms[i] == "surface-integrator") {
                        gotoArticle(base+"surface-integrator");
                        return true;
                }
                if (terms[i] == "volume-integrator") {
                        gotoArticle(base+"volume-integrator");
                        return true;
                }
                if (terms[i] == "cameras") {
                        gotoArticle(base+"cameras");
                        return true;
                }
                if (terms[i] == "transform") {
                        gotoArticle(base+"Camera-Transforms");
                        return true;
                }
                if (terms[i] == "objects") {
                        gotoArticle(base+"objects");
                        return true;
                }
                if (terms[i] == "volumes") {
                        gotoArticle(base+"volumes");
                        return true;
                }
                if (terms[i] == "backgrounds") {
                        gotoArticle(base+"backgrounds");
                        return true;
                }
        }
        return false;
}



void PicohelpBrowser::on_backButton_clicked() {
        ui->webView->back();
}
void PicohelpBrowser::on_forwardButton_clicked() {
        ui->webView->forward();
}



void PicohelpBrowser::on_smallerButton_clicked() {
        ui->webView->setZoomFactor(ui->webView->zoomFactor()/1.125);
        recalcScrollbars();
}
void PicohelpBrowser::on_biggerButton_clicked() {
        ui->webView->setZoomFactor(ui->webView->zoomFactor()*1.125);
        recalcScrollbars();
}
void PicohelpBrowser::on_resetScalingButton_clicked() {
        ui->webView->setZoomFactor(1);
        recalcScrollbars();
}



void PicohelpBrowser::on_verticalScrollBar_valueChanged(int value) {
        QWebFrame &frame = *ui->webView->page()->mainFrame();
        frame.setScrollPosition(QPoint(frame.scrollPosition().x(),value));
}
void PicohelpBrowser::on_horizontalScrollBar_valueChanged(int value) {
        QWebFrame &frame = *ui->webView->page()->mainFrame();
        frame.setScrollPosition(QPoint(value,frame.scrollPosition().y()));
}



void PicohelpBrowser::recalcScrollbars() {
        QWebFrame &frame = *ui->webView->page()->mainFrame();
        QScrollBar &verticalBar = *ui->verticalScrollBar;
        QScrollBar &horizontalBar = *ui->horizontalScrollBar;

        const int docHeight = frame.contentsSize().height();
        const int browserHeight = ui->webView->height();
        const int pageVerticalStep = browserHeight;

        if (docHeight <= browserHeight) {
                verticalBar.setVisible(false);
        } else {
                verticalBar.setVisible(true);

                verticalBar.setVisible(true);
                verticalBar.setPageStep(pageVerticalStep);
                verticalBar.setSingleStep(16);

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
                horizontalBar.setSingleStep(16);

                horizontalBar.setMinimum(0);
                horizontalBar.setMaximum(docWidth - pageHorizontalStep);
        }
}
