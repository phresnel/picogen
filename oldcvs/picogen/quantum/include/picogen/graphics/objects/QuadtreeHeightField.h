/***************************************************************************
 *            QuadtreeHeightField.h
 *
 *  2008-08-08
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


#ifndef _QUADTREEHEIGHTFIELD_H
#define _QUADTREEHEIGHTFIELD_H

// #include <cstdint> // C++0x
#include <stdint.h>

#include <picogen/graphics/objects.h>


namespace picogen {
    namespace graphics {
        namespace objects {
            class QuadtreeHeightField : public ::picogen::graphics::objects::abstract::IIntersectable {
                private:

                    // Typedefs.
                    typedef ::picogen::misc::prim::real real;
                    typedef ::picogen::misc::geometrics::Vector3d Vector3d;
                    typedef ::picogen::misc::geometrics::Ray Ray;
                    typedef ::picogen::misc::geometrics::BoundingBox BoundingBox;
                    typedef ::picogen::graphics::material::abstract::IBRDF IBRDF;
                    typedef ::picogen::graphics::material::abstract::IShader IShader;
                    typedef ::picogen::graphics::structs::intersection_t intersection_t;
                    typedef ::picogen::misc::functional::Function_R2_R1 Function_R2_R1;

                private:


                    // Types.

                    struct Height {
                        enum {
                            min = 0,
                            max = 0xFFFF
                        };
                        uint16_t h;

                        real asReal () const {
                            return static_cast<real> (h) / static_cast<real> (max);
                        }

                        real asReal (real min, real max)  const {
                            return min + (static_cast<real> (h) / static_cast<real> (Height::max)) * (max-min);
                        }

                        void fromReal (real h) {
                            int32_t tmp = static_cast<int32_t> (h * static_cast<real> (max));
                            if (tmp < static_cast<int32_t>(min)) {
                                //std::cout << "<u" << tmp << "f" << h;
                                tmp=static_cast<int32_t>(min);
                            }
                            if (tmp > static_cast<int32_t>(max)) {
                                //std::cout << ">u" << tmp << "f" << h;
                                tmp=static_cast<int32_t>(max);
                            }
                            this->h = static_cast<uint16_t>(tmp);
                        }
                    };

                    struct Chunk {
                        enum {
                            size = 3
                        };
                        Height field [size * size];
                        Height h_min, h_max;
                    };

                    struct Node {
                        Chunk chunk;
                        Node *children;

                        Node () : chunk(), children (0) {}
                        ~Node () {
                            if (0 != children)
                                delete [] children;
                        }
                    };


                    // Fields.
                    BoundingBox bbox;
                    real bboxSize [3], bboxMin [3];
                    unsigned int heightFieldSize;
                    const IBRDF *brdf;
                    const IShader *shader;

                    Node rootNode;


                    // Private Methods.
                    void initNode (
                        QuadtreeHeightField::Node *node, QuadtreeHeightField::Node *parent,
                        unsigned int left, unsigned int top, unsigned int size, const unsigned int minSize, const unsigned int heightFieldSize,
                        bool smooth,
                        const Function_R2_R1 &fun, real fun_min, real fun_max,
                        real percentageFinished, real percentageFinishedScale
                    ) ;

                    bool intersectNode (
                        param_out (intersection_t, intersection),
                        param_in (Ray, ray), param_in (QuadtreeHeightField::Node, node),
                        const unsigned int left, const unsigned int top, const unsigned int size
                    ) const;


                    // Constructors.
                    explicit QuadtreeHeightField (
                        unsigned int size,
                        const ::picogen::misc::functional::Function_R2_R1 &fun,
                        const BoundingBox &bbox,
                        bool smooth,
                        real h_min, real h_max
                    );


                    // Helpers.
                    static inline real smoothedHeightFunc (const Function_R2_R1 &fun, real u, real v, real cellSizeX, real cellSizeY);

                public:

                    // Public Methods.
                    virtual ~QuadtreeHeightField();

                    virtual bool intersect (param_out (intersection_t, intersection), param_in (Ray, ray)) const;
                    static QuadtreeHeightField * create (unsigned int size, const Function_R2_R1 &fun, param_in (BoundingBox, bbox), real boundsGuessAccuracy = 1.0, bool smooth = true);

                    //void setBox (param_in (Vector3d, min), param_in (Vector3d, max));
                    void setBRDF (const IBRDF* brdf);
                    void setShader (const IShader* shader);

            };

        } // namespace objects {
    } // namespace graphics {
} // namespace picogen {

#if 0
namespace picogen {
    namespace graphics {
        namespace objects {

            class SimpleHeightField;

            class QuadtreeHeightField : public ::picogen::graphics::objects::abstract::IIntersectable {
                private:
                    typedef ::picogen::misc::prim::real real;
                    typedef ::picogen::misc::geometrics::Vector3d Vector3d;
                    typedef ::picogen::misc::geometrics::Ray Ray;
                    typedef ::picogen::misc::geometrics::BoundingBox BoundingBox;
                    typedef ::picogen::graphics::material::abstract::IBRDF IBRDF;
                    typedef ::picogen::graphics::material::abstract::IShader IShader;
                    typedef ::picogen::graphics::structs::intersection_t intersection_t;

                private:
                    SimpleHeightField * const field;
                    const BoundingBox rootBox;

                    // Mission 1 is to write a straightforward implementation
                    // that is easy to debug and maintain and port.
                    class QuadtreeNode {

                        // Layout of the children:
                        //   0 1   or   0+0 0+1
                        //   2 3        1+0 1+1
                        private:
                            QuadtreeNode *children;
                            BoundingBox boundingBox;
                            Vector3d center;

                            unsigned int left, top, right, bottom;

                            // This should allow for easy bottom-up construction.
                            explicit QuadtreeNode (
                                QuadtreeNode *children, const BoundingBox &boundingBox,
                                unsigned int left, unsigned int top, unsigned int right, unsigned int bottom
                            ) : children (children), boundingBox (boundingBox), center (boundingBox.computeCenter()),
                                left (left), top (top), right (right), bottom (bottom)
                            {}

                            explicit QuadtreeNode () : children (0), boundingBox() {}

                        public:

                            ~QuadtreeNode () {
                                if (0 != children)
                                    delete [] children;
                            }

                            static QuadtreeNode *create () {
                                return new QuadtreeNode ();
                            }

                            static QuadtreeNode * create (
                                QuadtreeNode *children, const BoundingBox &boundingBox,
                                unsigned int left, unsigned int top, unsigned int right, unsigned int bottom
                            ) {
                                return new QuadtreeNode (children, boundingBox, left, top, right, bottom);
                            }

                            static QuadtreeNode * createArray (size_t size) {
                                return new QuadtreeNode [4] ();
                            }

                            /*static const QuadtreeNode createTemporal (QuadtreeNode *children, const BoundingBox &boundingBox) {
                                return QuadtreeNode (children, boundingBox);
                            }*/

                            void setChildren (QuadtreeNode *children) {
                                this->children = children;
                            }

                            void setBoundingBox (const BoundingBox &boundingBox) {
                                this->boundingBox = boundingBox;
                                this->center = boundingBox.computeCenter();
                            }

                            void setRectangle (unsigned int left, unsigned int top, unsigned int right, unsigned int bottom) {
                                this->left   = left;
                                this->top    = top;
                                this->right  = right;
                                this->bottom = bottom;
                            }

                            const QuadtreeNode & operator = (const QuadtreeNode & node) {
                                children = node.children;
                                boundingBox = node.boundingBox;
                                center = node.boundingBox.computeCenter();
                                return *this;
                            }

                            const QuadtreeNode & operator [] (unsigned int u) const {
                                return children [u];
                            }

                            QuadtreeNode & operator [] (unsigned int u) {
                                return children [u];
                            }

                        private:

                        public:

                            bool intersect (
                                param_out (intersection_t, intersection),
                                param_in (SimpleHeightField, field),
                                param_in (Ray, ray)
                            ) const {
                                real t_min, t_max;
                                if (!boundingBox.intersect (t_min, t_max, ray))
                                    return false;
                                // Saturate t_min to 0; maybe return.
                                t_min = t_min < 0 ? 0 : t_min;
                                if (t_min > t_max)
                                    return false;

                                switch (0 == children) {
                                    case false: {

                                        /*const real tx = (ray.x()[0] - center[0]) / ray.w()[0];
                                        const real tz = (ray.x()[2] - center[2]) / ray.w()[2];
                                        const bool phi = tx > tz;*/


                                        /*
                                        unsigned int order[4];

                                        const bool isInNegX = ray.x()[0] < center[0];
                                        const bool isInNegZ = ray.x()[2] < center[2];

                                        if (isInNegX && isInNegZ) {
                                            order [0] = 2;
                                            order [1] = 0;
                                            order [2] = 3;
                                            order [3] = 1;
                                        } else if (isInNegX && !isInNegZ) {
                                            order [0] = 0;
                                            order [1] = 1;
                                            order [2] = 2;
                                            order [3] = 3;
                                        } else if (!isInNegX && isInNegZ) {
                                            order [0] = 3;
                                            order [1] = 2;
                                            order [2] = 1;
                                            order [3] = 0;
                                        } else if (!isInNegX && !isInNegZ) {
                                            order [0] = 1;
                                            order [1] = 3;
                                            order [2] = 0;
                                            order [3] = 2;
                                        }

                                        intersection.t = ::picogen::misc::constants::real_max;
                                        intersection_t tmp;
                                        unsigned int numIntersections = 0;

                                        for (unsigned int u=0; u<sizeof (order) / sizeof(order[0]); ++u) {
                                            bool doesIntersect = children [order [u]].intersect (tmp, field, ray);
                                            numIntersections += doesIntersect ? 1 : 0;
                                            intersection = (doesIntersect && tmp.t < intersection.t) ? tmp : intersection;
                                            if (doesIntersect)
                                                break;
                                        }


                                        */
                                        intersection.t = ::picogen::misc::constants::real_max;
                                        intersection_t tmp;
                                        for (unsigned int u=0; u<4; ++u) {
                                            bool doesIntersect = children [u].intersect (tmp, field, ray);
                                            doesIntersect = doesIntersect && tmp.t >= t_min && tmp.t <= t_max;
                                            intersection = (doesIntersect && tmp.t < intersection.t) ? tmp : intersection;
                                            //if (doesIntersect)
                                              //  break;
                                        }

                                        return intersection.t != ::picogen::misc::constants::real_max;
                                    } break;
                                    case true: {


                                        bool doesIntersect = field.intersect (intersection, ray, t_min, t_max);
                                        //bool doesIntersect = field.intersectRectangle (intersection, ray, left, top, right, bottom);

                                        if (doesIntersect) {
                                            srand ((unsigned int)(void*)(this));
                                            intersection.color = ::picogen::graphics::image::color::Color (
                                                (real)rand() / (real)RAND_MAX,
                                                (real)rand() / (real)RAND_MAX,
                                                (real)rand() / (real)RAND_MAX
                                            );
                                            intersection.normal = Vector3d (0,1,0);
                                            intersection.L_e = 0.3;
                                            return true;
                                        }
                                        return doesIntersect;
                                    } break;
                                };
                                return false;
                            }

                    };

                    //QuadtreeNode *buildChildren (unsigned int u, unsigned int v, unsigned int size, const unsigned int maxSize, const BoundingBox &rootBox) const;
                    QuadtreeNode *buildChildren (unsigned int u, unsigned int v, unsigned int size, const unsigned int maxSize, const BoundingBox &rootBox, BoundingBox *aggregateBB) const;
                    QuadtreeNode *rootNode;


                public:
                    QuadtreeHeightField (unsigned int maxSize, SimpleHeightField *simpleHeightField);
                    virtual ~QuadtreeHeightField();
                    virtual bool intersect (param_out (intersection_t, intersection), param_in (Ray, ray)) const;
            };

        } // namespace objects {
    } // namespace graphics {
} // namespace picogen {
#endif // #if 0


#endif /* _QUADTREEHEIGHTFIELD_H */
