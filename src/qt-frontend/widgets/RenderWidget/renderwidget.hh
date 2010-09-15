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

#ifndef RENDERWIDGET_HH
#define RENDERWIDGET_HH

#include <QWidget>
#include <QThread>
#include "redshift/include/smart_ptr.hh"
#include "redshift/include/auxiliary/mutex.hh"
#include "../../redshift/include/auxiliary/stopwatch.hh"
#include "../../redshift/include/auxiliary/computationtime.hh"

namespace Ui {
        class RenderWidget;
}

namespace redshift {
        namespace scenefile {
                class Scene;
                class FilmSettings;
        }
        class Film;
}


class RenderWidgetThread;
class RenderWidgetProgressReporter;
class RenderWidgetUserCommandProcessor;


class RenderWidget : public QWidget
{
        Q_OBJECT

public:
        explicit RenderWidget(QWidget *parent = 0);
        ~RenderWidget();


        void setSceneAndRender (
                redshift::shared_ptr<const redshift::scenefile::Scene> scene,
                bool overrideFilmSizeWithWidgetSize = false
        ) {
                setSceneAndRender (scene, 0, 0, overrideFilmSizeWithWidgetSize);
        }
        void setSceneAndRender (
                redshift::shared_ptr<const redshift::scenefile::Scene>,
                int renderSettingsIndex, int cameraIndex,
                bool overrideFilmSizeWithWidgetSize = false);

        bool error() const { return error_; }
        QString errorMessage() const { return errorMessage_; }

        // Sync.
        void updateImage (const redshift::Film &,
                          const redshift::scenefile::FilmSettings &);

private:
        // Ui.
        Ui::RenderWidget *ui;

        // Sync.
        redshift::Mutex mutex;
        RenderWidgetThread *renderThread;

        // Error reporting.
        bool error_;
        QString errorMessage_;
};



class RenderWidgetProgressReporter;


class RenderWidgetThread : public QThread {
public:
        RenderWidgetThread (RenderWidget *,
                            redshift::shared_ptr<const redshift::scenefile::Scene>,
                            int renderSettingsIndex, int cameraIndex);
        virtual ~RenderWidgetThread();
        void run();
private:

        friend class RenderWidgetProgressReporter;

        void renderImage (redshift::shared_ptr<redshift::Film>,
                          const redshift::scenefile::Scene &,
                          int renderSettingsIndex, int cameraIndex);
        void report (int completed, int total);

        // Redshift.
        RenderWidget *renderWidget_;
        redshift::shared_ptr<const redshift::scenefile::Scene> scenefile_;
        redshift::shared_ptr<redshift::Film> renderBuffer_;
        int renderSettingsIndex_;
        int cameraIndex_;

        // Error reporting.
        bool error_;
        QString errorMessage_;

        // Time.
        redshift::StopWatch updateWatch_;
        bool firstReport_;

        // Sync.
        double updateLatency_;

        redshift::shared_ptr<RenderWidgetProgressReporter> reporter;
        redshift::shared_ptr<RenderWidgetUserCommandProcessor> commandProcessor;
};









#include "redshift/include/interaction/progressreporter.hh"
class RenderWidgetProgressReporter
: public redshift::interaction::ProgressReporter
{
public:
        RenderWidgetProgressReporter(RenderWidgetThread *rw)
        : done_(false), renderThread_(rw) {}

        virtual void report (int completed, int total) {
                renderThread_->report (completed, total);
        }

        virtual void reportDone () {
                done_ = true;
        }

        bool done() const { return done_; }
private:
        bool done_;
        RenderWidgetThread *renderThread_;
};

#include "redshift/include/interaction/usercommandprocessor.hh"
class RenderWidgetUserCommandProcessor
: public redshift::interaction::UserCommandProcessor
{
public:
        RenderWidgetUserCommandProcessor() : quit_(false) {}

        virtual void tick () {}
        virtual bool userWantsToQuit () const { return quit_; }
        virtual bool userWantsToPause () const { return false; }

        void setQuit(bool quit) { quit_ = quit; }
private:
        bool quit_;
};




#endif // RENDERWIDGET_HH
