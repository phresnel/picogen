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

#ifndef INT_TRAITS_H_20090729
#define INT_TRAITS_H_20090729

///////////////////////////////////////////////////////////////////////////////
// int traits
///////////////////////////////////////////////////////////////////////////////

namespace kallisto { namespace traits {

        ///////////////////////////////////////////////////////////////////////
        // std::numeric_limits only provides template-problematic *functions*
        ///////////////////////////////////////////////////////////////////////
        template <typename T> struct integer_limits ;
        template <> struct integer_limits <int8_t>   { enum { min = INT8_MIN,   max = INT8_MAX   }; };
        template <> struct integer_limits <int16_t>  { enum { min = INT16_MIN,  max = INT16_MAX  }; };
        template <> struct integer_limits <int32_t>  { enum { min = INT32_MIN,  max = INT32_MAX  }; };
        template <> struct integer_limits <int64_t>  { enum { min = INT64_MIN,  max = INT64_MAX  }; };
        template <> struct integer_limits <uint8_t>  { enum { min = 0, max = UINT8_MAX  }; };
        template <> struct integer_limits <uint16_t> { enum { min = 0, max = UINT16_MAX }; };
        template <> struct integer_limits <uint32_t> { enum { min = 0, max = UINT32_MAX }; };
        template <> struct integer_limits <uint64_t> { enum { min = 0, max = UINT64_MAX }; };

        STATIC_ASSERT((integer_limits<int8_t>::min == INT8_MIN));
        STATIC_ASSERT((integer_limits<int16_t>::min == INT16_MIN));
        STATIC_ASSERT((integer_limits<int32_t>::min == INT32_MIN));
        STATIC_ASSERT((integer_limits<int64_t>::min == INT64_MIN));
        STATIC_ASSERT((integer_limits<uint8_t>::min == 0));
        STATIC_ASSERT((integer_limits<uint16_t>::min == 0));
        STATIC_ASSERT((integer_limits<uint32_t>::min == 0));
        STATIC_ASSERT((integer_limits<uint64_t>::min == 0));
        STATIC_ASSERT((integer_limits<int8_t>::max == INT8_MAX));
        STATIC_ASSERT((integer_limits<int16_t>::max == INT16_MAX));
        STATIC_ASSERT((integer_limits<int32_t>::max == INT32_MAX));
        STATIC_ASSERT((integer_limits<int64_t>::max == INT64_MAX));
        STATIC_ASSERT((integer_limits<uint8_t>::max == UINT8_MAX));
        STATIC_ASSERT((integer_limits<uint16_t>::max == UINT16_MAX));
        STATIC_ASSERT((integer_limits<uint32_t>::max == UINT32_MAX));
        STATIC_ASSERT((integer_limits<uint64_t>::max == UINT64_MAX));

        
        
} }

#endif // INT_TRAITS_H_20090729
