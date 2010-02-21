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

#ifndef CURRENTDATE_HH_INCLUDED_20100221
#define CURRENTDATE_HH_INCLUDED_20100221

#include <ctime>
#include <sstream>
#include <string>

class CurrentDate {
public:
        CurrentDate ()
        : tm (getTm())
        {}


        unsigned int year ()    const { return 1900 + tm.tm_year; }
        unsigned int month ()   const { return 1+tm.tm_mon; }
        unsigned int day ()     const { return tm.tm_mday; }
        unsigned int hour ()    const { return tm.tm_hour; }
        unsigned int minute ()  const { return tm.tm_min; }
        unsigned int seconds () const { return tm.tm_sec; }

        std::string asPartOfFilename () const {
                std::stringstream ss;
                ss << year()
                        << "" << (month()<10?"0":"") << month()
                        << "" << (day()<10?"0":"") << day()
                        << "-" << (hour()<10?"0":"") << hour()
                        << "" << (minute()<10?"0":"") << minute()
                        << "" << (seconds()<10?"0":"") << seconds()
                ;
                return ss.str();
        }

        static std::string AsPartOfFilename () {
                return CurrentDate().asPartOfFilename ();
        }
private:
        const std::tm tm;
        static std::tm getTm () {
                std::time_t t;
                std::tm *tm;
                time (&t);
                return *localtime (&t);
        }
};

inline std::string filename_extension (const std::string &str) {
        using std::string;
        string ret;
        for (string::const_reverse_iterator it=str.rbegin(); it!=str.rend(); ++it) {
                if ('.' == *it) {
                        reverse (ret.begin(), ret.end());
                        return ret;
                }
                ret += *it;
        }
        return "";
}

#endif // CURRENTDATE_HH_INCLUDED_20100221
