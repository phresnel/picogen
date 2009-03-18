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

#ifndef USERCOMMANDPROCESSOR_HH_INCLUDED_20090318
#define USERCOMMANDPROCESSOR_HH_INCLUDED_20090318

namespace redshift { namespace interaction {
        class UserCommandProcessor {
        public:
                virtual void tick () = 0;
                virtual bool userWantsToQuit () const = 0;
                virtual void waitForQuit () = 0;
                virtual ~UserCommandProcessor () {}
                
                typedef shared_ptr<UserCommandProcessor> Ptr;
                typedef shared_ptr<UserCommandProcessor const> ConstPtr;
        };
} }

#endif // USERCOMMANDPROCESSOR_HH_INCLUDED_20090318
