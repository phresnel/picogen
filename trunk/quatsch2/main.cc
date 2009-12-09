#if 1
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>

#include <iostream>
#include <sstream>

#include <stdexcept>
#include <deque>
#include <stack>
#include <vector>

#include <string>

namespace quatsch2 {
        //using boost::optional;
        using boost::shared_ptr;
}


namespace quatsch2 { namespace frontend {
        class Position {
                std::wstring::iterator curr_, prog_begin_, prog_end_;
        public:
                /*template <typename iterator>
                static
                Position from_iterators (iterator current, iterator begin, iterator end) {
                        size_t offset_ = distance(begin, current);
                        size_t line_ = 1;
                        size_t column_ = 1;
                        for (iterator it=begin; it!=end && it!=current;  ++it) {
                                if (*it == '\n') {
                                        ++line_;
                                        column_ = 1;
                                } else {
                                        column_++;
                                }
                        }
                        return Position (offset_, line_, column_);
                }*/

                /*Position (size_t offset, size_t line, size_t column)
                : line_ (line), column_(column), offset_(offset) {}*/

                Position (std::wstring::iterator curr,
                        std::wstring::iterator prog_begin,
                        std::wstring::iterator prog_end)
                : curr_(curr_), prog_begin_(prog_begin), prog_end_(prog_end)
                {}

        private:
                void compute(size_t &line, size_t &column) const {
                        using std::wstring;
                        line = 1;
                        column = 1;
                        for (wstring::iterator it=prog_begin_; it!=curr_ && it!=prog_end_; ++it) {
                                if (*it == '\n') {
                                        ++line;
                                        column = 1;
                                } else {
                                        column++;
                                }
                        }
                }
        public:
                size_t line () const {
                        size_t line, col;
                        compute (line, col);
                        return line;
                }
                size_t column () const {
                        size_t line, col;
                        compute (line, col);
                        return col;
                }
                size_t offset () const { return distance(prog_begin_,curr_); }

                std::string to_string () const {
                        size_t line, col;
                        compute (line, col);
                        std::stringstream ss;
                        ss << line << ":" << col;
                        return ss.str();
                }
        };
} }

namespace quatsch2 { namespace middleend {

        class ExpressionListExpr;
        class InvalidExpr;
        class AdditionExpr;
        class SubtractionExpr;
        class MultiplicationExpr;
        class DivisionExpr;
        class FloatLiteralExpr;
        class IntLiteralExpr;
        class IdExpr;

        struct ExpressionVisitor {
                virtual void visit (ExpressionListExpr &) = 0;
                virtual void visit (InvalidExpr &) = 0;
                virtual void visit (AdditionExpr &) = 0;
                virtual void visit (SubtractionExpr &) = 0;
                virtual void visit (MultiplicationExpr &) = 0;
                virtual void visit (DivisionExpr &) = 0;
                virtual void visit (FloatLiteralExpr &) = 0;
                virtual void visit (IntLiteralExpr &) = 0;
                virtual void visit (IdExpr &) = 0;
        };

        class Expression {
        public:
                Expression (frontend::Position const &position)
                : position_ (position)
                {}

                virtual ~Expression() {}

                const frontend::Position & position () const {
                        return position_;
                }

                /*bool valid () const {
                        return begin_ != end_;
                }*/

                /*operator bool () const {
                        return valid();
                }*/

                /*size_t size () const {
                        return distance (begin_, end_);
                }*/

                virtual void accept (ExpressionVisitor &v) = 0;
        private:
                frontend::Position position_;
        };



        class parameters_mixin {
        public:
                typedef shared_ptr<Expression> ExprPtr;
                typedef shared_ptr<const Expression> ConstExprPtr;
                typedef
                        std::vector<ExprPtr>
                        param_vec;
        private:
                param_vec parameters;
                parameters_mixin & operator = (parameters_mixin const &);
        protected:
                void add (ExprPtr expr) {
                        parameters.push_back (expr);
                }
        public:
                parameters_mixin (){}
                parameters_mixin (param_vec const &parameters_)
                : parameters (parameters_)
                {}
                parameters_mixin (parameters_mixin const &pm)
                : parameters (pm.parameters)
                {}

                size_t parameter_count () const {
                        return parameters.size();
                }
                ExprPtr operator [] (size_t index) {
                        return parameters[index];
                }
                ConstExprPtr operator [] (size_t index) const {
                        return parameters[index];
                }
        };



        class InvalidExpr : public Expression {
        public:
                InvalidExpr (frontend::Position const &pos)
                : Expression (pos)
                {}
                virtual ~InvalidExpr() {}
                void accept (ExpressionVisitor &v) {v.visit (*this);}
        private:
        };

        class ExpressionListExpr
        : public Expression
        , public parameters_mixin {
        public:
                ExpressionListExpr(frontend::Position const & pos)
                : Expression (pos)
                , parameters_mixin ()
                {}

                virtual ~ExpressionListExpr() {}
                void add (shared_ptr<Expression> expr) {
                        parameters_mixin::add (expr);
                }
                void accept (ExpressionVisitor &v) {v.visit (*this);}

                size_t expression_count () const {
                        return this->parameter_count();
                }
                shared_ptr<Expression> operator [] (size_t index) {
                        return parameters_mixin::operator[](index);
                }
                shared_ptr<const Expression> operator [] (size_t index) const {
                        return parameters_mixin::operator[](index);
                }
        };


        // +-*/
        class AdditionExpr
        : public Expression
        , public parameters_mixin {
        public:
                AdditionExpr(
                 frontend::Position const &pos,
                 parameters_mixin::param_vec const & parameters
                )
                : Expression (pos)
                , parameters_mixin (parameters)
                {}

                virtual ~AdditionExpr() {}
                void accept (ExpressionVisitor &v) {v.visit (*this);}
                enum { min_param_count = 1, max_param_count = -1 };
        };



        class SubtractionExpr
        : public Expression
        , public parameters_mixin {
        public:
                SubtractionExpr(
                 frontend::Position const &pos,
                 parameters_mixin::param_vec const & parameters
                )
                : Expression (pos)
                , parameters_mixin (parameters)
                {}

                virtual ~SubtractionExpr() {}
                void accept (ExpressionVisitor &v) {v.visit (*this);}
                enum { min_param_count = 1, max_param_count = -1 };
        };



        class MultiplicationExpr
        : public Expression
        , public parameters_mixin {
        public:
                MultiplicationExpr(
                 frontend::Position const &pos,
                 parameters_mixin::param_vec const & parameters
                )
                : Expression (pos)
                , parameters_mixin (parameters)
                {}

                virtual ~MultiplicationExpr() {}
                void accept (ExpressionVisitor &v) {v.visit (*this);}
                enum { min_param_count = 1, max_param_count = -1 };
        };



        class DivisionExpr
        : public Expression
        , public parameters_mixin {
        public:
                DivisionExpr(
                 frontend::Position const &pos,
                 parameters_mixin::param_vec const & parameters
                )
                : Expression (pos)
                , parameters_mixin (parameters)
                {}

                virtual ~DivisionExpr() {}
                void accept (ExpressionVisitor &v) {v.visit (*this);}
                enum { min_param_count = 1, max_param_count = -1 };
        };



        class FloatLiteralExpr : public Expression {
        public:
                FloatLiteralExpr(frontend::Position const &pos)
                : Expression (pos)
                {}
                virtual ~FloatLiteralExpr() {}
                void accept (ExpressionVisitor &v) {v.visit (*this);}
        private:
        };



        class IntLiteralExpr : public Expression {
        public:
                IntLiteralExpr(frontend::Position const &pos)
                : Expression (pos)
                {}
                virtual ~IntLiteralExpr() {}
                void accept (ExpressionVisitor &v) {v.visit (*this);}
        private:
        };



        class IdExpr : public Expression {
        public:
                IdExpr(frontend::Position const &pos, std::wstring name)
                : Expression (pos), name_(name)
                {}
                virtual ~IdExpr() {}
                void accept (ExpressionVisitor &v) {v.visit (*this);}
                std::wstring name () const { return name_; }
        private:
                const std::wstring name_;
        };

} }



#include "jux_frontend.hh"
#include "treeprinter_backend.hh"

#include <iostream>
int main () {
        std::cout << sizeof (quatsch2::frontend::Position) << "\n";
        using namespace quatsch2::frontend::jux;
        typedef Token Tok;
        std::wstring str = L"(+ 1";

        std::deque<Tok> tokens;
        Tokenizer tokenizer;
        tokenizer.tokenize (str.begin(), str.end(), tokens);
        dbg_dump_tokens (tokens);

        try {
                //parse (str.begin(), str.end(), tokens);
                Compiler compiler (str.begin(), str.end(), tokens);
                quatsch2::shared_ptr<quatsch2::middleend::Expression> ast = compiler.parse();
                if (ast.get()) {
                        quatsch2::backend::TreePrinter vt;
                        ast->accept (vt);
                }
        } catch (std::domain_error const &exc) {
                std::cout << exc.what() << '\n';
        }

        std::cout << "\n";
}



#if 0
        #include <climits>
        #include <vector>
        #include <iostream>
        //#include <cstdint>
        #include <inttypes.h>

        template <size_t BASE, typename DIG> class BigInt {
                /*
                typedef unsigned int digit;
                enum { digit_max = UINT_MAX };
                */
                typedef DIG digit;
                enum { base = BASE, digit_max = BASE-1 };


                typedef typename std::vector<digit>::iterator iterator;
                typedef typename std::vector<digit>::const_iterator const_iterator;
                typedef typename std::vector<digit>::reverse_iterator reverse_iterator;
                typedef typename std::vector<digit>::const_reverse_iterator const_reverse_iterator;

                int sign; // let us make things easier
                std::vector<digit> digits;

                digit operator () (size_t i) const {
                        if (i<digits.size())
                                return digits[i];
                        return 0;
                }
                size_t length() const {
                        return digits.size();
                }
                void add (size_t i, digit val) {
                        if (i<length()) {
                                const bool overflow = digits[i] > digit_max - val;
                                digits[i] = (digits[i] + val) % base;
                                if (overflow)
                                        add (i+1, 1);
                        } else {
                                digits.push_back (val);
                        }
                }


                BigInt operator = (BigInt const & rhs);
        public:
                BigInt () : sign(1) {}
                BigInt (BigInt const & rhs) : sign(rhs.sign), digits(rhs.digits) {}

                BigInt (digit val) {
                        do {
                                digits.push_back (val % base);
                                val /= base;
                        } while (val);
                }

                friend BigInt operator + (BigInt const &lhs, BigInt const &rhs) {
                        BigInt ret;
                        const size_t length = lhs.length()>rhs.length()
                                            ? lhs.length()
                                            : rhs.length();
                        for (size_t i=0; i<length; ++i) {
                                ret.add (i, lhs(i));
                                ret.add (i, rhs(i));
                        }
                        return ret;
                }



                void dump() const {
                        for (const_reverse_iterator it=digits.rbegin(); it!=digits.rend(); ++it) {
                                std::cout << (unsigned int)(*it) << ":";
                        }
                        std::cout << '\n';
                }

        };

        int main () {
                typedef BigInt<10, uint8_t> bi;
                (bi(99) + bi(50)).dump();
        }
#endif





#if 0
#include <cstdlib>
#include <iostream>
#include <ctime>

#include <xmmintrin.h>
#include <emmintrin.h>

namespace {
        template <size_t i> struct afloat_array {
                typedef float type[i] __attribute__((aligned (16)));
        };

        template <size_t len>
        struct Vector2 {
                typename afloat_array<len>::type a, b, c, d;
        };


        template <size_t len>
        struct SSEVector {
                __m128 a[len], b[len], c[len], d[len];
        };

        const size_t size = 1000000;
        const size_t more = 1000;

        afloat_array<size>::type dots;
        Vector2<size> b, c;
}
__m128 sse_dots[size/4];
SSEVector<size/4> sse_lhs, sse_rhs;

// report bug when above data put in anonymous namespace

int main () {
        // spread randomness to prevent folding actions
        int some_seed;
        std::cin >> some_seed;
        srand (some_seed);

        for (size_t i=0; i<size; ++i) {
                dots [i] = rand () / (RAND_MAX+1.0f);
                b.a [i]= rand () / (RAND_MAX+1.0f);
                b.b [i]= rand () / (RAND_MAX+1.0f);
                b.c [i]= rand () / (RAND_MAX+1.0f);
                b.d [i]= rand () / (RAND_MAX+1.0f);
                c.a [i]= rand () / (RAND_MAX+1.0f);
                c.b [i]= rand () / (RAND_MAX+1.0f);
                c.c [i]= rand () / (RAND_MAX+1.0f);
                c.d [i]= rand () / (RAND_MAX+1.0f);

                // dirty init
                if (i%4 == 0) {
                        sse_dots[i/4] = _mm_set_ps1(rand () / (RAND_MAX+1.0f));
                        sse_lhs.a[i/4] = _mm_set_ps1(rand () / (RAND_MAX+1.0f));
                        sse_lhs.b[i/4] = _mm_set_ps1(rand () / (RAND_MAX+1.0f));
                        sse_lhs.c[i/4] = _mm_set_ps1(rand () / (RAND_MAX+1.0f));
                        sse_lhs.d[i/4] = _mm_set_ps1(rand () / (RAND_MAX+1.0f));
                        sse_rhs.a[i/4] = _mm_set_ps1(rand () / (RAND_MAX+1.0f));
                        sse_rhs.b[i/4] = _mm_set_ps1(rand () / (RAND_MAX+1.0f));
                        sse_rhs.c[i/4] = _mm_set_ps1(rand () / (RAND_MAX+1.0f));
                        sse_rhs.d[i/4] = _mm_set_ps1(rand () / (RAND_MAX+1.0f));
                }
        }


        /*std::cout << "sse test 2" << std::endl;
        const clock_t sse_begin2 = clock();
        // do some SSE number crunching
        for( size_t m=0; m<more; ++m ) {
        for( size_t i=0; i<size/4; ++i) {
                const __m128
                        a = _mm_mul_ps( sse_lhs.a[i], sse_rhs.a[i]),
                        b = _mm_mul_ps( sse_lhs.b[i], sse_rhs.b[i]),
                        c = _mm_mul_ps( sse_lhs.c[i], sse_rhs.c[i]),
                        d = _mm_mul_ps( sse_lhs.d[i], sse_rhs.d[i]),
                        a_ = _mm_add_ps (a,b),
                        b_ = _mm_add_ps (c,d)
                ;
                sse_dots[i] = _mm_add_ps (a_,b_);
        } }
        const clock_t sse_end2 = clock();*/



        std::cout << "scalar test" << std::endl;
        // do some number crunching
        const clock_t begin = clock();
        if (1) { for (size_t m=0; m<more; ++m) {
        for (size_t i=0; i<size; ++i) {
                dots [i] = b.a[i]*c.a[i]
                         + b.b[i]*c.b[i]
                         + b.c[i]*c.c[i]
                         + b.d[i]*c.d[i];
        }}}
        const clock_t end = clock();


        std::cout << "sse test" << std::endl;
        const clock_t sse_begin = clock();
        // do some SSE number crunching
        if (0) {
        for( size_t m=0; m<more; ++m ) {
        for( size_t i=0; i<size/4; ++i) {
                sse_dots[i] = _mm_add_ps( _mm_add_ps( _mm_add_ps( _mm_mul_ps( sse_lhs.a[i], sse_rhs.a[i] )
                                                                , _mm_mul_ps( sse_lhs.b[i], sse_rhs.b[i] )
                                   )
                                   , _mm_mul_ps( sse_lhs.c[i], sse_rhs.c[i] )
                                   )
                       , _mm_mul_ps( sse_lhs.d[i], sse_rhs.d[i] )
                );
        }}}
        const clock_t sse_end = clock();



        // prevent folding
        float accu = 0.0f;
        for (size_t i=0; i<size; ++i)
                accu += dots[i];
        for (size_t i=0; i<size; ++i) {
                union {__m128 m; float f[4];} const x = {sse_dots[i]}; //evil
                accu += x.f[0]+x.f[1]+x.f[2]+x.f[3];
        }
        std::cout << accu << std::endl;

        std::cout << "\ntime for " << (long)more*size << " dp4's: "
                  << (end-begin)/(float)(CLOCKS_PER_SEC) << '\n';
        std::cout << "\ntime for " << (long)more*size << " sse-dp4's: "
                  << (sse_end-sse_begin)/(float)(CLOCKS_PER_SEC) << '\n';
        /*std::cout << "\ntime for " << (long)more*size << " sse-dp4-2's: "
                  << (sse_end2-sse_begin2)/(float)(CLOCKS_PER_SEC) << '\n';*/

        // sse:    7.924 7.892
        // scalar: 8.111 7.922

}
#endif



// 0) anonymous namespace: not happy
/*#include <xmmintrin.h>
namespace { __m128 *a = (__m128*)_mm_malloc(4*4, 16); }
int main () {*a = _mm_set1_ps(.0f);}*/

/*
// 1) external: happy
#include <xmmintrin.h>
__m128 a;
int main () { a = _mm_set1_ps(.0f); }

// 2) internal: not happy
#include <xmmintrin.h>
static __m128 a;
int main () { a = _mm_set1_ps(.0f); }
*/


/*
namespace photon {

        struct Meters {} meters;

        template <typename float_t=float>
        class LengthBase {
                float_t quantity;
                LengthBase (float_t quant) : quantity(quant) {}
        public:
                LengthBase () : quantity(0.0f) {}
                LengthBase (LengthBase const &rhs) : quantity(rhs.quantity) {}
                LengthBase & operator = (LengthBase const &rhs) {
                        quantity = rhs.quantity;
                        return *this;
                }

                template <typename float_t_>
                friend LengthBase<float_t_> operator * (float_t_ quant, Meters) {
                        return LengthBase<float_t_>(quant);
                }
        };


        namespace float_ {
                typedef LengthBase<float> Length;
        };
        namespace double_ {
                typedef LengthBase<double> Length;
        };
}


int main () {
        using namespace photon;
        using namespace photon::double_;
        Length l = 1.0f*meters;
}

*/
/*
int main () {
        int x;
        int const * const a = &x;                    // const ptr -> const int
        int * const b = &x;                          // const ptr -> int
        int const * c = &x;                          // ptr       -> const int
        int volatile const * const d = &x;           // const ptr -> const volatile int
        int volatile * const e = &x;                 // const ptr -> volatile int
        int volatile const * f = &x;                 // ptr                -> const volatile int
        int const * const volatile g = &x;           // const volatile ptr -> const int
        int * const volatile h = &x;                 // const volatile ptr -> int
        int const * volatile i = &x;                 // volatile ptr       -> const int
        int volatile const * const volatile k = &x;  // const volatile ptr -> const volatile int
        int volatile * const volatile l = &x;        // const volatile ptr -> volatile int
        int volatile const * volatile m = &x;        // volatile ptr -> const volatile int
        int volatile * n = &x;                       // ptr          -> volatile int
        int * volatile o = &x;                       // volatile ptr -> int
        int volatile * volatile p = &x;              // volatile ptr -> volatile int
}
*/

#if 0
#include <climits>

// Make *_MAX available for template code.
template <typename T> struct get_val_max;
template <> struct get_val_max<char> { enum { value = CHAR_MAX }; };
template <> struct get_val_max<signed char> { enum { value = SCHAR_MAX }; };
template <> struct get_val_max<unsigned char> { enum { value = UCHAR_MAX }; };
template <> struct get_val_max<short> { enum { value = SHRT_MAX }; };
template <> struct get_val_max<unsigned short> { enum { value = USHRT_MAX }; };
template <> struct get_val_max<int> { enum { value = INT_MAX }; };
template <> struct get_val_max<unsigned int> { enum { value = UINT_MAX }; };
template <> struct get_val_max<long> { enum { value = LONG_MAX }; };
template <> struct get_val_max<unsigned long> { enum { value = ULONG_MAX }; };
#ifdef LLONG_MAX
template <> struct get_val_max<long long> { enum { value = LLONG_MAX }; };
template <> struct get_val_max<unsigned long long> { enum { value = ULLONG_MAX }; };
#endif

// Check signedness of a type.
template <typename T> struct is_signed { enum { value = 1 }; };
template <> struct is_signed <char> { enum { value = CHAR_MIN < 0 }; };
template <> struct is_signed <unsigned char> { enum { value = 0 }; };
template <> struct is_signed <unsigned short> { enum { value = 0 }; };
template <> struct is_signed <unsigned int> { enum { value = 0 }; };
template <> struct is_signed <unsigned long> { enum { value = 0 }; };
#ifdef LLONG_MAX
template <> struct is_signed <unsigned long long> { enum { value = 0 }; };
#endif

// Recurse and divide by base until zero is reached
template <typename T, T base, T val, bool recurse=val!=0>
struct get_dig_max_rec {
        enum { value = 1 + get_dig_max_rec<T, base, val/base>::value };
};
template <typename T, T base, T val> struct get_dig_max_rec<T,base,val,false> {
        enum { value = 0 };
};

// Get number of digits needed to represent any value of T in base
template <typename T, int base> struct get_dig_max {
        enum {
                value = is_signed<T>::value
                      + get_dig_max_rec<T, base, get_val_max<T>::value>::value
        };
};

// some typedefs
template <typename T> struct get_base10_dig_max : get_dig_max<T,10> {};
template <typename T> struct get_base16_dig_max : get_dig_max<T,16> {};
template <typename T> struct get_base2_dig_max : get_dig_max<T,2> {};

#include <iostream>
int main () {
        std::cout << get_base10_dig_max<char>::value << '\n';
        std::cout << get_base10_dig_max<signed char>::value << '\n';
        std::cout << get_base10_dig_max<unsigned char>::value << '\n';
        std::cout << get_base10_dig_max<unsigned int>::value << '\n';
        std::cout << get_base10_dig_max<int>::value << '\n';
        // etc.
}
#endif








#if 0
#include <iostream>
#include <cmath>
#include <cstdlib>

namespace {

struct Rgb {
        float r,g,b;
};

struct Point {
        float x,y,z;
};

struct Vector {
        float x,y,z;
};

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

Vector normalize (Vector const & vec) {
        return vec * (1.f/length(vec));
}



Point operator + (Point const &lhs, Vector const &rhs) {
        const Point ret = {
                lhs.x + rhs.x,
                lhs.y + rhs.y,
                lhs.z + rhs.z,
        };
        return ret;
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



struct Ray {
        Point position;
        Vector direction;
};
Point operator * (Ray const & ray, float f) {
        return ray.position + ray.direction * f;
}

struct Intersection {
        float distance;
        Rgb color;
};

inline float sat (float f) {
        return f<.0f?.0f:f>1.f?1.f:f;
}

inline bool fun (Point const &pos) {
        const bool a = pos.y < -2 + (0.5f * sin(pos.x*3) * sin(pos.z*3));
        const bool b = sqrt (pos.x*pos.x + (pos.y)*(pos.y) + pos.z*pos.z)<1.0f;
        const bool c = (sqrt(pow (pos.x+2.f, 2.f)+pow (pos.z-2.f, 2.f))<3.f)
                        & (pos.y>2.0f) & (pos.y<2.2f);
        const bool d = (sqrt(pow (pos.x+2.f, 2.f)+pow (pos.z-2.f, 2.f))<1.f)
                        & (pos.y<2.0f);

        return a | b | c | d;
}

inline bool is_empty (Point const &c, float size) {
        const int count = 10+20*size*size*size;
        for (int i=0; i<count; ++i) {
                const Point samplePoint = {
                        c.x + (size * rand() / (1.f+RAND_MAX) - size *.5f),
                        c.y + (size * rand() / (1.f+RAND_MAX) - size *.5f),
                        c.z + (size * rand() / (1.f+RAND_MAX) - size *.5f)
                };
                if (fun (samplePoint)) {
                        return false;
                }
        }
        return true;
}

/*
bool ray_box_does (const Ray &r, const Point &c, float size, float t0=0, float t1=1000000.f) {
        float tmin, tmax, tymin, tymax, tzmin, tzmax;

        const int sign_x = r.direction.x < 0;
        const int sign_y = r.direction.y < 0;
        const int sign_z = r.direction.z < 0;
        const Vector size05 = {.5f*size, .5f*size, .5f*size};
        const Point parameters [2] = {
                c - size05, c + size05
        };

        tmin = (parameters[sign_x].x - r.position.x) * (1.f/r.direction.x);
        tmax = (parameters[1-sign_x].x - r.position.x) * (1.f / r.direction.x);
        tymin = (parameters[sign_y].y - r.position.y) * (1.f / r.direction.y);
        tymax = (parameters[1-sign_y].y - r.position.y) * (1.f / r.direction.y);
        if ( (tmin > tymax) || (tymin > tmax) )
                return false;
        if (tymin > tmin)
                tmin = tymin;
        if (tymax < tmax)
                tmax = tymax;
        tzmin = (parameters[sign_z].z - r.position.z) * (1.f/r.direction.z);
        tzmax = (parameters[1-sign_z].z - r.position.z) * (1.f/r.direction.z);
        if ( (tmin > tzmax) || (tzmin > tmax) )
                return false;
        if (tzmin > tmin)
                tmin = tzmin;
        if (tzmax < tmax)
                tmax = tzmax;
        return ( (tmin < t1) && (tmax > t0) );
}*/

// stupid code duplication
bool ray_box (const Ray &r, const Point &c, float size, float &tmin, float &tmax) {
        const float t0=0, t1=1000000.f;
        float tymin, tymax, tzmin, tzmax;

        const int sign_x = r.direction.x < 0;
        const int sign_y = r.direction.y < 0;
        const int sign_z = r.direction.z < 0;
        const Vector size05 = {.5f*size, .5f*size, .5f*size};
        const Point parameters [2] = {
                c - size05, c + size05
        };

        tmin = (parameters[sign_x].x - r.position.x) * (1.f/r.direction.x);
        tmax = (parameters[1-sign_x].x - r.position.x) * (1.f / r.direction.x);
        tymin = (parameters[sign_y].y - r.position.y) * (1.f / r.direction.y);
        tymax = (parameters[1-sign_y].y - r.position.y) * (1.f / r.direction.y);
        if ( (tmin > tymax) || (tymin > tmax) )
                return false;
        if (tymin > tmin)
                tmin = tymin;
        if (tymax < tmax)
                tmax = tymax;
        tzmin = (parameters[sign_z].z - r.position.z) * (1.f/r.direction.z);
        tzmax = (parameters[1-sign_z].z - r.position.z) * (1.f/r.direction.z);
        if ( (tmin > tzmax) || (tzmin > tmax) )
                return false;
        if (tzmin > tmin)
                tmin = tzmin;
        if (tzmax < tmax)
                tmax = tzmax;
        return ( (tmin < t1) && (tmax > t0) );
}


const Vector cc [8] = {
        { -1, -1, -1 },
        {  1, -1, -1 },
        { -1,  1, -1 },
        {  1,  1, -1 },
        { -1, -1,  1 },
        {  1, -1,  1 },
        { -1,  1,  1 },
        {  1,  1,  1 },
};
class BitOctree {

        struct Node {
                enum State {
                        uninit,
                        empty,
                        nonempty
                };
                mutable State state;
                mutable Node *children;


                Node () : state (uninit), children(0) {}
                ~Node() { delete [] children; }

                float intersect (
                        Ray const &ray, Point const &c, float size
                ) const {
                        if (uninit == state)
                                state = is_empty (c, size) ? empty : nonempty;
                        if (empty == state) {
                                return -1;
                        }
                        float tmin, tmax;
                        const float rb = ray_box (ray, c, size, tmin, tmax);
                        if (rb>0) {
                                if (0 == children && size>0.05f)
                                        children = new Node[8];

                                if (children) {
                                        float nearest = -1;
                                        for (int i=0; i<8; ++i) {
                                                const float tmp = children[i].intersect(
                                                        ray,
                                                        c+cc[i]*size*0.25f,
                                                        size*0.5f
                                                );
                                                if (nearest < 0 || (tmp > 0 && tmp < nearest))
                                                        nearest = tmp;
                                        }
                                        return nearest;
                                } else {
                                        // ray march, hacky, too much and too big epsila
                                        if (tmin < 0.3) tmin = 0.3;
                                        //bool init_sign = fun (ray*(tmin-0.3f));
                                        bool curr_sign = true;// = init_sign;
                                        float dist;
                                        for (
                                                dist=tmin;
                                                (dist<tmax+0.01f) & (curr_sign);
                                                dist+=.0025f
                                        ) {
                                                curr_sign = fun(ray*dist);
                                        }

                                        return curr_sign ? dist : -1;
                                }
                        }
                        return -1;
                }
        };

        float size;
        Node root;

public:
        BitOctree (float size)
        : size(size), root ()
        {
        }

        float intersect (Ray const &ray) const {
                const Point c = {.0f,.0f,0.f};
                return root.intersect (ray, c, size);
        }
};

class Renderer {
        const int width, height;
        mutable BitOctree ldo;

        Ray generateRay (float x, float y) const {
                const Point position = {
                        0.f, 0.5f, -5.2f
                };
                const Vector direction = {
                        (x-.5f)*2,
                        -(y-.5f)*2,
                        1.0f,
                };
                const Ray ray = {
                        position,
                        normalize (direction)
                };
                return ray;
        }

        Intersection raytrace (float x, float y, float startDist) const {
                const Ray ray = generateRay (x,y);
                const Rgb backCol = {
                        ray.direction.x,
                        ray.direction.y,
                        ray.direction.z
                };

                const float distance = ldo.intersect (ray);
                const Rgb hitCol = {
                        distance*.2f,
                        distance*.1f,
                        distance*.05f
                };

                const Intersection i = {
                        distance,
                        distance>.0f ? hitCol : backCol
                };

                return i;
        }

public:
        Renderer (int width, int height)
        : width(width), height(height), ldo(20.f)
        {
        }

        ~Renderer () {
        }


        void render () {
                std::cout << "P3\n"
                          << width << ' ' << height << '\n'
                          << "255\n";

                for (int y=0; y<height; ++y) {
                        float startDist = 0.f;
                        for (int x=0; x<width; ++x) {
                                const Intersection i = raytrace (x/(float)width,y/(float)height,0);
                                startDist = i.distance;
                                std::cout << (int)(sat(i.color.r)*255) << ' '
                                          << (int)(sat(i.color.g)*255) << ' '
                                          << (int)(sat(i.color.b)*255) << ' ';
                        }
                }
        }
};

}



int main () {
        Renderer renderer (256,256);
        renderer.render();
}
#endif




#if 0
#include <iostream>

namespace {
namespace inline_asm_training {
        int inc (int i) {
                asm ("\n\t"
                "incl %0"
                : "=g"(i)
                : "0"(i)
                );
                return i;
        }

        int fac (int i) {
                int ret=1;

                asm ("\n\t"
                "again%=:        \n\t"      // inlining et al. require %= to make the label unique
                "    mul %2      \n\t"      // a = a * %2
                "    dec %2      \n\t"
                "    jnz again%= \n\t"
                "\n\t"
                : "=a"(ret)
                : "0"(ret), "r"(i)
                : "dx" // because it will be clobbered by mul
                );

                return ret;
        }
};
}

int main () {
        using namespace inline_asm_training;
        std::cout << fac (6) << '\n';
}
#endif





/*
#include <iostream>
#include <string>

class Value;
class Real;
class Addition;
class Multiply;
class Group;

class Visitor {
public:
        virtual void visit (Real const &n) = 0;
        virtual void visit (Addition const &n) = 0;
        virtual void visit (Multiply const &n) = 0;
        virtual void visit (Group const &n) = 0;
};


class Value {
public:
        virtual void accept (Visitor & visitor) const = 0;
};

class Real : public Value {
        float value_;
public:
        Real (float value_) : value_ (value_) {}

        float value () const { return value_; }

        void accept (Visitor & visitor) const {
                visitor.visit (*this);
        }
};

class Addition : public Value {
        Value *lhs_, *rhs_;
public:
        Addition (Value &lhs_, Value &rhs_)
        : lhs_(&lhs_), rhs_(&rhs_) {}

        const Value & lhs() const { return *lhs_; }
        const Value & rhs() const { return *rhs_; }

        void accept (Visitor & visitor) const {
                visitor.visit (*this);
        }
};


class Multiply : public Value {
        Value *lhs_, *rhs_;
public:
        Multiply (Value &lhs_, Value &rhs_)
        : lhs_(&lhs_), rhs_(&rhs_) {}

        const Value & lhs() const { return *lhs_; }
        const Value & rhs() const { return *rhs_; }

        void accept (Visitor & visitor) const {
                visitor.visit (*this);
        }
};


class Group : public Value {
        Value *val;
public:
        Group (Value &val)
        : val(&val) {}

        const Value & value() const { return *val; }

        void accept (Visitor & visitor) const {
                visitor.visit (*this);
        }
};


class CxxVisitor : public Visitor {
        void visit (Real const &n) {
                std::cout << n.value();
        }

        void visit (Group const &n) {
                std::cout << "(";
                n.value().accept (*this);
                std::cout << ")";
        }

        void visit (Addition const &n) {
                n.lhs().accept (*this);
                std::cout << " + ";
                n.rhs().accept (*this);
        }

        void visit (Multiply const &n) {
                n.lhs().accept (*this);
                std::cout << " * ";
                n.rhs().accept (*this);
        }
};

class LispVisitor : public Visitor {
        void visit (Real const &n) {
                std::cout << n.value();
        }

        void visit (Group const &n) {
                n.value().accept (*this);
        }

        void visit (Addition const &n) {
                std::cout << "(+ ";
                n.lhs().accept (*this);
                std::cout << " ";
                n.rhs().accept (*this);
                std::cout << ")";
        }

        void visit (Multiply const &n) {
                std::cout << "(* ";
                n.lhs().accept (*this);
                std::cout << " ";
                n.rhs().accept (*this);
                std::cout << ")";
        }
};

int main () {
        //CxxVisitor cxx;
        Real alpha(5), bravo(6);
        Addition add (alpha, bravo);
        Addition add2 (add, add);
        Group gg (add);
        Multiply mul (gg,gg);

        CxxVisitor cxx;
        mul.accept (cxx);
        std::cout << '\n';

        LispVisitor lisp;
        mul.accept (lisp);
}
*/
#endif


#if 0

#include <ctime>
#include <cmath>
#include <iostream>

void test_normal(int c, float accu) {
        const clock_t begin = clock();
        for (int i=0; i<c; ++i) {
                accu = std::sin (accu);
        }
        const clock_t end = clock();
        std::cout << accu << ":" << (end-begin)/(float)CLOCKS_PER_SEC << '\n';
}

namespace fast_math {
        const float pi = 3.14159265358979323846f;
        const float pi2 = pi * 2.f;

        const int sin_table_size = 256;
        /*
        .980785
        .978451
        .978342

        */
        float sin_table [sin_table_size];

        void init () {
                for (int i=0; i<sin_table_size; ++i) {
                        const float f = (pi2*i)/sin_table_size;
                        sin_table[i] = std::sin (f);
                }
        }
        // TODO: write for negative inpuit
        float sin (float f) {
                const int index = (int)((sin_table_size*f)/pi2) % sin_table_size;
                return sin_table[index];
        }
        float sin_i (float f) {
                const float x = (sin_table_size*f)/pi2;
                const int index = (int)x % sin_table_size;
                const float a = sin_table[index];
                const float b = sin_table[(index+1) % sin_table_size];
                return a + (b-a) * (x-index);
        }
};
void test_fast(int c, float accu) {
        {
        const clock_t begin = clock();
        for (int i=0; i<c; ++i)
                accu = fast_math::sin (accu);
        const clock_t end = clock();
        std::cout << accu << ":" << (end-begin)/(float)CLOCKS_PER_SEC << '\n';
        }
        {
        const clock_t begin = clock();
        for (int i=0; i<c; ++i)
                accu = fast_math::sin_i (accu);
        const clock_t end = clock();
        std::cout << "(i)" << accu << ":" << (end-begin)/(float)CLOCKS_PER_SEC << '\n';
        }
}

int main () {
        fast_math::init ();
        while (1) {
                float f;
                std::cin >> f;
                std::cout << "std:" << std::sin (f) << '\n';
                std::cout << "fast:" << fast_math::sin (f) << '\n';
                std::cout << "fast w/ interp:" << fast_math::sin_i (f) << '\n';

                test_normal(1<<26, f);
                test_fast(1<<26, f);
        }
}

#endif

#if 0
#include <iostream>
#include <cmath>
int main () {
        float f;
        std::cin >> f;
        float a = sin (f);
        float b = cos (f);
        std::cout << a+b;
}
#endif
