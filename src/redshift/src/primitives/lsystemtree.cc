//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
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

#include <stack>
#include <stdexcept>
#include <boost/optional.hpp>

#include "../../include/constants.hh"
#include "../../include/basictypes/intersection.hh"
#include "../../include/texture/image.hh"
#include "../../include/primitives/lsystemtree.hh"
#include "../../include/primitives/triangle.hh"
#include "xyto/lsystem.hh"
#include "xyto/draw.hh"
boost::optional<LSystem> compile(const char*);



namespace redshift { namespace {
        class LSystemTreeMesher {
        public:
                LSystemTreeMesher (unsigned int slices,
                                   shared_ptr<ColorTexture> barkTexture,
                                   shared_ptr<ColorTexture> leafTexture);
                ~LSystemTreeMesher ();
                void moveTo (Turtle state);
                void drawTo (Turtle newState);
                void leaf();
                //GLuint displayList() const;
                //std::vector<GLuint> textures() const;
                void pushState();
                void popState();

                std::vector<redshift::primitive::Triangle> triangles;
        private:
                Turtle state;
                std::stack<Turtle> stateStack;
                unsigned int slices;

                shared_ptr<ColorTexture> barkTexture;
                shared_ptr<ColorTexture> leafTexture;
                //GLuint displayList_;
                //std::vector<GLuint> textures_;
        };



        LSystemTreeMesher::LSystemTreeMesher (unsigned int slices,
                                        shared_ptr<ColorTexture> barkTexture,
                                        shared_ptr<ColorTexture> leafTexture)
        : slices(slices), barkTexture(barkTexture), leafTexture(leafTexture)
        {
                //textures_.push_back(loadTexture("bark.jpg"));
                //textures_.push_back(loadTexture("leaf.png"));

                //displayList_ = glGenLists(1);
                //glNewList(displayList_, GL_COMPILE);
                //glBindTexture(GL_TEXTURE_2D, textures_[0]);
                //glEnable(GL_CULL_FACE);
        }



        LSystemTreeMesher::~LSystemTreeMesher () {
                //glEndList();
        }



        void LSystemTreeMesher::moveTo (Turtle state) {
                this->state = state;
        }



        void LSystemTreeMesher::drawTo (Turtle newState) {
                using std::fabs;
                using std::acos;
                using redshift::primitive::Triangle;
                typedef Triangle::Vertex Vertex;
                typedef Triangle::TextureCoordinates TexCoords;
                using namespace redshift;

                const double pi = 3.14159, pi2 = pi*2;


                const double fdot = dot (state.rotation.forward(),
                                         newState.rotation.forward());
                if (fabs(fdot) < 0.999) {
                        const TurtleVector axis =
                                        normalize(cross(
                                                state.rotation.forward(),
                                                newState.rotation.forward()));
                        const double angle = acos(fdot);
                        newState.rotation = state.rotation
                                            *
                                            TurtleMatrix::Rotate(angle, axis)
                                            ;
                } else {
                        newState.rotation = state.rotation;
                }

                const double oldTexV = state.pathLength    * 0.1,
                             newTexV = newState.pathLength * 0.1;

                //glBegin(GL_QUAD_STRIP);
                Point prevOldV, prevNewV;
                TexCoords prevOldTC, prevNewTC;
                for (unsigned int i=0; i<=slices; ++i) {
                        const double f = i / (float)slices;
                        const double phi = f * pi2;
                        const TurtleVector oldV_ = state.disk(phi) * state.scale;
                        const TurtleVector newV_ = newState.disk(phi) * newState.scale;
                        const TurtleVector oldN_ = state.normal(phi);
                        const TurtleVector newN_ = newState.normal(phi);

                        const Point oldV (oldV_.x, oldV_.y, oldV_.z);
                        const Point newV (newV_.x, newV_.y, newV_.z);

                        const TexCoords oldTC(1-f, oldTexV);
                        const TexCoords newTC(1-f, newTexV);

                        if (i>0) {
                                triangles.push_back(Triangle(
                                        Vertex(prevOldV, prevOldTC),
                                        Vertex(prevNewV, prevNewTC),
                                        Vertex(oldV, oldTC),
                                        barkTexture
                                ));
                                triangles.push_back(Triangle(
                                        Vertex(prevNewV, prevNewTC),
                                        Vertex(newV, newTC),
                                        Vertex(oldV, oldTC),
                                        barkTexture
                                ));
                        }
                        prevOldV = oldV;
                        prevNewV = newV;
                        prevOldTC = oldTC;
                        prevNewTC = newTC;
                }
                //glEnd();

                state = newState;
        }



        void LSystemTreeMesher::leaf() {
                using redshift::primitive::Triangle;
                typedef Triangle::Vertex Vertex;
                typedef Triangle::TextureCoordinates TexCoords;
                using namespace redshift;

                Turtle state = this->state;
                state.rollToVertical();
                state.rollLeft(0.3*((rand()/(double)RAND_MAX)-0.5));
                state.up(0.1*((rand()/(double)RAND_MAX)-0.5));

                TurtleVector head = state.heading();
                TurtleVector right = state.right();

                const double s = 40;
                const TurtleVector
                                A = (state.position + right*-s) * state.scale,
                                B = (state.position + right*-s + head*s) * state.scale,
                                C = (state.position + right*s + head*s) * state.scale,
                                D = (state.position + right*s) * state.scale;

                triangles.push_back(Triangle(
                        Vertex(Point(A.x, A.y, A.z), TexCoords(0,1)),
                        Vertex(Point(B.x, B.y, B.z), TexCoords(0,0)),
                        Vertex(Point(C.x, C.y, C.z), TexCoords(1,0)),
                        leafTexture
                ));
                triangles.push_back(Triangle(
                        Vertex(Point(A.x, A.y, A.z), TexCoords(0,1)),
                        Vertex(Point(C.x, C.y, C.z), TexCoords(1,0)),
                        Vertex(Point(D.x, D.y, D.z), TexCoords(1,1)),
                        leafTexture
                ));
        }



        //GLuint LSystemTreeMesher::displayList() const {
        //        return displayList_;
        //}



        //std::vector<GLuint> LSystemTreeMesher::textures() const {
        //        return textures_;
        //}



        void LSystemTreeMesher::pushState() {
                stateStack.push(state);
        }



        void LSystemTreeMesher::popState() {
                state = stateStack.top();
                stateStack.pop();
        }

} }



namespace redshift { namespace primitive {


//-----------------------------------------------------------------------------
// Bvh
//-----------------------------------------------------------------------------
LSystemTree::LSystemTree(
        const char *code,
        unsigned int level,
        unsigned int slicesPerSegment
)
{
        using redshift::primitive::Triangle;

        LSystem lsys;
        Pattern pat;
        const boost::optional<LSystem> newLsys = compile(code);
        if (!newLsys) {
                lsys = LSystem();
                pat = Pattern();
                throw std::runtime_error("invalid l-system");;
        }
        lsys = *newLsys;
        pat = lsys.run(level);

        LSystemTreeMesher mesher(slicesPerSegment,
                                 shared_ptr<ColorTexture>(new texture::ColorImage("bark.bmp")),
                                 shared_ptr<ColorTexture>(new texture::ColorImage("leaf.bmp")));
        draw(lsys, pat, Turtle(), mesher);

        TriangleBvhBuilder builder;
        typedef std::vector<Triangle>::const_iterator It;
        for (It it=mesher.triangles.begin(); it!=mesher.triangles.end(); ++it){
                builder.add (*it);
        }

        mesh = builder.toTriangleBvh();
}



LSystemTree::~LSystemTree() {
}



BoundingBox LSystemTree::boundingBox() const {
        return mesh->boundingBox();
}



bool LSystemTree::doesIntersect (Ray const &ray) const {
        return mesh->doesIntersect(ray);
}



optional<Intersection> LSystemTree::intersect(Ray const &ray) const {
        return mesh->intersect(ray);
}

} }
