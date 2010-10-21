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

#ifndef REDUCTION_HH_INCLUDED_20100309
#define REDUCTION_HH_INCLUDED_20100309

namespace kallisto {
        // -- Reductions ------------------------------------------------------
        // I guess making reductions part of the expression template closure
        // would not make much sense, so KISS away.

        template <typename T> class _reduce_product {
                T result_;
        public:
                _reduce_product () : result_(1) {}
                T result () const { return result_; }
                void apply (T element) { result_ *= element; }
        };

        template <typename T, unsigned int N, typename RULES, typename REP>
        inline typename traits::enable_if<
                has_member_reduction_product<RULES>,
                T
        >::type
        product (array<T,N,RULES,REP> const &arr) {
                return _array_reduce<_reduce_product> (arr);
        }


        template <typename T> class _reduce_sum {
                T result_;
        public:
                _reduce_sum () : result_(0) {}
                T result () const { return result_; }
                void apply (T element) { result_ += element; }
        };

        template <typename T, unsigned int N, typename RULES, typename REP>
        inline typename traits::enable_if<
                has_member_reduction_sum<RULES>,
                T
        >::type
        sum (array<T,N,RULES,REP> const &arr) {
                return _array_reduce<_reduce_sum> (arr);
        }


        template <typename T> class _reduce_length {
                T result_;
        public:
                _reduce_length () : result_(0) {}
                T result () const { return std::sqrt(result_); }
                void apply (T element) { result_ += element*element; }
        };

        template <typename T, unsigned int N, typename RULES, typename REP>
        inline typename traits::enable_if<
                has_member_reduction_length<RULES>,
                T
        >::type
        length (array<T,N,RULES,REP> const &arr) {
                return _array_reduce<_reduce_length> (arr);
        }


        template <typename T> class _reduce_length_sq {
                T result_;
        public:
                _reduce_length_sq () : result_(0) {}
                T result () const { return result_; }
                void apply (T element) { result_ += element*element; }
        };

        template <typename T, unsigned int N, typename RULES, typename REP>
        inline typename traits::enable_if<
                has_member_reduction_length_sq<RULES>,
                T
        >::type
        length_sq (array<T,N,RULES,REP> const &arr) {
                return _array_reduce<_reduce_length_sq> (arr);
        }


        // For sake of KISS the following are implemented quite
        // straightforward.
        // TODO: use KALLISTO_ARRAY_FRIEND_FOREACHELEM again,
        //       so that it might profit from parallelization again.
        //       (note though that as of writing this, there is
        //       no parallelization in KALLISTO_ARRAY_FRIEND_FOREACHELEM)
        template <unsigned int N, typename RULES, typename REP>
        inline typename traits::enable_if<has_member_reduction_all<RULES>,bool>::type
        all (array<bool,N,RULES,REP> const &arr) {
                for (unsigned int i=0; i<N; ++i) {
                        if (!arr[i]) return false;
                }
                return true;
        }

        template <unsigned int N, typename RULES, typename REP>
        inline typename traits::enable_if<has_member_reduction_none<RULES>,bool>::type
        none (array<bool,N,RULES,REP> const &arr) {
                for (unsigned int i=0; i<N; ++i) {
                        if (arr[i]) return false;
                }
                return true;
        }

        template <unsigned int N, typename RULES, typename REP>
        inline typename traits::enable_if<has_member_reduction_any<RULES>,bool>::type
        any (array<bool,N,RULES,REP> const &arr) {
                for (unsigned int i=0; i<N; ++i) {
                        if (arr[i]) return true;
                }
                return false;
        }



        // isnan isinf
        template <typename T> inline bool _isnan (T val) {
                return val != val;
        }
        template <typename T> inline bool _isinf (T val) {
                return std::numeric_limits<T>().infinity() == val;
        }

        template <typename T, unsigned int N, typename RULES, typename REP>
        inline typename traits::enable_if<has_member_reduction_isnan<RULES>,bool>::type
        isnan (array<T,N,RULES,REP> const &arr) {
                for (unsigned int i=0; i<N; ++i)
                        if (_isnan(arr[i])) return true;
                return false;
        }

        template <typename T, unsigned int N, typename RULES, typename REP>
        inline typename traits::enable_if<has_member_reduction_isinf<RULES>,bool>::type
        isinf (array<T,N,RULES,REP> const &arr) {
                for (unsigned int i=0; i<N; ++i)
                        if (_isinf(arr[i])) return true;
                return false;
        }


        // Sidelore: Upon testing my implementation with [uninitialized] data,
        // I realised that iff there are dividesisions in the calculation,
        // my array was a tad slower than std::valarray. I then initialized it
        // so that the dividesision doesn't go nan or inf, and apart from a boost
        // up of roughly 1000 percent, I was ahead of valarray<> again.
}

#endif // REDUCTION_HH_INCLUDED_20100309
