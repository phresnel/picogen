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



class SimpleHeightField : public abstract::IIntersectable {
private:
    typedef unsigned short t_heightVal;
    misc::geometrics::BoundingBox m_scaleBox;
    misc::prim::real m_scaleSize[3];
    misc::prim::real m_vScale;
    const material::abstract::IBRDF *m_pBRDF;
    const material::abstract::IShader *m_pShader;
    t_heightVal *m_pField;
    unsigned int m_size;
    misc::prim::real m_scaleBoxMin[3];
    misc::prim::real m_invertHMapSize;
    misc::prim::real m_invertScaleSize[3];
    misc::prim::real m_scaleSize_mul_invertHMapSize[3];
private:
    void updateOptimizerVars();
    inline misc::prim::real htor( t_heightVal v ) const {
        return ( misc::prim::real( v ) / misc::prim::real( 0xFFFF ) );
    }
    inline t_heightVal rtoh( misc::prim::real v ) const {
        static const misc::prim::real maxF = misc::prim::real( 0xFFFE );
        const misc::prim::real f = misc::prim::real( v ) * maxF;
        return t_heightVal( f < 0.0 ? 0 : f >= maxF ? maxF : f );
    }

    misc::prim::real smoothedHeightFunc( const misc::functions::abstract::uv_to_scalar *heightFunc, misc::prim::real u, misc::prim::real v, misc::prim::real cellSizeX, misc::prim::real cellSizeY );

    inline int intersectVoxel(
        param_out( structs::intersection_t, intersection ),
        int u, int v,
        misc::prim::real x1, misc::prim::real z1, misc::prim::real x2, misc::prim::real z2,
        param_in( misc::geometrics::Ray, ray ) ) const;
    inline int lineIntersection(
        param_out( structs::intersection_t, intersection ),
        misc::prim::real au, misc::prim::real av,
        misc::prim::real bu, misc::prim::real bv,
        param_in( misc::geometrics::Ray, ray ) ) const;
    /*inline void shade(
        param_out(image::color::Color,color),
        param_in(misc::geometrics::Vector3d,normal) ) const;*/
public:
    SimpleHeightField();
    virtual ~SimpleHeightField();
    void SetBox( param_in( misc::geometrics::Vector3d, min ), param_in( misc::geometrics::Vector3d, max ) );
    void SetBRDF( const material::abstract::IBRDF* brdf );
    void SetShader( const material::abstract::IShader* shader );
    void Init( unsigned int size, const misc::functions::abstract::uv_to_scalar *heightFunc, misc::prim::real boundsGuessAccuracy = 1.0, bool smooth = true );

    virtual bool Intersect( param_out( structs::intersection_t, intersection ), param_in( misc::geometrics::Ray, ray ) ) const;
};




#endif /* _SIMPLEHEIGHTFIELD_H */
