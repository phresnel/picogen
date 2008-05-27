/***************************************************************************
 *            AABox.cc
 *
 *  Sat May 24 11:43:00 2008
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

#include <picogen/picogen.h>


struct BRDF : public picogen::graphics::material::abstract::IBRDF
{
	BRDF(){}
	virtual bool RandomSample(
		param_out(picogen::misc::prim::real,brdf),
		param_out(picogen::misc::prim::real,p),
		param_out(bool,specular),
		param_out(picogen::misc::geometrics::Ray,r_out),
		param_in (picogen::misc::geometrics::Ray,r_in),
		param_in (picogen::misc::geometrics::Vector3d,N)
	) const
	{
		using picogen::misc::constants::pi;
		using picogen::misc::prim::real;
		using picogen::misc::geometrics::Vector3d;

		/*if( (static_cast<real>( rand() % 10000 ) / 10000.0)>0.9 )
			return false;*/
		r_out.x() = r_in.x();
		p = 1.0;
		brdf = 1;
		do{
			r_out.w() = Vector3d(
				static_cast<real>( rand() % 20000 ) / 10000.0 - 1.0,
				static_cast<real>( rand() % 20000 ) / 10000.0 - 1.0,
				static_cast<real>( rand() % 20000 ) / 10000.0 - 1.0
			);
		}while( r_out.w().lengthSq()>1 || N*r_out.w()<0.0 );

		r_out.w() = r_out.w().normal();
		p = 1.0/(2.0*pi);// / xrt::constants::pi;
		brdf = 1.0/pi;// / xrt::constants::pi;//r_out.w().normal() * N;// / xrt::constants::pi;
		specular = false;
		return true;
	}
};
static const BRDF diffuseBRDF;


class ConstantShader : public picogen::graphics::material::abstract::IShader{
        picogen::graphics::image::color::Color color;
	public:
		virtual ~ConstantShader() {};
		ConstantShader( picogen::graphics::image::color::Color color ) : color(color) {}
		virtual void Shade(
			picogen::graphics::image::color::Color &color,
			const picogen::misc::geometrics::Vector3d &normal,
			const picogen::misc::geometrics::Vector3d &position
		) const
		{
			color = this->color;
		}
};

class RectLightShader : public picogen::graphics::material::abstract::IShader{
        picogen::graphics::image::color::Color nonLightColor, lightColor;
        picogen::prim::real L_e, size, centerX, centerZ;
	public:
		virtual ~RectLightShader() {};
		RectLightShader(
            picogen::graphics::image::color::Color nonLightColor,
            picogen::graphics::image::color::Color lightColor,
            picogen::prim::real L_e,
            picogen::prim::real size,
            picogen::prim::real centerX,
            picogen::prim::real centerZ
        ) :
            nonLightColor(nonLightColor),
            lightColor(lightColor),
            L_e(L_e),
            size(size),
            centerX(centerX),
            centerZ(centerZ)
        {}

        virtual void Shade(
            picogen::graphics::image::color::Color &color,
            const picogen::misc::geometrics::Vector3d &normal,
            const picogen::misc::geometrics::Vector3d &position
        ) const {
            color = picogen::graphics::image::color::Color(1.0,0.0,0.0);
        }

		virtual void Shade(
			picogen::graphics::image::color::Color &color,
			picogen::misc::prim::real &L_e,
			const picogen::misc::geometrics::Vector3d &normal,
			const picogen::misc::geometrics::Vector3d &position
		) const
		{
		    if( fabs( position[0]-centerX ) <= size && fabs( position[2]-centerZ ) <= size ){
		        L_e = this->L_e;
                color = lightColor;
            }else{
                L_e = 0.0;
                color = nonLightColor;
            }
		}
};

static const ConstantShader  red  ( picogen::graphics::image::color::Color( 1.0, 0.3, 0.3 ) );
static const ConstantShader  green( picogen::graphics::image::color::Color( 0.3, 1.0, 0.3 ) );
static const ConstantShader  blue ( picogen::graphics::image::color::Color( 0.3, 0.3, 1.0 ) );
static const ConstantShader  white( picogen::graphics::image::color::Color( 1.0, 1.0, 1.0 ) );
static const RectLightShader whiteL(
    picogen::graphics::image::color::Color( 1.0, 1.0, 1.0 ),
    picogen::graphics::image::color::Color( 1.0, 1.0, 1.0 ), 10.0,
    0.25,
    0.0,
    0.0
);


namespace picogen{ namespace graphics{ namespace objects{


AABox::AABox() {
    shaders[x_negative] = &red;
    shaders[x_positive] = &green;
    shaders[y_negative] = &white;
    shaders[y_positive] = &whiteL;
    shaders[z_negative] = &white;
    shaders[z_positive] = &white;

    brdfs[x_negative] =
    brdfs[x_positive] =
    brdfs[y_negative] =
    brdfs[y_positive] =
    brdfs[z_negative] =
    brdfs[z_positive] = &diffuseBRDF;

    enable[x_negative] =
    enable[x_positive] =
    enable[y_negative] =
    enable[y_positive] =
    enable[z_negative] =
    enable[z_positive] = true;

    enableOutside_ = true;
}

AABox::~AABox() {
}


void AABox::setBRDF( face_t face, const picogen::graphics::material::abstract::IBRDF* const brdf ) {
    brdfs[face] = brdf;
}


void AABox::setShader( face_t face, const picogen::graphics::material::abstract::IShader* const shader )  {
    shaders[face] = shader;
}


void AABox::setBRDF( const picogen::graphics::material::abstract::IBRDF* const brdf ) {
    setBRDF( x_negative, brdf );
    setBRDF( x_positive, brdf );
    setBRDF( y_negative, brdf );
    setBRDF( y_positive, brdf );
    setBRDF( z_negative, brdf );
    setBRDF( z_positive, brdf );
}


void AABox::setShader( const picogen::graphics::material::abstract::IShader* const shader ) {
    setShader( x_negative, shader );
    setShader( x_positive, shader );
    setShader( y_negative, shader );
    setShader( y_positive, shader );
    setShader( z_negative, shader );
    setShader( z_positive, shader );
}


void AABox::enableFace( face_t face, bool enable ){
    this->enable[ face ] = enable;
}

void AABox::enableOutside( bool enable ){
    this->enableOutside_ = enable;
}

bool AABox::Intersect( param_out(intersection_t,intersection), param_in(Ray,ray) ) const {
    using picogen::prim::real;

    const real wx = ray.w()[0];
    const real wy = ray.w()[1];
    const real wz = ray.w()[2];
    const real px = ray.x()[0];
    const real py = ray.x()[1];
    const real pz = ray.x()[2];

    const real bbMin[3] = {
        -1, -1, -1
    };

    const real bbMax[3] = {
        1, 1, 1
    };


    intersection.color = Color(1.0,1.0,1.0);
    intersection.side  = misc::constants::outside;
    //intersection.brdf  = &brdf;
    intersection.L_e   = 0.0;

    real tmin = 9999999.0;
    bool any = false;

    const picogen::graphics::material::abstract::IShader* shader = NULL;

    if( (enableOutside_ || wx<0.0) && enable[x_negative] ){
        const real dist = px - bbMin[0];
        const real t    = dist / -wx;
        const real iy = py + t*wy;
        const real iz = pz + t*wz;
        if( t > 0 && t < tmin
            && iy>=bbMin[1] && iy<=bbMax[1] && iz>=bbMin[2] && iz<=bbMax[2]
        ){
            tmin = t;
            any  = true;
            intersection.t      = t;
            intersection.brdf    = brdfs[x_negative];
            shader = shaders[x_negative];
            intersection.normal = Vector3d( wx>0.0?-1.0:1.0, 0.0, 0.0 );
        }
    }
    if( (enableOutside_ || wx>0.0) && enable[x_positive] ){
        const real dist = px - bbMax[0];
        const real t    = dist / -wx;
        const real iy = py + t*wy;
        const real iz = pz + t*wz;
        if( t > 0 && t < tmin
            && iy>=bbMin[1] && iy<=bbMax[1] && iz>=bbMin[2] && iz<=bbMax[2]
        ){
            tmin = t;
            any  = true;
            intersection.t      = t;
            intersection.brdf    = brdfs[x_positive];
            shader = shaders[x_positive];
            intersection.normal = Vector3d( wx>0.0?-1.0:1.0, 0.0, 0.0 );
        }
    }


    if( (enableOutside_ || wz<0.0) && enable[z_negative] ){
        const real dist = pz - bbMin[2];
        const real t    = dist / -wz;
        const real ix = px + t*wx;
        const real iy = py + t*wy;
        if( t > 0 && t < tmin
            && ix>=bbMin[0] && ix<=bbMax[0] && iy>=bbMin[1] && iy<=bbMax[1]
        ){
            tmin = t;
            any  = true;
            intersection.t      = t;
            intersection.brdf    = brdfs[z_negative];
            shader = shaders[z_negative];
            intersection.normal = Vector3d( 0.0, 0.0, wz>0.0?-1.0:1.0 );
        }
    }
    if( (enableOutside_ || wz>0.0) && enable[z_positive] ){
        const real dist = pz - bbMax[2];
        const real t    = dist / -wz;
        const real ix = px + t*wx;
        const real iy = py + t*wy;
        if( t > 0 && t < tmin
            && ix>=bbMin[0] && ix<=bbMax[0] && iy>=bbMin[1] && iy<=bbMax[1]
        ){
            tmin = t;
            any  = true;
            intersection.t      = t;
            intersection.brdf    = brdfs[z_positive];
            shader = shaders[z_positive];
            intersection.normal = Vector3d( 0.0, 0.0, wz>0.0?-1.0:1.0 );
        }
    }

    if( (enableOutside_ || wy<0.0) && enable[y_negative] ){
        const real dist = py - bbMin[1];
        const real t    = dist / -wy;
        const real ix = px + t*wx;
        const real iz = pz + t*wz;
        if( t > 0 && t < tmin
            && ix>=bbMin[0] && ix<=bbMax[0] && iz>=bbMin[2] && iz<=bbMax[2]
        ){
            tmin = t;
            any  = true;
            intersection.t      = t;
            intersection.brdf    = brdfs[y_negative];
            shader = shaders[y_negative];
            intersection.normal = Vector3d( 0.0, wy>0.0?-1.0:1.0, 0.0 );
        }
    }
    if( (enableOutside_ || wy>0.0) && enable[y_positive] ){
        const real dist = py - bbMax[1];
        const real t    = dist / -wy;
        const real ix = px + t*wx;
        const real iz = pz + t*wz;
        if( t > 0 && t < tmin
            && ix>=bbMin[0] && ix<=bbMax[0] && iz>=bbMin[2] && iz<=bbMax[2]
        ){
            tmin = t;
            any  = true;
            intersection.t      = t;
            intersection.brdf    = brdfs[y_positive];
            shader = shaders[y_positive];
            intersection.normal = Vector3d( 0.0, wy>0.0?-1.0:1.0, 0.0 );
        }
    }

    if( NULL != shader ){
        shader->Shade( intersection.color, intersection.L_e, intersection.normal, ray(intersection.t) );
    }
    return any;
}


} } } // namespace picogen{ namespace graphics{ namespace objects{
