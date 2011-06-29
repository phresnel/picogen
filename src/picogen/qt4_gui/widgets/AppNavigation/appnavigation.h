//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2011  Sebastian Mach (*1983)
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

#ifndef APPNAVIGATION_H
#define APPNAVIGATION_H

#include <QFrame>
#include <QToolButton>
#include <QDebug>

namespace Ui {
    class AppNavigation;
}


class QString;
class QIcon;
class AppNavigation;

namespace picogen { namespace qt4_gui {

class AppNavigationButton : public QToolButton {
        Q_OBJECT
public:
        AppNavigationButton () : QToolButton()
        {
                connect(this, SIGNAL(clicked()),
                        SLOT(activate()));
        }
        virtual ~AppNavigationButton() {}

        Q_PROPERTY(bool active READ active WRITE setActive)

signals:
        void activated(AppNavigationButton *emitter);
private slots:
        void activate() {
                setActive(true);
                emit activated(this);
        }
private:
        bool active_;

        bool active () { return active_; }
        void setActive (bool active) {
                active_ = active;
                setStyle(style());
        }
        friend class AppNavigation;
};

class AppNavigation : public QFrame
{
    Q_OBJECT

public:
    explicit AppNavigation(QWidget *parent = 0);
    ~AppNavigation();

    AppNavigationButton* addButton (const QString &title, const QIcon &icon);

private slots:
    void activateButton (AppNavigationButton*);
private:
    void addToolButton (AppNavigationButton*);
private:
    Ui::AppNavigation *ui;
    QVector<AppNavigationButton*> buttons;
};

} }

#endif // APPNAVIGATION_H
