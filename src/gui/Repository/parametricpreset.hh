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

#ifndef PARAMETRICPRESET_HH
#define PARAMETRICPRESET_HH

#include <QString>
#include "package.hh"

class Entity {
public:
        Entity(Package package, QString path);
        Entity();

        QString title() const;
        void setTitle(QString);

        QString author() const;
        void setAuthor(QString);

        QString email() const;
        void setEmail(QString);

        QString homepage() const;
        void setHomepage(QString);

        Package package() const;
        QString path() const;

        QString name() const;
        //void setName(QString);

        virtual void save() const;
        virtual void reload();

protected:
        QString readAll (QString filename, quint64 maxlen=1024) const;
        void writeAll (QString value, QString filename, quint64 maxlen=1024) const;

private:
        Package package_;
        QString path_, name_;
        QString title_, author_, email_, homepage_;
};



class ParametricPreset : public Entity
{
public:
        ParametricPreset(Package package, QString path);
        ParametricPreset();

        QString preset() const;
        void setPreset(QString);

        virtual void save() const;
        virtual void reload();
private:
        QString preset_;
};

#endif // PARAMETRICPRESET_HH
