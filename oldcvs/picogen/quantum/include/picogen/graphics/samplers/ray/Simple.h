/***************************************************************************
 *            path_integrator_WhittedStyle.h
 *
 *  Sun Aug 12 12:08:41 2007
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


#ifndef _RAYSAMP_SIMPLE_H
#define _RAYSAMP_SIMPLE_H

class Simple {
private:
    typedef misc::prim::real              real;
    typedef misc::geometrics::Vector3d    Vector3d;
    typedef misc::geometrics::Ray         Ray;
    typedef graphics::image::color::Color Color;
    typedef graphics::structs::intersection_t intersection_t;
    typedef graphics::objects::abstract::IIntersectable IIntersectable;
    typedef graphics::objects::abstract::ISky ISky;

    IIntersectable *m_intersectable;
    ISky *m_skyShader;
    Color L_i( param_in(Ray,ray), bool specularOrFirst, param_out(intersection_t,primaryIntersection), unsigned int max );
public:
    Simple( IIntersectable *intersectable );
    Simple();
    virtual ~Simple();
    void SetIntersectable( IIntersectable *intersectable );
    void SetSky( ISky *skyShader );
    bool Integrate( param_out(Color, color), param_out(intersection_t,primaryIntersection), param_in(Ray,ray) );
};


#endif /* _RAYSAMP_SIMPLE_H */
