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

#ifndef OARCHIVE_HH_INCLUDED_20090829
#define OARCHIVE_HH_INCLUDED_20090829

#include <stack>

//-----------------------------------------------------------------------------
// OArchive
//-----------------------------------------------------------------------------
namespace actuarius {
class OArchive {
public:
        enum { serialize = 1, deserialize = 0 };

        OArchive (std::ostream &out) 
        : indendation(0), out(out)
        {
                path.push ("/");
        }
        
        OArchive (OArchive const & oa)
        : indendation(oa.indendation), out (oa.out)
        {}

        OArchive &operator & (comment val) {
                out << indent() << "/* " << val.text << " */\n";
                return *this;
        }
        
        template <typename T>
        typename detail::disable_if<
                detail::has_serialize_function<OArchive,T>, 
                OArchive
        >::type &
        operator & (nrp<T> val) {
                path.push (path.top() + val.name + "/");
                //std::cout << path.top() << '\n';
                
                out << indent() << val.name << ":" << val.value << ";\n";
                path.pop ();
                return *this;
        }
        
        template <typename T>
        OArchive &operator & (ncrp<T> val) {
                path.push (path.top() + val.name + "/");
                
                for (typename T::iterator it = val.value.begin(); 
                     it!=val.value.end();
                     ++it
                ) {
                        *this & make_nrp (val.name, *it);
                }
                path.pop ();
                return *this;
        }
        
        template <typename T>
        OArchive &operator & (necrp<T> val) {
                path.push (path.top() + val.name + "/");
                
                for (typename T::iterator it = val.value.begin(); 
                     it!=val.value.end();
                     ++it
                ) {
                        *this & make_nerp (val.name, val.enumDesc, *it);
                }
                path.pop ();
                return *this;
        }
        
        template <typename T>
        OArchive &operator & (nerp<T> val) {
                path.push (path.top() + val.name + "/");
                out << indent()
                    << val.name << ":" << val.enumDesc[val.value] << ";\n";
                path.pop ();
                return *this;
        }
        
        template <typename T> 
        typename detail::enable_if<
                detail::has_serialize_function<OArchive,T>, 
                OArchive
        >::type&
        operator & (nrp<T> val) {
                path.push (path.top() + val.name + "/");
                //std::cout << path.top() << '\n';
                
                out << indent() << val.name << "{\n";                
                ++indendation;                                
                val.value.serialize (*this);                
                --indendation;
                out << indent () << "}\n" << std::flush;
                path.pop ();
                return *this;
        }
        
private:
        int indendation;
        std::ostream &out;
        std::string indent () const {
                std::string ret = "";
                for (int i=0; i<indendation; ++i)
                        ret += "    ";
                return ret;
        }
        
        std::stack<std::string> path;
        
        OArchive & operator = (OArchive const & rhs);
};
}


#endif // OARCHIVE_HH_INCLUDED_20090829
