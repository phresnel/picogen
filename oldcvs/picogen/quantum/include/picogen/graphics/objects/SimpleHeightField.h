/***************************************************************************
 *            SimpleHeightField.h
 *
 *  Sun Oct 28 12:07:14 2007
 *  Copyright  2007  Sebastian Mach
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


#ifndef _SIMPLEHEIGHTFIELD_H
#define _SIMPLEHEIGHTFIELD_H

namespace picogen {
    namespace graphics {
        namespace objects {



            class SimpleHeightField : public ::picogen::graphics::objects::abstract::IIntersectable {
                    /// \todo de-uglify typenames
                private:
                    typedef ::picogen::misc::prim::real real;
                    typedef ::picogen::misc::geometrics::Vector3d Vector3d;
                    typedef ::picogen::misc::geometrics::Ray Ray;
                    typedef ::picogen::misc::geometrics::BoundingBox BoundingBox;
                    typedef ::picogen::graphics::material::abstract::IBRDF IBRDF;
                    typedef ::picogen::graphics::material::abstract::IShader IShader;
                    typedef ::picogen::graphics::structs::intersection_t intersection_t;



                    typedef unsigned short t_heightVal;
                    BoundingBox m_scaleBox;
                    real m_scaleSize[3];
                    real m_vScale;
                    const IBRDF *m_pBRDF;
                    const IShader *m_pShader;
                    t_heightVal *m_pField;
                    unsigned int m_size;
                    real m_scaleBoxMin[3];
                    real m_invertHMapSize;
                    real m_invertScaleSize[3];
                    real m_scaleSize_mul_invertHMapSize[3];
                private:
                    void updateOptimizerVars();
                    inline real htor (t_heightVal v) const {
                        return (real (v) / real (0xFFFF));
                    }
                    inline t_heightVal rtoh (real v) const {
                        static const real maxF = real (0xFFFE);
                        const real f = misc::prim::real (v) * maxF;
                        return t_heightVal (f < 0.0 ? 0 : f >= maxF ? maxF : f);
                    }

                    real smoothedHeightFunc (const ::picogen::misc::functions::abstract::uv_to_scalar *heightFunc, real u, real v, real cellSizeX, real cellSizeY);

                    inline int intersectVoxel (
                        param_out (intersection_t, intersection),
                        int u, int v,
                        real x1, real z1, real x2, real z2,
                        param_in (Ray, ray)) const;
                    inline int lineIntersection (
                        param_out (intersection_t, intersection),
                        real au, real av,
                        real bu, real bv,
                        param_in (Ray, ray)) const;
                    /*inline void shade(
                        param_out(image::color::Color,color),
                        param_in(misc::geometrics::Vector3d,normal) ) const;*/
                public:
                    SimpleHeightField();
                    virtual ~SimpleHeightField();
                    void setBox (param_in (Vector3d, min), param_in (Vector3d, max));
                    void setBRDF (const IBRDF* brdf);
                    void setShader (const IShader* shader);
                    void init (unsigned int size, const ::picogen::misc::functions::abstract::uv_to_scalar *heightFunc, real boundsGuessAccuracy = 1.0, bool smooth = true);

                    virtual bool intersect (param_out (intersection_t, intersection), param_in (Ray, ray)) const;
            };

        } // namespace objects {
    } // namespace graphics {
} // namespace picogen {



#endif /* _SIMPLEHEIGHTFIELD_H */
