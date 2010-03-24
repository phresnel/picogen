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

namespace Ui {
    class RenderWindow;
}


class RenderWindow;
class RenderWindowImpl;


namespace redshift {
        class QImageRenderTarget;
        class RenderTargetLock;
        class ColorRenderTarget;
        class Scene;

        namespace scenefile {
                class Scene;
        }
}



class RenderWindow : public QDialog {
        Q_OBJECT
public:
        RenderWindow(redshift::shared_ptr<redshift::scenefile::Scene>,
                     QWidget* parent=0);
        ~RenderWindow();

protected:
        void changeEvent(QEvent *e);

private slots:
        void updateImage (QImage image, double percentage);

private:
        Ui::RenderWindow *ui;
        redshift::shared_ptr<RenderWindowImpl> impl;
        friend class RenderWindowImpl;

        redshift::shared_ptr<redshift::scenefile::Scene> scenefile;
};



/*class ProgressReporter {
public:
        virtual void report (shared_ptr<RenderTargetLock const> rlock,
                                int completed, int total) const = 0;
        virtual void reportDone () const = 0;

        typedef shared_ptr<ProgressReporter> Ptr;
        typedef shared_ptr<ProgressReporter const> ConstPtr;
};*/


class RenderWindowImpl
        : public QThread
        , public redshift::interaction::ProgressReporter
        , public redshift::enable_shared_from_this<RenderWindowImpl>
{
        Q_OBJECT
public:
        RenderWindowImpl (redshift::shared_ptr<redshift::scenefile::Scene>);
        virtual ~RenderWindowImpl ();

        // redshift::interaction::ProgressReporter
        void report (redshift::shared_ptr<redshift::RenderTargetLock const> rlock,
                     int completed, int total);
        void reportDone ();

        // QThread
        void run();

        bool error() const { return error_; }
        QString errorMessage() const { return errorMessage_; }

signals:
        // any percentage >= 1. is assumed to mean 100%
        void updateImage (QImage image, double percentage);

private:
        int reportW;
        redshift::shared_ptr<redshift::QImageRenderTarget> target;
        redshift::shared_ptr<redshift::ColorRenderTarget> renderBuffer;
        redshift::shared_ptr<redshift::scenefile::Scene> scenefile;

        bool error_;
        QString errorMessage_;
};

#endif // RENDERWINDOW_HH
