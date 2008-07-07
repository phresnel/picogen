/***************************************************************************
 *            TriBIH.h
 *
 *  2008-05-29 15:43
 *  Copyright  2008  Sebastian Mach (*1983)
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

// This file has been forked from graphics/objects/TriBIH.h


#ifndef _TEMPLATED__TRI_BIH_H
#define _TEMPLATED__TRI_BIH_H

#include <list>

namespace picogen {
    namespace graphics {
        namespace objects {
            namespace templates{


#define TPL_TriBIH_VisualizeLeafs 0
#define TPL_TriBIH_DebugBIHBuild 0
#define TPL_TriBIH_DebugBIHRecConvert 0

#define TriBIH_Template         template <class t_triangle>
#define TriBIH_Member( type )   TriBIH_Template type TriBIH<t_triangle>

                TriBIH_Template class TriBIH : public ::picogen::graphics::objects::abstract::ITriScene {
                    private:

                        typedef ::picogen::misc::prim::real real;
                        typedef ::picogen::misc::geometrics::Vector3d Vector3d;
                        typedef ::picogen::misc::geometrics::Ray Ray;
                        typedef ::picogen::misc::geometrics::BoundingBox BoundingBox;
                        typedef ::picogen::graphics::material::abstract::IBRDF IBRDF;
                        typedef ::picogen::graphics::material::abstract::IShader IShader;
                        typedef ::picogen::graphics::structs::intersection_t intersection_t;

                        const real tri_eps;

                        /*struct t_triangle {
                            Vector3d A,B,C;
                            t_triangle (Vector3d a, Vector3d b, Vector3d c) : A (a), B (b), C (c) {};
                            t_triangle() : A(), B(), C() {};
                        };*/
                        int raytri_intersect (
                            const Ray &ray,
                            const Vector3d &a, const Vector3d &b, const Vector3d &c,
                            real &t, real &u, real &v,
                            Vector3d &normal
                        ) const;

                        typedef typename std::list<t_triangle> t_trianglelist;
                        typedef typename std::list<t_triangle>::iterator t_trianglelist_iterator;
                        typedef typename std::list<t_triangle>::const_iterator t_trianglelist_const_iterator;

                        struct bih_node {
                            bool is_leaf;
    #if TriBIH_VisualizeLeafs
                            BoundingBox aabb;
    #endif
                            union {
                                struct {
                                    unsigned int triangles_begin;
                                    unsigned int triangles_end;
                                };
                                struct {
                                    unsigned int children;
                                    real clip[2];
                                    unsigned int axis;
                                };
                            };
                        };

                        class bih_build_node {
                                t_trianglelist triangles;
                                BoundingBox aabb;

                                bih_build_node *p_left, *p_right;
                                real split;
                                int axis;

                                bool rec_build (const BoundingBox &build_aabb, const unsigned int maxRec);
                                //size_t convert_size() const;
                                unsigned int num_nodes() const;
                                unsigned int num_triangles() const;
                                void rec_convert (
                                    bih_node *bih_root, t_triangle *triangles,
                                    unsigned int curr,
                                    unsigned int &bih_free, unsigned int &tri_free,
                                    unsigned int depth,
                                    unsigned int &max_tris_in_node, unsigned int &max_depth
                                ) const;
                            public:
                                bih_build_node() : triangles(), aabb(), p_left (0), p_right (0) {}
                                virtual ~bih_build_node() {
                                    free();
                                }
                                void insert (param_in (t_triangle,tri));
                                bool build (bih_node **bih, t_triangle **triangles);
                                void free();
                        };

                        BoundingBox AABB;
                        bih_build_node BIHBuild;
                        bih_node *BIH;
                        t_triangle *Triangles;
                        const IBRDF *BRDF;
                        const IShader *shader;

                        bool intersect (param_out (intersection_t,intersection), const bih_node *curr, real t_min, real t_max, param_in (Ray,ray)) const;

                    public:
                        TriBIH();
                        virtual ~TriBIH();
                        virtual void setBRDF (const IBRDF *brdf);
                        virtual void setShader (const IShader* shader);
                        virtual bool intersect (param_out (intersection_t,intersection), param_in (Ray,ray)) const;
                        virtual void insert (param_in (Vector3d,A), param_in (Vector3d,B), param_in (Vector3d,C));
                        virtual void flush();
                        virtual void invalidate();
                };

            } // namespace templates
        } // namespace picogen
    } // namespace graphics
} // namespace objects

#include "TriBIH.cc.h"


#undef TPL_TriBIH_VisualizeLeafs
#undef TPL_TriBIH_DebugBIHBuild
#undef TPL_TriBIH_DebugBIHRecConvert

#endif /* _TEMPLATED__TRI_BIH_H */
