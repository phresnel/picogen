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

#ifndef RECTANGLE_H_INCLUDED_20090311
#define RECTANGLE_H_INCLUDED_20090311

namespace redshift {
        class Rectangle {
        public:
                int left;
                int top;
                int right;
                int bottom;

                Rectangle (int left_, int top_, int right_, int bottom_) 
                : left(left_), top(top_), right(right_), bottom(bottom_) 
                {}
                
                Rectangle () : left(), top(), right(), bottom() {}
                
                Rectangle (Rectangle const &r)
                : left(r.left), top(r.top), right(r.right), bottom(r.bottom)
                {}
                
                Rectangle &operator  = (Rectangle const &rhs) {
                        this->left   = rhs.left;
                        this->top    = rhs.top;
                        this->right  = rhs.right;
                        this->bottom = rhs.bottom;
                        return *this;
                }

                int width () const {
                        return right - left;
                }
                
                int height () const {
                        return bottom - top;
                } 
                
                int area () const {
                        return width() * height();
                }
                
                // Non-Members.
                
                friend inline int width (Rectangle const &r) {
                        return r.width();
                }
                
                friend inline int height (Rectangle const &r) {
                        return r.height();
                }
                
                friend inline int area (Rectangle const &r) {
                        return r.area();
                }
        };
}

#endif // RECTANGLE_H_INCLUDED_20090311
