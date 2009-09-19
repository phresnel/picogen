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


#ifndef ENUM_HH_INCLUDED_20090828
#define ENUM_HH_INCLUDED_20090828

#include <map>
#include <vector>
#include <set>
#include <stdexcept>
#include <typeinfo>
#include <sstream>
#include <iostream>
namespace actuarius {

        
//-----------------------------------------------------------------------------
// Name value pair
//-----------------------------------------------------------------------------
template <typename T>
class Nvp {
public:
        Nvp (const char *name_, T value_)
        : name_ (name_), value_ (value_)
        {}
                
        Nvp (T value_, const char *name_)
        : name_ (name_), value_ (value_)
        {}
                
        Nvp (Nvp const &rhs)
        : name_ (rhs.name_), value_ (rhs.value_)
        {}
                
        Nvp& operator = (Nvp const &rhs) {
                name_ = rhs.name_;
                value_ = rhs.value_;
        }
        
        std::string name() const {
                return name_;
        }
        
        T value() const {
                return value_;
        }
        
private:
        Nvp ();        

        std::string name_;
        T value_;
};



//-----------------------------------------------------------------------------
// Name value pair - list construction helpers
//-----------------------------------------------------------------------------
template <typename T>
std::vector<Nvp<T> > operator | (
        Nvp<T> const & lhs, 
        Nvp<T> const & rhs
) {
        std::vector<Nvp<T> > ret;
        ret.push_back (lhs);
        ret.push_back (rhs);
        return ret;
}



template <typename T>
std::vector<Nvp<T> > operator | (
        std::vector<Nvp<T> > const & lhs, 
        Nvp<T> const & rhs
) {
        std::vector<Nvp<T> > ret (lhs);
        ret.push_back (rhs);
        return ret;
}



//-----------------------------------------------------------------------------
// Enum
//-----------------------------------------------------------------------------
template <typename T>
class Enum {
public:
        Enum () {
        }
        
        Enum (std::vector<Nvp<T> > const & list) {
                std::set<T> values;
                typedef std::vector<Nvp<T> > V;
                for (typename V::const_iterator it = list.begin();
                     it != list.end();
                     ++it
                ) {                        
                        if (values.count(it->value()) > 0) {
                                throw std::invalid_argument (
                                        std::string ("actuarius::Enum requires"
                                                     " each enum-value to be"
                                                     " unique (enum-typeid = ")
                                        + typeid (T).name()
                                        + ")"
                                );
                        }
                        values.insert (it->value());
                        
                        name2value [it->name()] = it->value();
                        value2name [it->value()] = it->name();
                }
        }
        
        Enum (Nvp<T> const & rhs) {
                name2value [rhs.name] = rhs.value();
        }
        
        Enum (Enum const &rhs) 
        : name2value (rhs.name2value)
        , value2name (rhs.value2name)
        {}
                
        Enum & operator = (Enum const &rhs) {
                name2value = rhs.name2value;
                value2name = rhs.value2name;
                return *this;
        }
        
        T operator [] (const char* name) const {
                if (!exists (name)) {
                        std::stringstream ss;
                        ss << "enum-name of " << name
                           << " not found. "
                           << "(enum-typeid = "
                           << typeid (T).name()
                           << "). "
                           << "did you forget to pass it to actuarius::Enum()?"
                        ;
                        throw std::out_of_range (ss.str());
                }
                return name2value.find (name)->second;
        }
        
        std::string operator [] (T value) const {
                if (!exists (value)) {                        
                        std::stringstream ss;
                        ss << "enum-value of " << value
                           << " not found. "
                           << "(enum-typeid = "
                           << typeid (T).name()
                           << "). "
                           << "did you forget to pass it to actuarius::Enum()?"
                        ;
                        throw std::out_of_range (ss.str());
                }
                return value2name.find (value)->second;
        }
        
        bool exists (std::string name) const {
                return name2value.find(name) != name2value.end();
        }
        
        bool exists (T value) const {
                return value2name.find(value) != value2name.end();
        }
        
        void debug_listAllKeys () const {
                for (typename std::map<std::string, T>::const_iterator it = name2value.begin();
                     it != name2value.end();
                     ++it
                ) {
                        std::cout << it->first << ":" << it->second << std::endl;
                }
        }

private:
        std::map<std::string, T> name2value;
        std::map<T, std::string> value2name;
};

}

#endif
