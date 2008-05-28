/***************************************************************************
 *            exceptions.h
 *
 *  Fri Oct 12 19:23:30 2007
 *  Copyright  2007  Sebastian Mach
 *  root@greenhybrid.net
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 3 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

namespace exceptions {

class base {
public:
    const std::string filename;
    const int line;
    const std::string message;
private:
    base() : filename(), line(), message() {}
public:
    base( const std::string _filename, int _line, const std::string _message ) :
            filename(_filename), line(_line), message(_message) {}
};
class null_pointer : public base {
private:
    null_pointer() : base( "", -1, "" ) {}
public:
    null_pointer( const std::string _filename, int _line, const std::string _message ) :
            base( _filename, _line, std::string("null_pointer exception: ") + _message ) {}
};
class invalid_call : public base {
private:
    invalid_call() : base( "", -1, "" ) {}
public:
    invalid_call( const std::string _filename, int _line, const std::string _message ) :
            base( _filename, _line, std::string("invalid_call exception: ") + _message ) {}
};
class value_constraint_2 : public base {
private:
    value_constraint_2() : base( "", -1, "" ) {}
public:
    value_constraint_2( const std::string _filename, int _line, const std::string _message )
            : base(_filename,_line,
                   std::string("value_constraint_2 exception: ") + _message
                  ) {
    }
};
class index_out_of_bounds : public base {
private:
    index_out_of_bounds() : base( "", -1, "" ) {}
public:
    template<class A,class B, class C>
    index_out_of_bounds( const std::string _filename, int _line, A min, B max, C actual )
            : base(_filename,_line,
                   std::string("while index bounds go from ")+string_helpers::tostring(min) + " to "
                   + string_helpers::tostring(max) + " exclusive, it has been tried to access index "
                   + string_helpers::tostring(actual)
                  ) {
    }
};
class coords_out_of_bounds : public base {
private:
    coords_out_of_bounds() : base( "", -1, "" ) {}
public:
    template<class LEFT,class TOP, class RIGHT, class BOTTOM, class X, class Y>
    coords_out_of_bounds( const std::string _filename, int _line,
                          LEFT left, TOP top, RIGHT right, BOTTOM bottom, X x, Y y )
            : base(_filename,_line,
                   std::string("while the rect is (")
                   +string_helpers::tostring(left)+"|"+string_helpers::tostring(top)+"|"
                   +string_helpers::tostring(right)+"|"+string_helpers::tostring(bottom)+")"
                   +" it has been tried to access ("+string_helpers::tostring(x)+"|"+string_helpers::tostring(y)+")"
                  ) {
    }
};
class out_of_memory : public base {
private:
    out_of_memory() : base( "", -1, "" ) {}
public:
    template<class A>
    out_of_memory( const std::string _filename, int _line, A requested_size )
            : base(_filename,_line,
                   std::string("out of memory while trying to allocate ")+string_helpers::tostring(requested_size)
                   + std::string( " bytes of memory." )
                  ) {
    }
};

};

#endif /* _EXCEPTIONS_H */
