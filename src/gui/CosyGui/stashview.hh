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

#ifndef STASHVIEW_HH
#define STASHVIEW_HH

#include <QDialog>
#include <QVector>
#include <ctime>
#include "redshift/include/smart_ptr.hh"
#include "cosyscene/stash.hh"
#include "stashframe.hh"

class QListWidgetItem;
namespace Ui {
        class StashView;
}

class StashView;

class StashViewItem {
public:
        template <typename T> StashViewItem (StashObject<T> const &stashObject)
                : time(stashObject.time())
                , data(new T (stashObject.value()))
        {}

        StashViewItem () {}

private:
        friend class StashView;
        std::time_t time;
        redshift::shared_ptr<void> data;
};

class StashView : public QDialog
{
        Q_OBJECT

public:
        explicit StashView(QWidget *parent = 0);
        ~StashView();

        template <typename T>
        void addItems (Stash<T> const &stash);

        template <typename T>
        void addItem (StashObject<T> const &stashObject);

        void addItem (const StashViewItem &item);

        template <typename T>
        T selectedData () const;

        bool hasSelectedData() const;

        int selectedIndex() const;

        template <typename T> Stash<T> itemsToStash () const;


        template <typename T>
        static bool StashDialog (QWidget *self, boost::shared_ptr<T> value)
        {
                Q_UNUSED(self)
                if (value->getStash().contains_data(*value)) {
                        switch (confirmRestash (self)) {
                        case ConfirmRestash_Abort:
                                return false;
                        case ConfirmRestash_RestashAndKeepOld:
                                break;
                        case ConfirmRestash_RestashAndKillOld:
                                value->getStash().kill_all(*value);
                                break;
                        }
                }
                value->stash();
                return true;
        }

        template <typename T>
        static bool RestoreDialog (
                QWidget *self, boost::shared_ptr<T> value
        ) {
                StashView *sw = new StashView (self);
                sw->addItems(value->getStash());
                if (QDialog::Accepted != sw->exec())
                        return false;

                T ret = T(sw->selectedData<T>());
                ret.setStash (sw->itemsToStash<T>());

                *value = ret;
                return true;
        }

        template <typename T>
        static bool ResetDialog(QWidget *self, boost::shared_ptr<T> value)
        {
                if (value->is_default())
                        return false;

                if (!value->getStash().contains_data(*value)) {
                        switch (confirmReset (self)) {
                        case ConfirmReset_Abort:
                                return false;
                        case ConfirmReset_StashBeforeReset:
                                value->stash();
                                break;
                        case ConfirmReset_Reset:
                                break;
                        }
                }
                T t;
                t.setStash(value->getStash());
                *value = t;
                return true;
        }

private slots:
        void on_cancelButton_clicked();
        void on_okayButton_clicked();
        void on_listWidget_itemDoubleClicked(QListWidgetItem* );

private:
        Ui::StashView *ui;
        QVector<StashViewItem> items;
};

template <typename T>
void StashView::addItem (StashObject<T> const &stashObject) {
        addItem(StashViewItem(stashObject));
}

template <typename T>
void StashView::addItems (Stash<T> const &stash) {
        typedef typename Stash<T>::const_iterator const_iterator;

        for (const_iterator it=stash.begin(); it!=stash.end(); ++it) {
                addItem(*it);
        }
}

template <typename T>
T StashView::selectedData () const {
        const int s = selectedIndex();
        return *((T*)items[s].data.get());
}

template <typename T>
Stash<T> StashView::itemsToStash () const {
        Stash<T> ret;
        for (int i=0; i<items.size(); ++i) {
                ret.push_back (StashObject<T>(
                        items[i].time,
                        *((T*)items[i].data.get())
                ));
        }
        return ret;
}

#endif // STASHVIEW_HH
