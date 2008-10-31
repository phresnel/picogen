/***************************************************************************
 *            CloudAdapter.cc
 *
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
#include <picogen/graphics/objects/CloudAdapter.h>

namespace {
    using ::picogen::real;
    using ::picogen::geometrics::Vector3d;
    using ::picogen::geometrics::Ray;
    using ::picogen::graphics::color::Color;
    using ::picogen::graphics::objects::Preetham;
    using ::picogen::misc::functional::Function_R2_R1;
    using ::picogen::misc::functional::Function_R2_R1_Refcounted;



    real sat01 (real f) {
        return f < 0 ? 0 : f > 1 ? 1 : f;
    }



    real max (real a, real b) {
        return a > b ? a : b;
    }



    real f2_to_hemisphere (real fx, real fy) {
        const real fz_sq = 1 - fx*fx - fy*fy;
        if (fz_sq < 0.0) {
            return -1;
        }
        return sqrt (fz_sq);
    }
}



namespace picogen {
    namespace graphics {
        namespace objects {

            CloudAdapter::CloudAdapter (const Preetham &preetham, boost::intrusive_ptr<Function_R2_R1_Refcounted> fun)
            : preetham (preetham), fun (fun) {
            }



            CloudAdapter::CloudAdapter (const Preetham &preetham) : preetham (preetham), fun (0) {
            }



            void CloudAdapter::setCloudFunction (boost::intrusive_ptr<Function_R2_R1_Refcounted> fun) {
                this->fun = fun;
            }



            void CloudAdapter::shade (param_out (Color,color), param_in (Ray,ray)) const {
                //std::cout << (void*)(&*fun);
                //preetham.shade (color, ray);
                Color tmp = (*this) (ray);
                //sunShade (color, ray);
                color = tmp;
            }



            void CloudAdapter::sunShade (param_out (Color,color), param_in (Ray,ray)) const {
                preetham.sunShade (color, ray);
            }



            void CloudAdapter::sunSample (param_out (Color,color), param_out (Ray,ray), param_out (real,p), param_in (Vector3d,position)) const {
                preetham.sunSample (color, ray, p, position);
            }



            void CloudAdapter::atmosphereShade (param_out (Color,color), param_in (Color,src_color), param_in (Ray,ray), real distance) const {
                preetham.atmosphereShade (color, src_color, ray, distance);
            }



            void CloudAdapter::getSunColor (param_out (Color,color)) const {
                preetham.getSunColor (color);
            }



            void CloudAdapter::getSunDirection (param_out (Vector3d,direction)) const {
                preetham.getSunDirection (direction);
            }



            real CloudAdapter::getSunArealFactor () const {
                return preetham.getSunArealFactor ();
            }



            Color CloudAdapter::operator () (const Ray &ray_) const {
                //return Color (1,0,0);
                Ray ray;
                ray.setPosition (Vector3d (0,0,0));
                ray.setDirection (ray_.getDirection());
                #if 0
                    const real v = (real)y / (real)p_target->h;
                    const real u = (real)x / (real)p_target->w;
                    const real rs = 1;
                #else
                    const real cloudPlaneHeight = 2500.0;
                    const real rs = cloudPlaneHeight / ray.getDirection() [1]; // insert constant value here to get rid of the spherical distortion
                    const real u = ray (rs) [0];
                    const real v = ray (rs) [2];
                #endif

                Vector3d realSunDirection_init; preetham.getSunDirection (realSunDirection_init);
                const Vector3d realSunDirection = realSunDirection_init;
                const Vector3d projectedSunPosition (
                    realSunDirection * (cloudPlaneHeight / realSunDirection [1]) // <-- exactly on cloud plane
                    - realSunDirection * 100
                );
                const Vector3d projectedSunDirection (-(projectedSunPosition - ray (rs)).computeNormal());




                const real si = 25;//0.0005 * rs * rs;
                const real h = max (0, (*fun) (u, v));
                const real hu1 = max (0, (*fun) (u-si, v));
                const real hu2 = max (0, (*fun) (u+si, v));
                const real hv1 = max (0, (*fun) (u, v-si));
                const real hv2 = max (0, (*fun) (u, v+si));

                // compute tangent vectors
                const real H = si;
                const Vector3d U = /*Vector3d (si,0,0).computeNormal(); //*/ Vector3d (si, H*(hu2-hu1), 0).computeNormal();
                const Vector3d V = /*Vector3d (0,0,si).computeNormal(); //*/ Vector3d (0, H*(hv2-hv1), si).computeNormal();
                //--


                // cloud's surface normal
                const Vector3d normal_ = U.computeCross (V).computeNormal();

                #if 0
                    // these lines create a transformation on the hemisphere
                    // my intention was to get an effect where it's like looking to the side of the clouds near the horizon
                    Vector3d diff_ [2];
                    {
                        const real fx = ray.w()[0];
                        const real fy = ray.w()[2];

                        Vector3d U1 (fx-si, 0 , fy);   U1 = U1 + Vector3d (0, f2_to_hemisphere (U1[0], U1[2]), 0);
                        Vector3d U2 (fx+si, 0 , fy);   U2 = U2 + Vector3d (0, f2_to_hemisphere (U2[0], U2[2]), 0);

                        Vector3d V1 (fx, 0 , fy-si);   V1 = V1 + Vector3d (0, f2_to_hemisphere (V1[0], V1[2]), 0);
                        Vector3d V2 (fx, 0 , fy+si);   V2 = V2 + Vector3d (0, f2_to_hemisphere (V2[0], V2[2]), 0);

                        diff_ [0] = (U2 - U1).computeNormal ();
                        diff_ [1] = (V2 - V1).computeNormal ();
                    };
                    const Vector3d diff [2] = { diff_[0], diff_[1] };

                    // transform normal
                    const Vector3d normal =
                        ( diff[0]*normal_[0]
                         +ray.getDirection()*-normal_[1]
                         +diff[1]*normal_[2]
                        ).computeNormal()
                    ;//*/
                #else
                    // no transformation
                    const Vector3d normal = normal_;
                #endif


                // compute sunsky color
                Color skyColor, sunColorInRay;
                preetham.shade (skyColor, ray);
                preetham.sunShade (sunColorInRay, ray);
                const Color sunSky = sunColorInRay + skyColor;

                Color sunColor_init; preetham.getSunColor (sunColor_init);
                const Color sunColor = sunColor_init;

                const real dotProjected = (normal * projectedSunDirection);
                const real dot_ray_sun_real = (ray.w() * realSunDirection);

                // compute 'lambertian' light
                real diffuse_r=0, diffuse_g=0, diffuse_b=0;
                real diffuseDot;
                {
                    diffuseDot = dotProjected;
                    //diffuseDot = (diffuseDot*0.5 + 0.5); // [0..1]
                    diffuseDot += 0.0;//diffuseDot*0.75+0.25;
                    //diffuseDot = diffuseDot*0.75 + 0.25;   // [0.25..1]
                    diffuseDot = diffuseDot<0?0:diffuseDot>1?1:diffuseDot;
                    //diffuseDot = pow (diffuseDot, 3);

                    //diffuseDot = diffuseDot * 2 - 1;//pow (diffuseDot, 1.0);
                    //diffuseDot = diffuseDot<0?0:diffuseDot>1?1:diffuseDot;

                    (sunColor * diffuseDot).to_rgb (diffuse_r, diffuse_g, diffuse_b);
                }

                // compute 1-scatter light
                real s1_light_r=0, s1_light_g=0, s1_light_b=0;
                if (true) {
                    real s = dot_ray_sun_real;
                    s = s<0?0:s>1?1:s;
                    s = pow (s, 64); // reduce the solid angle of the sun contributing to silver lines
                    s = s / (1+pow(h*2,4)); // this concentrates the silver lines onto the edges (where h is low)
                    s *= 1.0;
                    Color s1 = sunColor * s;
                    s1.to_rgb (s1_light_r, s1_light_g, s1_light_b);
                }

                // compute multiple-scatter light
                real multi_r=0, multi_g=0, multi_b=0;
                const real sx_h_exp = 1;
                ((Color(450,450,500)+sunColor*(0.5/picogen::constants::pi)) * (1 / (1+pow(h,sx_h_exp)))).to_rgb (multi_r, multi_g, multi_b);

                // ambient
                real ambient_r = 0.0;//4.5;
                real ambient_g = 0.0;//3.5;
                real ambient_b = 0.0;//2.5;

                //
                const real d = 1.0*0.0;
                const real s1 = 0.5;
                const real sx = 2.0*0.006;
                Color final = Color(
                    diffuse_r*d + s1_light_r*s1 + multi_r*sx + ambient_r,
                    diffuse_g*d + s1_light_g*s1 + multi_g*sx + ambient_g,
                    diffuse_b*d + s1_light_b*s1 + multi_b*sx + ambient_b
                );
                preetham.atmosphereShade (final, final, ray, rs);

                real r, g, b;
                real ar, ag, ab;
                (sunSky).to_rgb (r,g,b);
                (final).to_rgb (ar,ag,ab);

                switch (0) {
                    case 0: {
                        //const real m = sat01 (1-h);//*/ 1 / exp(h);//(1+20*h);
                        const real m =  h > 0.25 ? 0 : (1-h*4.0);
                        const real A = m;
                        const real B = 1-m;

                        r = r * A + (ar)*B;
                        g = g * A + (ag)*B;
                        b = b * A + (ab)*B;
                    } break;
                    case 21:
                        r = normal[0]*0.5+0.5;
                        g = normal[1]*0.5+0.5;
                        b = normal[2]*0.5+0.5;
                        break;
                    case 22:
                        r = normal_[0]*0.5+0.5;
                        g = normal_[1]*0.5+0.5;
                        b = normal_[2]*0.5+0.5;
                        break;
                    /*case 231:
                        r = diff[0][0]*0.5+0.5;
                        g = diff[0][1]*0.5+0.5;
                        b = diff[0][2]*0.5+0.5;
                        break;
                    case 232:
                        r = diff[1][0]*0.5+0.5;
                        g = diff[1][1]*0.5+0.5;
                        b = diff[1][2]*0.5+0.5;
                        break;*/
                    case 24:
                        r = projectedSunDirection[0]*0.5+0.5;
                        g = projectedSunDirection[1]*0.5+0.5;
                        b = projectedSunDirection[2]*0.5+0.5;
                        break;
                    case 25:
                        r = (projectedSunPosition[0]/cloudPlaneHeight)*0.5+0.5;
                        g = (projectedSunPosition[1]/cloudPlaneHeight)*0.5+0.5;
                        b = (projectedSunPosition[2]/cloudPlaneHeight)*0.5+0.5;
                        break;
                    case 31:
                        r = g = b = dotProjected*0.5 + 0.5;
                        break;
                    case 32:
                        r = g = b = dotProjected;
                        break;
                    case 4:
                        r = g = b = h;
                        break;
                    case 5:
                        r = g = b = rs;
                        break;
                    case 6:
                        r = g = b = diffuseDot;
                        break;
                };
                return Color (r, g, b);
            }
        } // namespace objects
    } // namespace graphics
} // namespace picogen
