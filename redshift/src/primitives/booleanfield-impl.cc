//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2009  Sebastian Mach (*1983)
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

namespace impl {

        typedef quatsch::backend::est::Backend <redshift::real_t, const redshift::real_t *> backend_t;
        typedef backend_t::Function Function;
        typedef backend_t::FunctionPtr FunctionPtr;
        typedef backend_t::scalar_t scalar_t;
        typedef backend_t::parameters_t parameters_t;
        typedef quatsch::frontend::jux::Compiler <backend_t> Compiler;

        Compiler::ConfigurableFunctionsMap addfuns() {
                using namespace redshift;


                quatsch::ICreateConfigurableFunction<Function>::ConfigurableFunctionDescriptionPtr noiseDesc (
                        new quatsch::CreateConfigurableFunction <
                                quatsch :: configurable_functions :: Noise2d <Function, Compiler>,
                                Function
                        >
                );
                quatsch::ICreateConfigurableFunction<Function>::ConfigurableFunctionDescriptionPtr layeredNoise2dDesc (
                        new quatsch::CreateConfigurableFunction <
                                quatsch :: configurable_functions :: LayeredNoise2d <Function, Compiler>,
                                Function
                        >
                );

                Compiler::ConfigurableFunctionsMap addfuns;
                addfuns.addSymbol ("Noise2d", noiseDesc);
                addfuns.addSymbol ("LayeredNoise2d", layeredNoise2dDesc);
                return addfuns;
        }

        struct Rgb {
                float r,g,b;
        };

        struct Point {
                float x,y,z;
                float operator [] (size_t i) const {
                        switch (i) {
                        case 0: return x;
                        case 1: return y;
                        case 2: return z;
                        };
                        throw;
                }
        };

        struct Vector {
                float x,y,z;
                float operator [] (size_t i) const {
                        switch (i) {
                        case 0: return x;
                        case 1: return y;
                        case 2: return z;
                        };
                        throw;
                }
        };

        Vector cross (Vector const &lhs, Vector const &rhs) {
                const Vector ret = {
                        lhs.y*rhs.z - lhs.z*rhs.y,
                        lhs.z*rhs.x - lhs.x*rhs.z,
                        lhs.x*rhs.y - lhs.y*rhs.x,
                };
                return ret;
        }

        float length (Vector const &vec) {
                return sqrt (vec.x*vec.x+vec.y*vec.y+vec.z*vec.z);
        }
        float length_sqr (Vector const &vec) {
                return vec.x*vec.x+vec.y*vec.y+vec.z*vec.z;
        }
        Vector operator * (Vector const & lhs, float rhs) {
                const Vector ret = {
                        lhs.x * rhs,
                        lhs.y * rhs,
                        lhs.z * rhs
                };
                return ret;
        }
        Vector operator * (float lhs, Vector const & rhs) {
                return rhs*lhs;
        }

        float operator * (Vector const & lhs, Vector const & rhs) {
                return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
        }

        Vector operator - (Vector const & lhs, Vector const & rhs) {
                const Vector ret = {lhs.x-rhs.x, lhs.y-rhs.y, lhs.z-rhs.z};
                return ret;
        }
        Vector operator + (Vector const & lhs, Vector const & rhs) {
                const Vector ret = {lhs.x+rhs.x, lhs.y+rhs.y, lhs.z+rhs.z};
                return ret;
        }

        Vector normalize (Vector const & vec) {
                return vec * (1.f/length(vec));
        }
        Vector inverse (Vector const &vec) {
                const Vector ret = {1.f / vec.x, 1.f / vec.y, 1.f / vec.z};
                return ret;
        }

        Vector random_perpendicular (Vector const &lhs) {
                Vector t;
                do {
                        t.x = rand() / (RAND_MAX+1.f) - .5f;
                        t.y = rand() / (RAND_MAX+1.f) - .5f;
                        t.z = rand() / (RAND_MAX+1.f) - .5f;
                        t = normalize (t);
                } while (lhs*t >= .99f);
                return cross (lhs, t);
        }


        Point operator + (Point const &lhs, Vector const &rhs) {
                const Point ret = {
                        lhs.x + rhs.x,
                        lhs.y + rhs.y,
                        lhs.z + rhs.z,
                };
                return ret;
        }

        Point & operator += (Point &lhs, Vector const &rhs) {
                lhs.x += rhs.x;
                lhs.y += rhs.y;
                lhs.z += rhs.z;
                return lhs;
        }

        Point operator - (Point const &lhs, Vector const &rhs) {
                const Point ret = {
                        lhs.x - rhs.x,
                        lhs.y - rhs.y,
                        lhs.z - rhs.z,
                };
                return ret;
        }

        Vector operator - (Point const &lhs, Point const &rhs) {
                const Vector ret = {
                        lhs.x - rhs.x,
                        lhs.y - rhs.y,
                        lhs.z - rhs.z,
                };
                return ret;
        }

        Vector operator - (Vector const &rhs) {
                const Vector ret = {
                        -rhs.x,
                        -rhs.y,
                        -rhs.z,
                };
                return ret;
        }



        struct Ray {
                Point position;
                Vector direction;
        };
        Point operator * (Ray const & ray, float f) {
                return ray.position + ray.direction * f;
        }

        struct FastRay {
                const Point position;
                const Vector direction;
                const Vector direction_recip;
                const int dx_positive;
                const int dy_positive;
                const int dz_positive;
                const int dx_negative;
                const int dy_negative;
                const int dz_negative;

                FastRay (Ray const &ray)
                : position(ray.position)
                , direction(ray.direction)
                , direction_recip(inverse(ray.direction))
                , dx_positive(ray.direction.x>=0)
                , dy_positive(ray.direction.y>=0)
                , dz_positive(ray.direction.z>=0)
                , dx_negative(ray.direction.x<0)
                , dy_negative(ray.direction.y<0)
                , dz_negative(ray.direction.z<0)
                {}

                FastRay (FastRay const &ray)
                : position(ray.position)
                , direction(ray.direction)
                , direction_recip(ray.direction_recip)
                , dx_positive(ray.dx_positive)
                , dy_positive(ray.dy_positive)
                , dz_positive(ray.dz_positive)
                , dx_negative(ray.dx_negative)
                , dy_negative(ray.dy_negative)
                , dz_negative(ray.dz_negative)
                {}
        private:
                FastRay();
                FastRay & operator = (FastRay const &);
        };
        Point operator * (FastRay const & ray, float f) {
                return ray.position + ray.direction * f;
        }


        struct Intersection {
                float distance;
                Rgb color;
                Vector normal;
        };

        inline float sat (float f) {
                return f<.0f?.0f:f>1.f?1.f:f;
        }



        /*
        typedef quatsch::backend::est::Backend <redshift::real_t, const redshift::real_t *> backend_t;
        typedef backend_t::Function Function;
        typedef backend_t::FunctionPtr FunctionPtr;
        typedef backend_t::scalar_t scalar_t;
        typedef backend_t::parameters_t parameters_t;
        typedef quatsch::frontend::jux::Compiler <backend_t> Compiler;

        Compiler::ConfigurableFunctionsMap addfuns() {
                using namespace redshift;


                quatsch::ICreateConfigurableFunction<Function>::ConfigurableFunctionDescriptionPtr noiseDesc (
                        new quatsch::CreateConfigurableFunction <
                                quatsch :: configurable_functions :: Noise2d <Function, Compiler>,
                                Function
                        >
                );
                quatsch::ICreateConfigurableFunction<Function>::ConfigurableFunctionDescriptionPtr layeredNoise2dDesc (
                        new quatsch::CreateConfigurableFunction <
                                quatsch :: configurable_functions :: LayeredNoise2d <Function, Compiler>,
                                Function
                        >
                );

                Compiler::ConfigurableFunctionsMap addfuns;
                addfuns.addSymbol ("Noise2d", noiseDesc);
                addfuns.addSymbol ("LayeredNoise2d", layeredNoise2dDesc);
                return addfuns;
        }*/

        inline bool fun (float x, float y, float z) {
                #if 0
                y+=450;
                z -= 500;
                using namespace redshift;
                typedef quatsch::backend::est::Backend <real_t, const real_t *> backend_t;

                typedef backend_t::Function Function;
                typedef backend_t::FunctionPtr FunctionPtr;
                typedef backend_t::scalar_t scalar_t;
                typedef backend_t::parameters_t parameters_t;

                typedef quatsch::frontend::jux::Compiler <backend_t> Compiler;

                static Compiler::FunctionPtr fun (Compiler::compile (std::string("x;y"),
                        //std::string("([LayeredNoise2d frequency{1.0} filter{cosine} layercount{5} seed{41} persistence{0.52}] x y)"),
                        std::string(
                                /*"(* 5 (- 1 (abs "
                                "([LayeredNoise2d frequency{0.5} filter{bilinear} layercount{4} seed{45} persistence{0.5}] "
                                " ([LayeredNoise2d frequency{0.5} filter{bilinear} layercount{4} seed{41} persistence{0.5}] x y) "
                                " ([LayeredNoise2d frequency{0.5} filter{bilinear} layercount{4} seed{41} persistence{0.5}] x y) "
                                ")"
                                ")))"*/
                                "(- 1 (abs ([LayeredNoise2d filter{cosine} seed{12} frequency{0.25} layercount{9} persistence{0.54} levelEvaluationFunction{(abs h)}] "
                                "  (+ y (^ (abs ([LayeredNoise2d filter{cosine} seed{12} frequency{0.25} layercount{12} persistence{0.54} levelEvaluationFunction{(abs h)}] x y)) 4))"
                                "  (+ y (^ (abs ([LayeredNoise2d filter{cosine} seed{12} frequency{0.25} layercount{12} persistence{0.54} levelEvaluationFunction{(abs h)}] x y)) 4))"
                                ")))"
                        ),
                        addfuns(), std::cerr));
                const real_t p[] = { .0025*x, .0025*z };
                return y < 600*(*fun)(p);

                #elif 0
                y += 3;
                z -= 15;
                const bool a = y < -2 + (0.5f * sin(x*3) * sin(z*3));
                const bool b = sqrt (x*x + (y)*(y) + z*z)<1.0f;
                const bool c = (sqrt(pow (x+2.f, 2.f)+pow (z-2.f, 2.f))<3.f)
                                & (y>2.0f) & (y<2.2f);
                const bool d = (sqrt(pow (x+2.f, 2.f)+pow (z-2.f, 2.f))<1.f)
                                & (y<2.0f);

                return a | b | c | d;
                #elif 1
                y += 1;
                z -= 10;
                const bool a = y < -2 + (0.5f * sin(x*3) * sin(z*3));
                const bool b = sqrt (x*x + (y)*(y) + z*z)<1.0f;

                const bool c = (sqrt(pow (x+2, 2.f)+pow (z-2, 2.f))<3.f)
                                & (y>2.0f) & (y<2.2f);


                const float xxx = x+2;
                const float zzz = z-2;
                const float sintmp = sin(4*y);
                const float costmp = cos(4*y);
                const float xx = xxx+0.25f*sintmp;
                const float zz = zzz+0.25f*costmp;
                const bool d = (sqrt(pow (xx, 2.f)+pow (zz, 2.f))<1.f)
                                & (y<2.0f);

                return a | b | c | d;
                #else
                return y<0;
                #endif
        }

        inline bool fun (Point const &pos) {
                return fun (pos.x, pos.y, pos.z);
        }



        // stupid code duplication
        bool ray_box (const FastRay &r, const Point &c, Vector const & size, float &tmin, float &tmax) {
                const float t0=0, t1=1000000.f;
                float tymin, tymax, tzmin, tzmax;

                const Vector size05 = size * .5f;
                const Point parameters [2] = {c - size05, c + size05};

                tmin = (parameters[r.dx_negative].x - r.position.x) * r.direction_recip.x;
                tmax = (parameters[r.dx_positive].x - r.position.x) * r.direction_recip.x;
                tymin = (parameters[r.dy_negative].y - r.position.y) * r.direction_recip.y;
                tymax = (parameters[r.dy_positive].y - r.position.y) * r.direction_recip.y;
                if ( (tmin > tymax) | (tymin > tmax) )
                        return false;
                if (tymin > tmin)
                        tmin = tymin;
                if (tymax < tmax)
                        tmax = tymax;
                tzmin = (parameters[r.dz_negative].z - r.position.z) * r.direction_recip.z;
                tzmax = (parameters[r.dz_positive].z - r.position.z) * r.direction_recip.z;
                if ( (tmin > tzmax) | (tzmin > tmax) )
                        return false;
                if (tzmin > tmin)
                        tmin = tzmin;
                if (tzmax < tmax)
                        tmax = tzmax;
                return ( (tmin <= t1) & (tmax >= t0) );
        }


        /*const Vector cc [8] = {
                { -1, -1, -1 },
                {  1, -1, -1 },
                { -1,  1, -1 },
                {  1,  1, -1 },
                { -1, -1,  1 },
                {  1, -1,  1 },
                { -1,  1,  1 },
                {  1,  1,  1 },
        };*/
        const Vector cca [3][2] = {
                {
                        { -1, 0, 0 },
                        { +1, 0, 0 }
                },
                {
                        { 0, -1, 0 },
                        { 0, +1, 0 }
                },
                {
                        { 0, 0, -1 },
                        { 0, 0, +1 }
                },
        };
        class BooleanFieldImpl {

                mutable Random rand;

                /*float size;
                float rsize;*/
                Vector size, rsize, center;

                //size_t sampleGridSize = 128;
                //bool sampleGrid[sampleGridSize*sampleGridSize*sampleGridSize];
                inline bool is_empty (Point const &c, Vector const &size, Point const &watcher) const {
                        /*const float
                                left   = (c.x - size.x * .5f) * rsize + .5f,
                                right  = (c.x + size.x * .5f) * rsize + .5f,
                                front  = (c.y - size.y * .5f) * rsize + .5f,
                                back   = (c.y + size.y * .5f) * rsize + .5f,
                                bottom = (c.z - size.z * .5f) * rsize + .5f,
                                top    = (c.z + size.z * .5f) * rsize + .5f
                        ;
                        const size_t
                                sleft   = size_t(left * sampleGridSize),
                                sright  = size_t(right * sampleGridSize),
                                sfront  = size_t(front * sampleGridSize),
                                sback   = size_t(back * sampleGridSize),
                                sbottom = size_t(bottom * sampleGridSize),
                                stop    = size_t(top * sampleGridSize)
                        ;

                        if (sright-sleft>0 && sback-sfront>0 && sbottom-stop>0) {
                                for (size_t u=sleft; u<sright; ++u)
                                for (size_t v=sfront; v<sback; ++v)
                                for (size_t w=sbottom; w<stop; ++w) {
                                        const Sample &sample = sampleGrid [
                                                u*sampleGridSize*sampleGridSize
                                                +v*sampleGridSize
                                                +w];
                                        if (sample.state == Sample::nonempty) {
                                                return false;
                                        }
                                }
                        } else*/ {
                                const int count = min (
                                        1000.f,
                                        ((500+10*size.x*size.y*size.z)/6)
                                        #warning make this customizable
                                        // / (1+length (watcher-c))
                                );
                                const bool ref = fun (c.x, c.y, c.z);
                                for (int i=0; i<count; ++i) {
                                        #warning tbh i am unsure why *2.f gives good results
                                        const float s[] = {
                                                rand()*1.5f-.75f,
                                                rand()*1.5f-.75f,
                                                rand()*1.5f-.75f
                                        };
                                        if (ref!=fun (c.x + size.x * s[0], c.y + size.y * s[1], c.z + size.z * s[2])) return false;
                                        if (ref!=fun (c.x + size.x * s[0], c.y + size.y * s[2], c.z + size.z * s[1])) return false;
                                        if (ref!=fun (c.x + size.x * s[1], c.y + size.y * s[0], c.z + size.z * s[2])) return false;
                                        if (ref!=fun (c.x + size.x * s[1], c.y + size.y * s[2], c.z + size.z * s[0])) return false;
                                        if (ref!=fun (c.x + size.x * s[2], c.y + size.y * s[0], c.z + size.z * s[1])) return false;
                                        if (ref!=fun (c.x + size.x * s[2], c.y + size.y * s[1], c.z + size.z * s[0])) return false;
                                }
                        }
                        return true;

                }
                /*
                void init_grid () {
                        for (size_t u=0; u<sampleGridSize; ++u)
                        for (size_t v=0; v<sampleGridSize; ++v)
                        for (size_t w=0; w<sampleGridSize; ++w) {
                                Sample &sample = sampleGrid [
                                                        u*sampleGridSize*sampleGridSize
                                                        +v*sampleGridSize
                                                        +w];

                                const float
                                        left = (u/(float)sampleGridSize)*this->size - this->size*.5f,
                                        right = ((1+u)/(float)sampleGridSize)*this->size - this->size*.5f,
                                        front = (v/(float)sampleGridSize)*this->size - this->size*.5f,
                                        back = ((1+v)/(float)sampleGridSize)*this->size - this->size*.5f,
                                        bottom = (w/(float)sampleGridSize)*this->size - this->size*.5f,
                                        top = ((1+w)/(float)sampleGridSize)*this->size - this->size*.5f
                                ;
                                const Point c = { (left+right)*.5f, (front+back)*.5f,  (bottom+top)*.5f };
                                const Vector size = { right-left, back-front, top-bottom };
                                const int count = (500+10*size.x*size.y*size.z)/6;
                                const bool ref = fun (c.x, c.y, c.z);
                                sample.state = Sample::empty;
                                for (int i=0; i<count; ++i) {
                                        #warning tbh i am unsure why *2.f gives good results
                                        const float s[] = {
                                                mt.rand()*2-1,
                                                mt.rand()*2-1,
                                                mt.rand()*2-1
                                        };
                                        if (ref!=fun (c.x + size.x * s[0], c.y + size.y * s[1], c.z + size.z * s[2])) {
                                                sample.state = Sample::nonempty;
                                                break;
                                        } else if (ref!=fun (c.x + size.x * s[0], c.y + size.y * s[2], c.z + size.z * s[1])) {
                                                sample.state = Sample::nonempty;
                                                break;
                                        } else if (ref!=fun (c.x + size.x * s[1], c.y + size.y * s[0], c.z + size.z * s[2])) {
                                                sample.state = Sample::nonempty;
                                                break;
                                        } else if (ref!=fun (c.x + size.x * s[1], c.y + size.y * s[2], c.z + size.z * s[0])) {
                                                sample.state = Sample::nonempty;
                                                break;
                                        } else if (ref!=fun (c.x + size.x * s[2], c.y + size.y * s[0], c.z + size.z * s[1])) {
                                                sample.state = Sample::nonempty;
                                                break;
                                        }  else if (ref!=fun (c.x + size.x * s[2], c.y + size.y * s[1], c.z + size.z * s[0])) {
                                                sample.state = Sample::nonempty;
                                                break;
                                        }
                                }
                        }
                }*/


                struct Node {
                        enum State {
                                uninit,
                                empty,
                                nonempty,
                                nonempty_leaf

                                /*
                                inner,
                                nonempty_leaf,
                                empty_leaf
                                */
                        };
                        mutable State state;
                        mutable Node *children;
                        mutable int queryCount;


                        Node () : state (uninit), children(0), queryCount(0) {}
                        ~Node() { delete [] children; }

                        float intersect (
                                FastRay const &ray,
                                float tnear, float tfar,
                                Point const & c,
                                Vector const & size,
                                int axis,
                                BooleanFieldImpl const &field
                        ) const {
                                const float step = .0125f * (1 + length (ray.position-c)/10);

                                if (tnear > tfar)
                                        return -1;
                                if (uninit == state)
                                        state = field.is_empty (c, size, ray.position) ? empty : nonempty;
                                if (empty == state)
                                        return -1;
                                if (nonempty_leaf != state) {
                                        if (0 == children) {
                                                if (queryCount >= 20) {
                                                        const float min = (step * 5);// * (1+length (ray.position-c));
                                                        if ((size.x>min)|(size.y>min)|(size.z>min)) {
                                                                children = new Node[2];
                                                        } else {
                                                                state = nonempty_leaf;
                                                        }
                                                }
                                                ++queryCount;
                                        }
                                }

                                if (children) {
                                        const int nn[4] = {0,1,2,0};
                                        const int nextAxis = nn[axis+1];
                                        const int near = ray.direction[axis] < 0.f;
                                        const int far = !near;

                                        float s;
                                        Vector newSize = size;
                                        switch (axis) {
                                        case 0:
                                                s = (newSize.x *= 0.5f);
                                                break;
                                        case 1:
                                                s = (newSize.y *= 0.5f);
                                                break;
                                        case 2: default:
                                                s = (newSize.z *= 0.5f);
                                                break;
                                        };

                                        const float
                                                d = (c[axis] - ray.position[axis]) * ray.direction_recip[axis]
                                        ;

                                        const Point center_near = c+cca[axis][near]*s*.5f;
                                        const Point center_far = c+cca[axis][far]*s*.5f;

                                        if (d <= tnear) {
                                                return children[far].intersect (
                                                        ray, tnear, tfar,
                                                        center_far,
                                                        newSize,
                                                        nextAxis,
                                                        field
                                                );
                                        } else if (d >= tfar) {
                                                return children[near].intersect (
                                                        ray, tnear, tfar,
                                                        center_near,
                                                        newSize,
                                                        nextAxis,
                                                        field
                                                );
                                        }

                                        const float t = children[near].intersect (
                                                        ray, tnear, d,
                                                        center_near,
                                                        newSize,
                                                        nextAxis,
                                                        field
                                                );
                                        if ((t >= .0f) & (t <= d)) return t;
                                        else return children[far].intersect (
                                                        ray, d, tfar,
                                                        center_far,
                                                        newSize,
                                                        nextAxis,
                                                        field
                                                );
                                } else {
                                        const float
                                                begin=tnear-step,
                                                end=tfar+step
                                        ;
                                        float dist = begin;
                                        Point curr = ray*dist;
                                        const Vector vstep = ray.direction * step;
                                        const bool init_sign = fun (curr);
                                        bool curr_sign;
                                        do {
                                                dist += step;
                                                curr += vstep;
                                                curr_sign = fun (curr);
                                        } while((dist<=end) & (curr_sign==init_sign));
                                        if (init_sign != curr_sign)
                                                return dist;
                                }
                                return -1;
                        }
                };
                Node root;

        public:
                BooleanFieldImpl (float size)
                : /*sampleGridSize(128), sampleGrid(new Sample[sampleGridSize*sampleGridSize*sampleGridSize])*/
                 //size(size), rsize(1.f/size)
                 rand(12345,6789,01234,56789)
                , root ()
                {
                        Vector mins = {size/2,size/2,size/2}, maxs = {-size/2,-size/2,-size/2};
                        if (0) {
                                std::cerr << "determining bounding box\n";
                                const int count = min (50000.f, (5000+10*size*size*size)/6);
                                const bool refmin = fun (mins.x,mins.y,mins.z);
                                const bool refmax = fun (maxs.x,maxs.y,maxs.z);
                                for (int i=0; i<count; ++i) {
                                        #warning tbh i am unsure why *2.f gives good results
                                        const float s[] = {
                                                size*(rand()-.5),
                                                size*(rand()-.5),
                                                size*(rand()-.5)
                                        };
                                        const bool f = fun (s[0], s[1], s[2]);
                                        if (refmin==f) {
                                                if (s[0]<mins.x) mins.x = s[0];
                                                if (s[1]<mins.y) mins.y = s[1];
                                                if (s[2]<mins.z) mins.z = s[2];
                                        }
                                        if (refmax==f) {
                                                if (s[0]>maxs.x) maxs.x = s[0];
                                                if (s[1]>maxs.y) maxs.y = s[1];
                                                if (s[2]>maxs.z) maxs.z = s[2];
                                        }
                                }
                        } else {
                                mins.x = mins.y = mins.z = -size/2;
                                maxs.x = maxs.y = maxs.z = size/2;
                        }
                        const float epsilon = 1;
                        this->size = maxs-mins;
                        this->size.x += epsilon;
                        this->size.y += epsilon;
                        this->size.z += epsilon;
                        this->center = (maxs+mins)*.5f;
                        this->rsize.x = 1.f / this->size.x;
                        this->rsize.y = 1.f / this->size.y;
                        this->rsize.z = 1.f / this->size.z;
                        std::cerr << "min={" << mins.x << "," << mins.y << "," << mins.z << "}\n";
                        std::cerr << "max={" << maxs.x << "," << maxs.y << "," << maxs.z << "}\n";
                        std::cerr << "size={" << this->size.x << "," << this->size.y << "," << this->size.z << "}\n";
                        std::cerr << "center={" << this->center.x << "," << this->center.y << "," << this->center.z << "}\n";
                }

                ~BooleanFieldImpl() {
                }

                float intersect (Ray const &ray) const {
                        const Point c = {center.x, center.y, center.z};
                        const Vector s = {size.x, size.y, size.z};
                        float tnear, tfar;
                        const bool does = ray_box (ray, c, s, tnear, tfar);
                        /*if (tnear > 0 || tfar > 0)
                                std::cerr << "[" << tnear << ".." << tfar << "]=" << does;*/
                        if (does && (tnear < tfar)) {
                                return root.intersect (ray, tnear<0?0:tnear, tfar, c, s, 0, *this);
                        }
                        return -1.f;
                }

                float step (Ray const &ray, float distance, float stepSize) const {
                        const bool init_sign = fun (ray*distance);

                        // Assume we begin inside some volume. Hence move backwards first.
                        for (float f = distance; f > distance-0.1f; f-=stepSize) {
                                if (fun (ray*f) != init_sign)
                                        return f;
                        }

                        for (float f = distance; f < distance+0.1f; f+=stepSize) {
                                if (fun (ray*f) != init_sign)
                                        return f;
                        }

                        return -1;
                }


                redshift::optional<Vector> normal (
                        Ray const &ray, float distance,
                        Vector const & ruo, Vector const & rvo
                ) const {
                        const float l = 0.001f, sl = l * 0.1f; // 0.1
                        const Ray
                                ru = {(ray.position+ruo*l), ray.direction},
                                rv = {(ray.position+rvo*l), ray.direction}
                        ;
                        const float
                                a = step (ru, distance, sl),
                                b = step (rv, distance, sl),
                                c = step (ray, distance, sl)
                        ;
                        const Point
                                A = ru * a,
                                B = rv * b
                        ;
                        const Point
                                C = ray * c
                        ;
                        const Vector
                                U = normalize (A-C),
                                V = normalize (B-C),
                                n = cross (U, V)
                        ;

                        return ((a<0)|(b<0))?redshift::optional<Vector>():n;
                }

                Vector normal (Ray const &ray, const float distance) const {
                        const Vector up = {0,1,0};
                        const Vector
                                ruo = normalize (cross (ray.direction, up)),//*/random_perpendicular (ray.direction),
                                rvo = normalize (cross (ruo, ray.direction))
                        ;
                        redshift::optional<Vector> a = normal (ray, distance, ruo, rvo);
                        /*if (!a) a = normal (ray, distance, ruo, -rvo);
                        if (!a) a = normal (ray, distance, -ruo, rvo);
                        if (!a) a = normal (ray, distance, -ruo, -rvo);*/

                        /*if (a) {
                                const Vector ret = {0,1,0};
                                return ret;
                        } else {
                                const Vector ret = {1,0,0};
                                return ret;
                        }*/
                        if (a)
                                return *a;
                        return up;
                }
        };
} // namespace impl {
