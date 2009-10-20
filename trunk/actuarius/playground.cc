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

// THIS FILE IS TO PROVIDE A PLAYGROUND FOR THE DEVELOPER.
// NOTHING TO SEE HERE.

#include "actuarius.hh"

#include <iostream>
#include <fstream>
/*#include <iostream>
#include <fstream>

#include <stack>
#include <utility>
#include <map>
#include <queue>*/

class Bar;



namespace actuarius {

//-----------------------------------------------------------------------------
// serialize
//-----------------------------------------------------------------------------
template <typename Archive, typename T> 
void serialize (Archive & ar, T & val) {
        val.serialize (ar);
}

}



using namespace actuarius;

enum Foo {
        one, two, three, four
};
const Enum<Foo> fooNames
 ( Nvp<Foo>(one, "one")
 | Nvp<Foo>(two, "two")
 | Nvp<Foo>(three, "three")
 | Nvp<Foo>(four, "four")
 );


struct Bar {
        Foo foo;
        int some_int;
        std::vector <Bar> children;
        std::vector <Foo> foooos;
        float pi;

        Bar (bool)
        : foo(one), some_int(256), pi(3.14159) {
                children.push_back(Bar());
                children.push_back(Bar());
                children.push_back(Bar());
                foooos.push_back (one);
                foooos.push_back (two);
                foooos.push_back (three);
                foooos.push_back (four);
        }

        Bar ()
        : foo(four), some_int(41), pi(3.14159)
        {}
                        
                
        Bar (Bar const &rhs)
        : foo(rhs.foo), some_int(rhs.some_int), pi(rhs.pi)
        , children (rhs.children), foooos(rhs.foooos)
        {}
                
        Bar & operator = (Bar const &rhs) {
                foo = rhs.foo;
                some_int = rhs.some_int;
                some_int = rhs.some_int;
                children = rhs.children;
                foooos = rhs.foooos;
        }
        
        template<typename Arch>
        void serialize (Arch &arch);
};

template<typename Arch>
void Bar::serialize (Arch &arch) {
        using actuarius::pack;
        arch & pack ("some_int", some_int);
        arch & pack ("pi", pi);
        arch & pack ("foo", foo, fooNames);
        arch & pack ("child", children);
        //arch /*& comment("array of foo") */
        arch & pack ("foooos", foooos, fooNames);
}


int main () {        
        if (0) {
                Bar bar = Bar (true);
                std::ofstream os ("in.txt");
                OArchive(os) & make_nrp ("bar", bar);
                return 0;
        } else {
                Bar bar;
                std::ifstream is ("in.txt");
                IArchive(is) & make_nrp ("bar", bar);
                
                //std::ofstream os ("out.txt");
                OArchive(std::cout) & make_nrp ("bar", bar);
                
                std::ofstream tmp ("hehe.txt");
                OArchive(tmp) & make_nrp ("bar", bar);
        }

        //std::cout << detail::has_serialize_function<Bar, Bar>::value << std::endl;
        //std::cout << detail::has_serialize_function<actuarius::IArchive, Bar>::value << std::endl;
        //std::cout << detail::has_serialize_function<actuarius::IArchive, Bar>::value << std::endl;
        
        /*std::fstream is ("out.txt");
        parse (is);*/
}
