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



#ifndef RENDERWINDOW_HH
#define RENDERWINDOW_HH

#include <QDialog>
#include <QThread>

#include "../../redshift/include/smart_ptr.hh"
#include "../../redshift/include/interaction/progressreporter.hh"
#include "../../redshift/include/interaction/usercommandprocessor.hh"
#include "../../redshift/include/auxiliary/stopwatch.hh"
#include "../../redshift/include/auxiliary/computationtime.hh"

namespace Ui {
    class RenderWindow;
}


class RenderWindow;
class RenderWindowImpl;


namespace redshift {
        //class QImageRenderTarget;
        class RenderTargetLock;
        class ColorRenderTarget;
        class Scene;
        class Film;

        namespace scenefile {
                class Scene;
        }
}



class RenderWindow : public QDialog {
        Q_OBJECT
public:
        RenderWindow(redshift::shared_ptr<redshift::scenefile::Scene>,
                     int renderSettings, int camera,
                     QWidget* parent=0, double updateLatency=1.);
        ~RenderWindow();

        static void RenderProcess (QString pathToSource,
                              int renderSettings, int camera);

protected:
        void changeEvent(QEvent *e);
        void resizeEvent(QResizeEvent *);

private slots:
        void on_pauseButton_clicked(bool checked);
        void on_saveImageButton_clicked();
        //void updateImage (QImage const &image, double percentage);
        void updateImage (double percentage);

private:
        Ui::RenderWindow *ui;
        redshift::shared_ptr<RenderWindowImpl> impl;
        friend class RenderWindowImpl;

        redshift::shared_ptr<redshift::scenefile::Scene> scenefile;
        QImage image;
        double updateLatency;
        redshift::StopWatch realTime;
        redshift::ComputationTime compTime;
};



/*class ProgressReporter {
public:
        virtual void report (shared_ptr<RenderTargetLock const> rlock,
                                int completed, int total) const = 0;
        virtual void reportDone () const = 0;

        typedef shared_ptr<ProgressReporter> Ptr;
        typedef shared_ptr<ProgressReporter const> ConstPtr;
};*/


// Kind of a kitchen-sink of functionality.
// Sidenote: This *should* be hidden in a source-file, but then moc+Q_OBJECT
//           don't like that.
class RenderWindowImpl
        : public QThread
        , public redshift::interaction::ProgressReporter
        , public redshift::interaction::UserCommandProcessor
        , public redshift::enable_shared_from_this<RenderWindowImpl>
{
        Q_OBJECT
public:
        RenderWindowImpl (redshift::shared_ptr<redshift::scenefile::Scene>,
                          int renderSettings, int camera, double latency);
        virtual ~RenderWindowImpl ();

        void setUserWantsToQuit (bool) ;
        void setUserWantsToPause (bool) ;
        void saveQuit ();

        // ProgressReporter
        void report (int completed, int total);
        void reportDone ();


        // UserCommandProcessor
        void tick () ;
        bool userWantsToQuit () const ;
        bool userWantsToPause () const ;


        // QThread
        void run();

        bool error() const { return error_; }
        QString errorMessage() const { return errorMessage_; }

        redshift::shared_ptr<redshift::Film> film() const;

signals:
        // any percentage >= 1. is assumed to mean 100%
        void updateImage (double percentage);

private:

        void doCopy();
        int renderSettings, camera;
        //redshift::shared_ptr<redshift::QImageRenderTarget> target;
        redshift::shared_ptr<redshift::Film> renderBuffer;
        redshift::shared_ptr<redshift::scenefile::Scene> scenefile;

        bool error_;
        QString errorMessage_;
        volatile bool running;

        bool wantsToQuit;
        bool wantsToPause;

        redshift::StopWatch realTime;
        double updateLatency;
        bool firstReport;
};

#endif // RENDERWINDOW_HH
