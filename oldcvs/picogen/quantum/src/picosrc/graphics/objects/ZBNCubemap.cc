/***************************************************************************
 *            ZBNCubemap.h
 *
 *  Tue Apr 29 15:40:00 2008
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

#include <string>

#include <picogen/picogen.h>

using picogen::real;
using picogen::geometrics::Vector3d;
using picogen::geometrics::Ray;
using picogen::geometrics::BoundingBox;
using picogen::graphics::material::abstract::IBRDF;
using picogen::graphics::structs::intersection_t;
using picogen::graphics::objects::abstract::IIntersectable;
using picogen::graphics::color::Color;


using std::string;
using namespace picogen::misc::exceptions;


struct BRDF : public picogen::graphics::material::abstract::IBRDF {
    BRDF() {}
    virtual bool randomSample (
        param_out (picogen::real,brdf),
        param_out (picogen::real,p),
        param_out (bool,specular),
        param_out (picogen::geometrics::Ray,r_out),
        param_in (picogen::geometrics::Ray,r_in),
        param_in (picogen::geometrics::Vector3d,N)
    ) const {
        using picogen::constants::pi;
        using picogen::real;
        using picogen::geometrics::Vector3d;

        /*if( (static_cast<real>( rand() % 10000 ) / 10000.0)>0.9 )
         return false;*/
        r_out.setPosition( r_in.getPosition() );
        p = 1.0;
        brdf = 1;
        do {
            r_out.w() = Vector3d (
                            static_cast<real> (rand() % 20000) / 10000.0 - 1.0,
                            static_cast<real> (rand() % 20000) / 10000.0 - 1.0,
                            static_cast<real> (rand() % 20000) / 10000.0 - 1.0
                        );
        } while (r_out.getDirection().computeLengthSq() >1 || N*r_out.getDirection() <0.0);

        //r_out.w() = r_out.w().normal();
        r_out.setDirection( r_out.getDirection().computeNormal() );
        p = 1.0/ (2.0*pi);// / xrt::constants::pi;
        brdf = 1.0/pi;// / xrt::constants::pi;//r_out.w().normal() * N;// / xrt::constants::pi;
        specular = false;
        return true;
    }
};
static BRDF brdf;



namespace picogen {
    namespace graphics {
        namespace objects {


            ZBNCubemapSurface::ZBNCubemapSurface() : surface (0), width (0), height (0) {
            }



            ZBNCubemapSurface::~ZBNCubemapSurface() {
                if (0 != surface) {
                    delete [] surface;
                    surface = 0;
                }
                width = height = 0;
            }



            void ZBNCubemapSurface::resize (int new_width, int new_height) {
                if (0 != surface) {
                    delete [] surface;
                    surface = 0;
                }
                width   = height = 0;
                surface = new ZBNCubemapPixel[new_width*new_height];
                width   = new_width;
                height  = new_height;
            }



            void ZBNCubemapSurface::lock() {
            }



            void ZBNCubemapSurface::unlock() {
            }



            ZBNCubemapPixel ZBNCubemapSurface::getPixel (int x, int y) const {
                if (! ( (x>=0) & (x<width) & (y>=0) & (y<height))) {
                    throw coords_out_of_bounds (__FILE__, __LINE__, 0, 0, width, height, x, y);
                }
                if (0==surface) {
                    throw null_pointer (__FILE__, __LINE__, string ("surface has not been allocated"));
                }
                return surface[ y*width + x ];
            }



            /*
            ZBNCubemapPixel & ZBNCubemapSurface::getPixel( int x, int y ){
                if( !( (x>=0) & (x<width) & (y>=0) & (y<height) ) ){
                    throw coords_out_of_bounds( __FILE__, __LINE__, 0, 0, width, height, x, y );
                }
                if( 0==surface ){
                    throw null_pointer( __FILE__, __LINE__, string("surface has not been allocated") );
                }
                return surface[ y*width + x ];
            }*/



            ZBNCubemapPixel ZBNCubemapSurface::getPixel (real u, real v) const {
                const int x = static_cast<int> (u*static_cast<real> (width-1)  + 0.5);
                const int y = static_cast<int> (v*static_cast<real> (height-1) + 0.5);
                if (! ( (x>=0) & (x<width) & (y>=0) & (y<height))) {
                    throw coords_out_of_bounds (__FILE__, __LINE__, 0, 0, width, height, x, y);
                }
                if (0==surface) {
                    throw null_pointer (__FILE__, __LINE__, string ("surface has not been allocated"));
                }
                return surface[ y*width + x ];
            }



            void ZBNCubemapSurface::setPixel (real u, real v, const ZBNCubemapPixel &p) {
                const int x = static_cast<int> (u*static_cast<real> (width-1)  + 0.5);
                const int y = static_cast<int> (v*static_cast<real> (height-1) + 0.5);
                if (! ( (x>=0) & (x<width) & (y>=0) & (y<height))) {
                    throw coords_out_of_bounds (__FILE__, __LINE__, 0, 0, width, height, x, y);
                }
                if (0==surface) {
                    throw null_pointer (__FILE__, __LINE__, string ("surface has not been allocated"));
                }
                surface[ y*width + x ] = p;
            }



            void ZBNCubemapSurface::setPixel (int x, int y, const ZBNCubemapPixel &p) {
                if (! ( (x>=0) & (x<width) & (y>=0) & (y<height))) {
                    throw coords_out_of_bounds (__FILE__, __LINE__, 0, 0, width, height, x, y);
                }
                if (0==surface) {
                    throw null_pointer (__FILE__, __LINE__, string ("surface has not been allocated"));
                }
                surface[ y*width + x ] = p;
            }



            int ZBNCubemapSurface::getWidth() const {
                return width;
            }



            int ZBNCubemapSurface::getHeight() const {
                return height;
            }



            //==-------------------------------------------------------------------------==
            //=
            //=
            //==-------------------------------------------------------------------------==


            ZBNCubemap::ZBNCubemap() {
            }



            ZBNCubemap::~ZBNCubemap() {
            }



            ZBNCubemapSurface &ZBNCubemap::getSurface (face_t face) {
                return faces[face];
            }



            void ZBNCubemap::lock() {
                if (0 != faces) {
                    for (int i=0; i<6; ++i)
                        faces[i].lock();
                }
            }



            void ZBNCubemap::unlock() {
                if (0 != faces) {
                    for (int i=0; i<6; ++i)
                        faces[i].unlock();
                }
            }



            void ZBNCubemap::getCubeCoords (face_t &face, real &u, real &v, const Vector3d &pos) {
                static const face_t lut[3][2] = {
                    { x_negative, x_positive, },
                    { y_negative, y_positive, },
                    { z_negative, z_positive, },
                };

                int index = 0;
                real max = fabs (pos[0]);
                if (fabs (pos[1]) > max) {
                    index = 1;
                    max = fabs (pos[1]);
                }
                if (fabs (pos[2]) > max) {
                    index = 2;
                    max = fabs (pos[2]);
                }
                face = (pos[index]<0) ? lut[index][0] : lut[index][1];

                switch (face) {
                    case x_negative:
                        u = pos[2] / -pos[0];
                        v = pos[1] / -pos[0];
                        break;
                    case x_positive:
                        u = -pos[2] / pos[0];
                        v = pos[1] / pos[0];
                        break;

                    case y_negative:
                        u = pos[0] / -pos[1];
                        v = pos[2] / -pos[1];
                        break;
                    case y_positive:
                        u = pos[0] / pos[1];
                        v = -pos[2] / pos[1];
                        break;

                    case z_negative:
                        u = -pos[0] / -pos[2];
                        v = pos[1] / -pos[2];
                        break;
                    case z_positive:
                        u = pos[0] / pos[2];
                        v = pos[1] / pos[2];
                        break;
                };

                // [-1..1] >>> [0..1]
                u = u*0.5+0.5;
                v = v*0.5+0.5;
            }



            ZBNCubemapPixel ZBNCubemap::getPixel (const Vector3d &pos) const {
                face_t face;
                real u,v;
                getCubeCoords (face, u, v, pos);
                return faces[face].getPixel (u, v);
            }



            /*
            ZBNCubemapPixel & ZBNCubemap::getPixel( const Vector3d &pos ){
                face_t face;
                real u,v;
                getCubeCoords( face, u, v, pos );
                //printf( "( %2f, %2f, %2f ) > ( %.2f, %.2f ) | %u\n", pos[0], pos[1], pos[2], u, v, face );
                return faces[face].getPixel( u, v );
            }*/



            void ZBNCubemap::setPixel (const Vector3d &pos, const ZBNCubemapPixel &p) {
                face_t face;
                real u,v;
                getCubeCoords (face, u, v, pos);
                faces[face].setPixel (u, v, p);
            }



            bool ZBNCubemap::intersect (param_out (intersection_t,intersection), param_in (Ray,ray)) const {
                if (0) {
                    face_t face;
                    real u,v;
                    getCubeCoords (face, u, v, ray.w());
                    const real eps = 0.025;
                    if (u<eps || v<eps || u> (1.0-eps) || v> (1.0-eps)) {
                        intersection.color = Color (1.0,1.0,1.0);
                        intersection.t     = 0.1;
                        intersection.side  = constants::outside;
                        intersection.normal = Vector3d (0,1,0);
                        intersection.brdf  = &brdf;
                        intersection.L_e   = 1.0;
                        return true;
                    }
                }
                const ZBNCubemapPixel p = getPixel (ray.w());
                const real depth = p.getDepth();
                if (depth > 0.0) {
                    intersection.color = p.getColor();
                    intersection.t     = depth;
                    intersection.side  = constants::outside;
                    intersection.normal = p.getNormal();
                    intersection.brdf  = &brdf;
                    intersection.L_e   = 0.0;
                    return true;
                }
                return false;
            }



            void ZBNCubemap::render (const Vector3d &origin, const IIntersectable *I) {

                static const Vector3d planes[6][3] = { // base | u | v
                    { Vector3d (-1,0,0), Vector3d (0,0,1), Vector3d (0,1,0), }, // x-negative
                    { Vector3d (1,0,0), Vector3d (0,0,-1), Vector3d (0,1,0), }, // x-positive

                    { Vector3d (0,-1,0), Vector3d (1,0,0), Vector3d (0,0,1), }, // y-negative
                    { Vector3d (0,1,0), Vector3d (1,0,0), Vector3d (0,0,-1), }, // y-positive

                    { Vector3d (0,0,-1), Vector3d (-1,0,0), Vector3d (0,1,0), }, // z-negative
                    { Vector3d (0,0,1), Vector3d (1,0,0), Vector3d (0,1,0), }, // z-positive
                };

                lock();

                Ray ray;
                ray.x() = origin;

                printf ("rendering to ZBNCubemap{");
                int displayUpdate = 0;
                for (int face=0; face<6; face++) {
                    int width  = faces[face].getWidth();
                    int height = faces[face].getHeight();
                    printf ("\n  face %i: width=%i, height=%i\n", face, width, height);
                    for (int y=0; y<height; ++y) {

                        for (int x=0; x<width; ++x) {
                            const real u =  -1.0 + 2.0* (static_cast<real> (x)-0.5) / static_cast<real> (width-1);
                            const real v =  -1.0 + 2.0* (static_cast<real> (y)-0.5) / static_cast<real> (height-1);
                            ray.w() = (planes[face][0] + planes[face][1] * u + planes[face][2] * v).normal();

                            ZBNCubemapPixel pixel = faces[face].getPixel (x, y);

                            intersection_t intersection;
                            const real d = pixel.getDepth();
                            if (I->intersect (intersection,ray) && (d<0.0 || intersection.t < d)) {
                                pixel.setDepth (intersection.t);
                                pixel.setColor (intersection.color);
                                pixel.setNormal (intersection.normal);
                                faces[face].setPixel (x, y, pixel);
                            } else {
                                //pixel.setDepth( -1.0 );
                            }
                        }
                        if (++displayUpdate > 20) {
                            displayUpdate = 0;
                            printf ("\r   %.2f%% (%.2f%% total)",
                                    100.0f * static_cast<float> (y) /static_cast<float> (height),
                                    ( (static_cast<float> (y) /static_cast<float> (height*6))
                                      + static_cast<float> (face) /6.0f
                                    ) * 100.0f
                                   );
                            fflush (stdout);
                        }
                    }
                }
                printf ("\n}\n");

                unlock();
            }

        };
    };
};
