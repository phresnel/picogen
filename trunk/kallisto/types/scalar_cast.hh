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

#ifndef SCALAR_CAST_H__INCLUDED__20090210
#define SCALAR_CAST_H__INCLUDED__20090210

namespace kallisto {
        ///////////////////////////////////////////////////////////////////////
        // scalar cast
        ///////////////////////////////////////////////////////////////////////
        template <typename lhs_t, typename rhs_t>
        inline lhs_t scalar_cast (rhs_t const & rhs) ;

        template <typename lhs_t, typename rhs_t>
        inline void scalar_cast (lhs_t &lhs, rhs_t const & rhs) ;


        namespace scalar_cast_private_ {
                //+++++++++++ scalar_cast_to_primitive
                // TODO: instead of "is_float", there should be a more detailed trait that allows us to really distinguish between primitive - and non-primitive scalar types.
                template <typename lhs_t, typename rhs_t, bool lhs_is_float = traits::is_float<lhs_t>::value> struct scalar_cast_to_primitive ;

                template <typename lhs_t, typename rhs_basetype, unsigned int rhs_fractbits>
                class scalar_cast_to_primitive<lhs_t, fixed_point_t<rhs_basetype,rhs_fractbits>, true> {
                        scalar_cast_to_primitive();
                        template <typename, typename> friend class scalar_cast_def;
                        static lhs_t cast (fixed_point_t<rhs_basetype,rhs_fractbits> const & rhs) {
                                typedef fixed_point_t<rhs_basetype,rhs_fractbits> rhs_t;
                                enum {
                                        rhs_fractmask = (1 << rhs_fractbits)-1
                                };
                                return    static_cast<lhs_t> (rhs.bits >> rhs_fractbits)
                                        + static_cast<lhs_t> (rhs.bits & rhs_fractmask)
                                        / static_cast<lhs_t> (1 << rhs_fractbits);
                        }
                };

                template <typename lhs_t, typename rhs_basetype, unsigned int rhs_fractbits>
                class scalar_cast_to_primitive<lhs_t, fixed_point_t<rhs_basetype,rhs_fractbits>, false> {
                        scalar_cast_to_primitive();
                        template <typename, typename> friend class scalar_cast_def;
                        static lhs_t cast (fixed_point_t<rhs_basetype,rhs_fractbits> const & rhs) {
                                return static_cast<lhs_t> (rhs.bits >> rhs_fractbits);
                        }
                };
                //----------- scalar_cast_to_primitive


                //+++++++++++ scalar_cast_from_primitive
                template <typename lhs_t, typename rhs_t, bool rhs_is_float = traits::is_float<rhs_t>::value> struct scalar_cast_from_primitive ;


                template <typename lhs_basetype, unsigned int lhs_fractbits, typename rhs_t>
                class scalar_cast_from_primitive<fixed_point_t<lhs_basetype,lhs_fractbits>, rhs_t, false> {
                        scalar_cast_from_primitive();

                        template <typename, typename> friend class scalar_cast_def;
                        static fixed_point_t<lhs_basetype,lhs_fractbits>
                        cast (rhs_t const & rhs) {
                                //std::cout << "scalar_cast-from_primitive<int>" << std::endl;
                                //std::cout << " rhs:" << rhs << std::endl;
                                fixed_point_t<lhs_basetype,lhs_fractbits>
                                const ret = {
                                  static_cast<lhs_basetype>(rhs) <<
                                      static_cast<lhs_basetype>(lhs_fractbits)
                                };
                                //std::cout << " lhs:" << ret.bits << std::endl;
                                return ret;
                        }
                };


                inline static void tear_float_apart (float const & orig, float & fractional, float & integral) {
                        fractional = ::modff (orig, &integral);
                }
                inline static void tear_float_apart (double const & orig, double & fractional, double & integral) {
                        fractional = ::modf (orig, &integral);
                }
                inline static void tear_float_apart (long double const & orig, long double & fractional, long double &integral) {
                        fractional = ::modfl (orig, &integral);
                }

                template <typename lhs_basetype, unsigned int lhs_fractbits, typename rhs_t>
                class scalar_cast_from_primitive<fixed_point_t<lhs_basetype,lhs_fractbits>, rhs_t, true> {
                        scalar_cast_from_primitive();
                        template <typename, typename> friend class scalar_cast_def;

                        static fixed_point_t<lhs_basetype,lhs_fractbits>
                        cast (rhs_t const & rhs) {
                                rhs_t fractional, integral;
                                tear_float_apart (rhs, fractional, integral);
                                //::std::cout << "fract:" << fractional << ", int:" << integral << ::std::endl;
                                enum {
                                        max_fract_val = 1 << lhs_fractbits
                                };
                                fixed_point_t<lhs_basetype,lhs_fractbits> const ret = {
                                        (static_cast<lhs_basetype> (integral) << lhs_fractbits)
                                        + static_cast<lhs_basetype> (fractional * static_cast<rhs_t> (max_fract_val))
                                };
                                return ret;
                        }
                };

                //----------- scalar_cast_from_primitive



                //+++++++++++ scalar_cast_def
                template <typename lhs_t, typename rhs_t> struct scalar_cast_def ;


                // fixed to primitive type (to *int* or to *float*)
                template <
                        typename lhs_t,
                        typename rhs_basetype, unsigned int rhs_fractbits
                >
                class scalar_cast_def<
                        lhs_t, fixed_point_t<rhs_basetype,rhs_fractbits>
                > {
                        scalar_cast_def();

                        template <typename lhs_t_, typename rhs_t_> friend lhs_t_ kallisto::scalar_cast (rhs_t_ const &rhs);
                        template <typename lhs_t_, typename rhs_t_> friend void kallisto::scalar_cast (lhs_t_ &lhs, rhs_t_ const & rhs);

                        static lhs_t cast (fixed_point_t<rhs_basetype,rhs_fractbits> const & rhs) {
                                return scalar_cast_to_primitive<
                                        lhs_t,
                                        fixed_point_t<rhs_basetype,rhs_fractbits>
                                >::cast (rhs);
                        }
                };


                // fixed from primitive type (to *int* or to *float*)
                template <typename lhs_basetype, unsigned int lhs_fractbits, typename rhs_t>
                class scalar_cast_def <fixed_point_t<lhs_basetype,lhs_fractbits>, rhs_t> {
                        scalar_cast_def();

                        typedef fixed_point_t<lhs_basetype,lhs_fractbits> lhs_t;

                        template <typename lhs_t_, typename rhs_t_> friend lhs_t_ kallisto::scalar_cast (rhs_t_ const &rhs);
                        template <typename lhs_t_, typename rhs_t_> friend void kallisto::scalar_cast (lhs_t_ &lhs, rhs_t_ const & rhs);

                        static lhs_t
                        cast (rhs_t const & rhs) {
                                return scalar_cast_from_primitive<lhs_t,rhs_t>::cast (rhs);
                        }
                };

                // fixed to fixed
                template <
                        typename lhs_basetype, unsigned int lhs_fractbits,
                        typename rhs_basetype, unsigned int rhs_fractbits
                >
                class scalar_cast_def<
                        fixed_point_t<lhs_basetype,lhs_fractbits>,
                        fixed_point_t<rhs_basetype,rhs_fractbits>
                > {
                        scalar_cast_def ();

                        template <typename lhs_t, typename rhs_t> friend lhs_t kallisto::scalar_cast (rhs_t const &rhs);
                        template <typename lhs_t, typename rhs_t> friend void kallisto::scalar_cast (lhs_t &lhs, rhs_t const & rhs);

                        static fixed_point_t<lhs_basetype,lhs_fractbits> cast (fixed_point_t<rhs_basetype,rhs_fractbits> const & rhs) {
                                typedef fixed_point_t<lhs_basetype,lhs_fractbits> lhs_t;
                                typedef fixed_point_t<rhs_basetype,rhs_fractbits> rhs_t;

                                // In the following snippet, we need a signed type for lshift<>.
                                // the strange ?: device is to ensure that no over-/underflow happens.
                                enum {
                                        tmp_a = lhs_fractbits,
                                        tmp_b = rhs_fractbits,
                                        is_reduction = tmp_a<tmp_b
                                };
                                // Have to split up into another enum, above one can yield an unsigned basetype
                                // on g++ 4.3 (other compilers untested), but 'tmp_c' might depend on signed-ness.
                                enum { tmp_c = (is_reduction ? -(tmp_b - tmp_a) : (tmp_a-tmp_b)) };

                                if (is_reduction) {
                                        // round
                                        lhs_t const ret = { signed_shl<tmp_c> (rhs.bits) + (signed_shl<tmp_c+1> (rhs.bits) & 1) };
                                        return ret;
                                } else {
                                        // don't round
                                        lhs_t const ret = { signed_shl<tmp_c> (rhs.bits) };
                                        return ret;
                                }
                        }
                };
                //----------- scalar_cast_def

        } // namespace scalar_cast_private_

        //+++++++++++ scalar_cast
        template <typename lhs_t, typename rhs_t>
        inline lhs_t scalar_cast (rhs_t const & rhs) {
                return scalar_cast_private_::scalar_cast_def<lhs_t, rhs_t>::cast (rhs);
        }

        inline float scalar_cast (float const & rhs) { return rhs; }
        inline double scalar_cast (double const & rhs) { return rhs; }
        inline long double scalar_cast (long double const & rhs) { return rhs; }

        template <typename lhs_t, typename rhs_t>
        inline void scalar_cast (lhs_t &lhs, rhs_t const & rhs) {
                lhs = scalar_cast<lhs_t, rhs_t> (rhs);
        }

        inline void scalar_cast (float &lhs, float const & rhs) { lhs = rhs; }
        inline void scalar_cast (double &lhs, double const & rhs) { lhs = rhs; }
        inline void scalar_cast (long double &lhs, long double & rhs) { lhs = rhs; }
        //----------- scalar_cast
}

namespace kallisto { namespace unit_test {
        class test_scalar_cast {
        private:
                const static test_scalar_cast test;
                test_scalar_cast () {
                        UNIT_TEST_REPORT_BEGIN("");
                        UNIT_TEST_WARN("no tests defined!");
                }
        };
} }

#endif // SCALAR_CAST_H__INCLUDED__20090210
