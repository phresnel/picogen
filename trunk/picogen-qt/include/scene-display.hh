//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
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

#ifndef SCENEDISPLAY_HH_INCLUDED_20090404
#define SCENEDISPLAY_HH_INCLUDED_20090404

#include "../../redshift/include/redshift.hh"

#include <QThread>
#include <QImage>
#include <QResizeEvent>

#include "ui_render-window.h"


class SceneDisplayThread;
class Reporter : public redshift::interaction::ProgressReporter {

public:
        // redshift::ProgressReporter:
        virtual void report (redshift::RenderTarget::ReadLockPtr,int,int)const;
        virtual void reportDone () const;
        
        Reporter (
                SceneDisplayThread &sceneDisplayThread,
                redshift::RenderTarget::Ptr renderBuffer,
                redshift::RenderTarget::Ptr screenBuffer
        ) ;


private:                
        mutable clock_t lastReportTime;
        redshift::RenderTarget::Ptr renderBuffer;
        redshift::RenderTarget::Ptr screenBuffer;
        SceneDisplayThread &sceneDisplayThread;
        
private:
        Reporter ();
        Reporter(Reporter const &);
        Reporter & operator = (Reporter const &);

};






class SceneDisplayThread
: public QThread
{

        Q_OBJECT

signals:        
        void partialImage (const QImage &image);
        void fullImage (const QImage &image);
        
private:
        friend class Reporter;
        void reportPartialImage ();
        void reportFullImage ();
        
private:
        friend class SceneDisplayImpl;
        int widthForNextRender, heightForNextRender;        

protected:
        // QThread:
        void run();
        
        
private:
        redshift::interaction::ProgressReporter::Ptr reporter;
        redshift::RenderTarget::Ptr renderBuffer;
        redshift::RenderTarget::Ptr screenBuffer;

};






// when we derive from QAbstractScrollArea, the content is displayed
class SceneDisplayImpl : public QWidget, private Ui::RenderWindow
{
        Q_OBJECT

public:
        SceneDisplayImpl(/*QWidget* parent=0*/);
        virtual ~SceneDisplayImpl();
        
protected:
        void resizeEvent(QResizeEvent *event);

private slots:

        void updateDisplay (QImage const &image);
        void fullImage ();
 
private:

        SceneDisplayThread renderThread;
        bool mustReRender;

};


#endif // SCENEDISPLAY_HH_INCLUDED_20090404
