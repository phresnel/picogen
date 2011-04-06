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

#ifndef FRAGMENT_HH
#define FRAGMENT_HH

#include <QStringList>
#include "collection.hh"

namespace picogen_repository {

class Fragment {
public:
        Fragment(Collection package, QString path);
        Fragment();

        QString title() const;
        void setTitle(QString);

        QString author() const;
        void setAuthor(QString);

        QString email() const;
        void setEmail(QString);

        QString homepage() const;
        void setHomepage(QString);

        Collection package() const;
        QString path() const;

        QString name() const;
        //void setName(QString);

        QStringList previewFilenames() const;

        virtual void save() const;
        virtual void reload();

protected:
        QString read (QString filename, quint64 maxlen=1024) const;
        void write (QString value, QString filename, quint64 maxlen=1024) const;

private:
        Collection package_;
        QString path_, name_;
        QString title_, author_, email_, homepage_;
        QStringList previewFilenames_;
};


} // namespace picogen_repository {

#endif // FRAGMENT_HH
