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

#include "film.h"



namespace crystal {



Film::Average::Average () : radiance_(Radiance::Black()), count_(0)
{
}

void Film::Average::operator+= (Radiance const &rad)
{
        radiance_ += rad;
        ++count_;
}

Radiance Film::Average::radiance() const
{
        return count_ ? radiance_ * (1/count_)
                      : Radiance::FromRgb(1, 0.5, 0.25);
}



Film::Film (int width, int height /*real crop[4]*/)
        : surface_(width*height), width_(width), height_(height)
{
}

void Film::addSample (CameraSample const &cs, Radiance const &rad)
{
        const int out_x = cs.imageX,
                  out_y = cs.imageY;
        if (out_x < 0 || out_x>=width_) return;
        if (out_y < 0 || out_y>=height_) return;
        surface_[out_x + out_y*width_] += rad;
}

Radiance Film::operator() (int x, int y) const
{
        if (x < 0 || x>=width_)  return Radiance::Black();
        if (y < 0 || y>=height_) return Radiance::Black();
        return surface_[x + y*width_].radiance();
}

int Film::width() const
{
        return width_;
}

int Film::height() const
{
        return height_;
}



}
