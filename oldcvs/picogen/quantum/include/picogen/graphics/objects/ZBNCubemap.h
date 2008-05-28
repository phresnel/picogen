/***************************************************************************
 *            ZBNCubemap.h
 *
 *  Tue Apr 29 14:30:00 2008
 *  Copyright  2008  Sebastian Mach
 *  seb@greenhybrid.net
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


#ifndef _ZBNCUBEMAP_H
#define _ZBNCUBEMAP_H

#ifndef INSIDE_OBJECTS_H
namespace picogen {
namespace graphics {
namespace objects {
#endif // #ifndef INSIDE_OBJECTS_H

class ZBNCubemapPixel {
    typedef picogen::misc::geometrics::Vector3d Vector3d;
    typedef picogen::graphics::image::color::Color Color;
    typedef picogen::misc::prim::real real;

private:
    Color color;
    real depth;
    Vector3d normal;

public:
    ZBNCubemapPixel()
            : color(1.0,0.0,0.0), depth(-1) {
    }

    Color getColor() const             {
        return color;
    }
    void  setColor( const Color &col ) {
        color = col;
    }

    real getDepth() const       {
        return depth;
    }
    void setDepth( real depth_ ) {
        depth = depth_;
    }

    Vector3d getNormal() const {
        return normal;
    }
    void setNormal( const Vector3d &normal_ ) {
        normal = normal_;
    }
};

class ZBNCubemapSurface {
    typedef picogen::misc::geometrics::Vector3d Vector3d;
    typedef picogen::graphics::image::color::Color Color;
    typedef picogen::misc::prim::real real;
private:

    ZBNCubemapPixel *surface;
    int width, height;

public:
    ZBNCubemapSurface();
    virtual ~ZBNCubemapSurface();
    void resize( int width, int height );
    void lock();
    void unlock();

    int getWidth() const;
    int getHeight() const;

    //ZBNCubemapPixel & getPixel( int x, int y );
    ZBNCubemapPixel   getPixel( real u, real v ) const;
    void              setPixel( real u, real v, const ZBNCubemapPixel &p );

    ZBNCubemapPixel   getPixel( int x, int y ) const;
    void              setPixel( int x, int y, const ZBNCubemapPixel &p );
};

class ZBNCubemap : public picogen::graphics::objects::abstract::IIntersectable {
public:
    typedef enum face_t {
        x_negative=0,
        x_positive=1,
        y_negative=2,
        y_positive=3,
        z_negative=4,
        z_positive=5
    };

private:
    typedef picogen::misc::prim::real real;
    typedef picogen::misc::geometrics::Vector3d Vector3d;
    typedef picogen::misc::geometrics::Ray Ray;
    typedef picogen::misc::geometrics::BoundingBox BoundingBox;
    typedef picogen::graphics::material::abstract::IBRDF IBRDF;
    typedef picogen::graphics::structs::intersection_t intersection_t;
    typedef picogen::graphics::objects::abstract::IIntersectable IIntersectable;
    typedef picogen::graphics::image::color::Color Color;
    typedef picogen::misc::functions::vector_to_scalar::PerlinNoise PerlinNoise;

private:
    ZBNCubemapSurface faces[6];
    static void getCubeCoords( face_t &face, real &u, real &v, const Vector3d &pos );
public:
    ZBNCubemap();
    virtual ~ZBNCubemap();
    virtual bool Intersect( param_out(intersection_t,intersection), param_in(Ray,ray) ) const;

    ZBNCubemapSurface &getSurface( face_t face );

    //ZBNCubemapPixel & getPixel( const Vector3d &worldspacePosition );
    ZBNCubemapPixel   getPixel( const Vector3d &worldspacePosition ) const;
    void              setPixel( const Vector3d &worldspacePosition, const ZBNCubemapPixel &p );
    void render( const Vector3d &origin, const IIntersectable *I );

    void lock();
    void unlock();
};

#ifndef INSIDE_OBJECTS_H
}
}
} // namespace picogen{ namespace graphics{ namespace objects{
#endif // #ifndef INSIDE_OBJECTS_H


#endif /* _ZBNCUBEMAP_H */

