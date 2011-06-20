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



#ifndef PICOHELPBROWSER_HH
#define PICOHELPBROWSER_HH

#include <QWidget>

namespace Ui {
        class PicohelpBrowser;
}
class QUrl;

class PicohelpBrowser : public QWidget {
        Q_OBJECT
public:
        PicohelpBrowser(QWidget *parent = 0);
        ~PicohelpBrowser();

        enum QuatschHelp {
                QuatschReferencesOverview,
                QuatschInterestingPrograms
        };
        void gotoQuatsch(QuatschHelp);
        bool gotoQuatsch(QString searchTerm);
        bool gotoSimplexGuiPropertyEdit (QString searchTerm = "");
        bool gotoSimplexGuiPropertyEdit (QString a, QString b,
                                         QString c, QString d);

        void gotoArticle (QString const &filename);

        void setHelpRoot (QString str);

signals:
        void sceneFileClicked (QString const &);

protected:
        void changeEvent(QEvent *e);
        void resizeEvent(QResizeEvent *);
        void keyPressEvent(QKeyEvent *);
        bool eventFilter(QObject *o, QEvent *e);


private:
        Ui::PicohelpBrowser *ui;
        QString helpRoot;

        void recalcScrollbars ();
private slots:

        void on_webView_urlChanged (QUrl const &);
        void on_webView_loadFinished (bool);
        void on_webView_linkClicked (const QUrl &);

        void on_verticalScrollBar_valueChanged(int value);
        void on_horizontalScrollBar_valueChanged(int value);
        void on_resetScalingButton_clicked();
        void on_biggerButton_clicked();
        void on_smallerButton_clicked();
        void on_forwardButton_clicked();
        void on_backButton_clicked();
        void on_gotoReferencesButton_clicked();
        void on_gotoTutorialsButton_clicked();
        void on_gotoMainPageButton_clicked();
};

#endif // PICOHELPBROWSER_HH
