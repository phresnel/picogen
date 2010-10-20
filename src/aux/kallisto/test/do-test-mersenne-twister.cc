///////////////////////////////////////////////////////////////////////////////
// This file was generated automatically, manual edits might be overwritten"
// without any warning!
///////////////////////////////////////////////////////////////////////////////
// (C)2009 Sebastian Mach (*1983) / phresnel
//  this file is licensed under GPL, version 3, or newer
//  mail: com.gmail.phresnel , for nice mail swap "com" and "phresnel" 
//   web: http://picogen.org , http://phresnel.org 
///////////////////////////////////////////////////////////////////////////////
// file generated at Thu Jul 30 08:03:42 2009
// generator compiled at Jul 30 2009 08:02:36\n
///////////////////////////////////////////////////////////////////////////////
#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <iostream>
#include <cmath>
#include "../common.hh"

template <typename T> struct cmp_ {
        bool operator () (T lhs, T rhs) {
                return lhs == rhs;
        }
};
template <> struct cmp_<float> {
        bool operator () (float lhs, float rhs) {
                return fabsf (lhs-rhs) < 0.001f;
        }
};
template <> struct cmp_<double> {
        bool operator () (double lhs, double rhs) {
                return fabs (lhs-rhs) < 0.001;
        }
};
template <typename T> bool cmp (T lhs, T rhs) {
        return cmp_<T>()(lhs,rhs);
}

// helpers to print types nicely
template <typename T> struct Typename;
template <> struct Typename<uint8_t> { static const char* name () { return "uint8_t"; } };
template <> struct Typename<uint16_t> { static const char* name () { return "uint16_t"; } };
template <> struct Typename<uint32_t> { static const char* name () { return "uint32_t"; } };
template <> struct Typename<uint64_t> { static const char* name () { return "uint64_t"; } };
template <> struct Typename<int8_t> { static const char* name () { return "int8_t"; } };
template <> struct Typename<int16_t> { static const char* name () { return "int16_t"; } };
template <> struct Typename<int32_t> { static const char* name () { return "int32_t"; } };
template <> struct Typename<int64_t> { static const char* name () { return "int64_t"; } };
template <> struct Typename<float> { static const char* name () { return "float"; } };
template <> struct Typename<double> { static const char* name () { return "double"; } };
#define MAKE_STRING(T) Typename<T>::name()

// so that char-types will be printed as numbers
template <typename T> T printable (T value) {
        return value;
}
int printable (int8_t value) { return value; }
int printable (uint8_t value) { return value; }

// validator
template <
        typename T, 
        typename kallisto::random::make_int_if_float<T>::type min,
        typename kallisto::random::make_int_if_float<T>::type max
> struct Test {
        Test (size_t count, const T *testData, int seed) {
                using kallisto::random::MersenneTwister;
                MersenneTwister<T,min,max> mt (seed);
                std::vector<T> mtVals;
                bool fail = false;
                for (size_t i=0; i<count; ++i) {
                        T const val = mt.rand();
                        mtVals.push_back (val);
                        if (!cmp (val, testData[i]))
                                fail = true;
                }
                if (fail) {
                        std::cout << "---------------------------------------------------------\n";
                        std::cout << "Regression in kallisto::random::MersenneTwister detected!\n";
                        std::cout
                                << "T=" << MAKE_STRING(T) << ", "
                                << "seed=" << seed << ", " 
                                << "test size=" << count << "\n";
                        std::cout << " original values: ";
                        for (size_t i=0; i<count; ++i)
                                std::cout << printable(testData[i]) << " ";
                        std::cout << '\n';
                        std::cout << " regressed values: ";
                        for (size_t i=0; i<mtVals.size(); ++i)
                                std::cout << printable(mtVals[i]) << " ";
                        std::cout << '\n';
                        std::cout << "---------------------------------------------------------\n";
                } else {
                        std::cout << "Nice test for ";
                        std::cout
                                << "T=" << MAKE_STRING(T) << ", "
                                << "seed=" << printable(seed) << ", " 
                                << "min=" << printable(min) << ", " 
                                << "max=" << printable(max) << ", " 
                                << "test size=" << count << "\n";
                }
        }
};

int main () {
        std::cout << "random number generation regression tests\n";
        std::cout << "file generated at Thu Jul 30 08:03:42 2009\n";
        std::cout << "generator compiled at Jul 30 2009 08:02:36\n";
}


const size_t check_uint32_t__seed0__size32__min0__max255 = 32;
const uint32_t uint32_t__seed0__size32__min0__max255 [32] = {
        24U, 47U, 110U, 176U,
        52U, 183U, 22U, 177U,
        216U, 63U, 74U, 196U,
        224U, 152U, 200U, 29U,
        237U, 139U, 196U, 244U,
        149U, 159U, 14U, 245U,
        34U, 201U, 217U, 172U,
        36U, 172U, 44U, 75U
};
Test<
  uint32_t, 
  0U, 
  255U
> doTestuint32_t__seed0__size32__min0__max255 (32, uint32_t__seed0__size32__min0__max255, 0);

const size_t check_uint32_t__seed1__size32__min0__max255 = 32;
const uint32_t uint32_t__seed1__size32__min0__max255 [32] = {
        213U, 254U, 112U, 221U,
        0U, 65U, 154U, 255U,
        75U, 120U, 47U, 203U,
        95U, 198U, 176U, 86U,
        203U, 222U, 19U, 177U,
        214U, 160U, 94U, 12U,
        104U, 227U, 193U, 117U,
        14U, 17U, 87U, 211U
};
Test<
  uint32_t, 
  0U, 
  255U
> doTestuint32_t__seed1__size32__min0__max255 (32, uint32_t__seed1__size32__min0__max255, 1);

const size_t check_uint32_t__seed2__size32__min0__max255 = 32;
const uint32_t uint32_t__seed2__size32__min0__max255 [32] = {
        223U, 94U, 13U, 220U,
        25U, 229U, 222U, 248U,
        215U, 164U, 169U, 79U,
        104U, 101U, 61U, 61U,
        153U, 248U, 136U, 67U,
        62U, 162U, 14U, 93U,
        68U, 255U, 6U, 44U,
        94U, 112U, 146U, 132U
};
Test<
  uint32_t, 
  0U, 
  255U
> doTestuint32_t__seed2__size32__min0__max255 (32, uint32_t__seed2__size32__min0__max255, 2);

const size_t check_int32_t__seed0__size32__min0__max255 = 32;
const int32_t int32_t__seed0__size32__min0__max255 [32] = {
        24, 47, 110, 176,
        52, 183, 22, 177,
        216, 63, 74, 196,
        224, 152, 200, 29,
        237, 139, 196, 244,
        149, 159, 14, 245,
        34, 201, 217, 172,
        36, 172, 44, 75
};
Test<
  int32_t, 
  0, 
  255
> doTestint32_t__seed0__size32__min0__max255 (32, int32_t__seed0__size32__min0__max255, 0);

const size_t check_int32_t__seed1__size32__min0__max255 = 32;
const int32_t int32_t__seed1__size32__min0__max255 [32] = {
        213, 254, 112, 221,
        0, 65, 154, 255,
        75, 120, 47, 203,
        95, 198, 176, 86,
        203, 222, 19, 177,
        214, 160, 94, 12,
        104, 227, 193, 117,
        14, 17, 87, 211
};
Test<
  int32_t, 
  0, 
  255
> doTestint32_t__seed1__size32__min0__max255 (32, int32_t__seed1__size32__min0__max255, 1);

const size_t check_int32_t__seed2__size32__min0__max255 = 32;
const int32_t int32_t__seed2__size32__min0__max255 [32] = {
        223, 94, 13, 220,
        25, 229, 222, 248,
        215, 164, 169, 79,
        104, 101, 61, 61,
        153, 248, 136, 67,
        62, 162, 14, 93,
        68, 255, 6, 44,
        94, 112, 146, 132
};
Test<
  int32_t, 
  0, 
  255
> doTestint32_t__seed2__size32__min0__max255 (32, int32_t__seed2__size32__min0__max255, 2);

const size_t check_uint8_t__seed122__size32__min0__max1 = 32;
const uint8_t uint8_t__seed122__size32__min0__max1 [32] = {
        0U, 1U, 0U, 1U,
        1U, 1U, 1U, 0U,
        1U, 0U, 0U, 1U,
        0U, 1U, 0U, 0U,
        0U, 0U, 0U, 1U,
        1U, 1U, 1U, 0U,
        0U, 0U, 1U, 0U,
        0U, 1U, 0U, 1U
};
Test<
  uint8_t, 
  0U, 
  1U
> doTestuint8_t__seed122__size32__min0__max1 (32, uint8_t__seed122__size32__min0__max1, 122);

const size_t check_uint8_t__seed125__size32__min100__max101 = 32;
const uint8_t uint8_t__seed125__size32__min100__max101 [32] = {
        100U, 101U, 100U, 100U,
        100U, 101U, 100U, 100U,
        100U, 101U, 101U, 100U,
        100U, 100U, 101U, 100U,
        100U, 100U, 101U, 100U,
        101U, 100U, 100U, 101U,
        101U, 100U, 100U, 100U,
        101U, 101U, 100U, 101U
};
Test<
  uint8_t, 
  100U, 
  101U
> doTestuint8_t__seed125__size32__min100__max101 (32, uint8_t__seed125__size32__min100__max101, 125);

const size_t check_uint8_t__seed11__size32__min150__max255 = 32;
const uint8_t uint8_t__seed11__size32__min150__max255 [32] = {
        236U, 248U, 167U, 244U,
        209U, 156U, 235U, 229U,
        169U, 161U, 201U, 171U,
        214U, 224U, 181U, 170U,
        164U, 206U, 156U, 181U,
        175U, 168U, 246U, 233U,
        221U, 182U, 233U, 185U,
        178U, 175U, 234U, 166U
};
Test<
  uint8_t, 
  150U, 
  255U
> doTestuint8_t__seed11__size32__min150__max255 (32, uint8_t__seed11__size32__min150__max255, 11);

const size_t check_uint16_t__seed1111__size32__min0__max1 = 32;
const uint16_t uint16_t__seed1111__size32__min0__max1 [32] = {
        0U, 1U, 1U, 1U,
        1U, 1U, 1U, 0U,
        0U, 0U, 0U, 0U,
        0U, 1U, 0U, 0U,
        1U, 1U, 1U, 0U,
        0U, 0U, 0U, 1U,
        1U, 0U, 0U, 1U,
        1U, 1U, 0U, 0U
};
Test<
  uint16_t, 
  0U, 
  1U
> doTestuint16_t__seed1111__size32__min0__max1 (32, uint16_t__seed1111__size32__min0__max1, 1111);

const size_t check_uint16_t__seed2222__size32__min0__max65535 = 32;
const uint16_t uint16_t__seed2222__size32__min0__max65535 [32] = {
        29779U, 54112U, 37877U, 37466U,
        45821U, 14672U, 29761U, 21764U,
        27451U, 21519U, 17080U, 55885U,
        49615U, 19311U, 46836U, 19179U,
        64486U, 64544U, 33875U, 15862U,
        5036U, 62391U, 50606U, 60751U,
        20456U, 31625U, 38088U, 31859U,
        52380U, 59791U, 29137U, 6006U
};
Test<
  uint16_t, 
  0U, 
  65535U
> doTestuint16_t__seed2222__size32__min0__max65535 (32, uint16_t__seed2222__size32__min0__max65535, 2222);

const size_t check_uint16_t__seed12154__size32__min150__max155 = 32;
const uint16_t uint16_t__seed12154__size32__min150__max155 [32] = {
        152U, 153U, 151U, 150U,
        154U, 152U, 151U, 151U,
        155U, 150U, 151U, 150U,
        150U, 154U, 154U, 153U,
        152U, 150U, 153U, 150U,
        150U, 152U, 150U, 153U,
        151U, 150U, 152U, 152U,
        151U, 150U, 152U, 153U
};
Test<
  uint16_t, 
  150U, 
  155U
> doTestuint16_t__seed12154__size32__min150__max155 (32, uint16_t__seed12154__size32__min150__max155, 12154);

const size_t check_uint32_t__seed115611__size32__min10__max11 = 32;
const uint32_t uint32_t__seed115611__size32__min10__max11 [32] = {
        10U, 10U, 10U, 10U,
        10U, 10U, 10U, 11U,
        11U, 10U, 11U, 10U,
        10U, 10U, 10U, 11U,
        11U, 11U, 11U, 11U,
        10U, 11U, 11U, 11U,
        11U, 11U, 10U, 11U,
        11U, 10U, 10U, 11U
};
Test<
  uint32_t, 
  10U, 
  11U
> doTestuint32_t__seed115611__size32__min10__max11 (32, uint32_t__seed115611__size32__min10__max11, 115611);

const size_t check_uint32_t__seed256222__size32__min0__max4294967295 = 32;
const uint32_t uint32_t__seed256222__size32__min0__max4294967295 [32] = {
        496746515U, 2305976472U, 3117775529U, 3873969104U,
        567624088U, 484569067U, 2198961277U, 2961964126U,
        2955692891U, 3932789489U, 2478066366U, 1688988504U,
        766499218U, 2297655990U, 3247490721U, 2249782601U,
        2271638996U, 3838067332U, 2970551749U, 689631824U,
        1941216650U, 1876789420U, 2208716321U, 3994087787U,
        3920969559U, 2461205685U, 2954718377U, 3942398315U,
        3136270095U, 3619456397U, 96872294U, 1805318654U
};
Test<
  uint32_t, 
  0U, 
  4294967295U
> doTestuint32_t__seed256222__size32__min0__max4294967295 (32, uint32_t__seed256222__size32__min0__max4294967295, 256222);

const size_t check_uint32_t__seed1215564__size32__min0__max64 = 32;
const uint32_t uint32_t__seed1215564__size32__min0__max64 [32] = {
        1U, 52U, 50U, 37U,
        24U, 16U, 11U, 17U,
        21U, 29U, 50U, 42U,
        39U, 9U, 45U, 1U,
        31U, 38U, 13U, 0U,
        64U, 45U, 50U, 48U,
        38U, 12U, 61U, 32U,
        53U, 8U, 25U, 54U
};
Test<
  uint32_t, 
  0U, 
  64U
> doTestuint32_t__seed1215564__size32__min0__max64 (32, uint32_t__seed1215564__size32__min0__max64, 1215564);

const size_t check_uint64_t__seed1111__size32__min0__max2 = 32;
const uint64_t uint64_t__seed1111__size32__min0__max2 [32] = {
        0UL, 2UL, 2UL, 2UL,
        0UL, 1UL, 1UL, 1UL,
        0UL, 2UL, 0UL, 0UL,
        0UL, 1UL, 2UL, 0UL,
        0UL, 1UL, 2UL, 2UL,
        2UL, 0UL, 2UL, 1UL,
        2UL, 1UL, 0UL, 2UL,
        0UL, 1UL, 2UL, 0UL
};
Test<
  uint64_t, 
  0UL, 
  2UL
> doTestuint64_t__seed1111__size32__min0__max2 (32, uint64_t__seed1111__size32__min0__max2, 1111);

const size_t check_uint64_t__seed3281697__size32__min0__max18446744073709551615 = 32;
const uint64_t uint64_t__seed3281697__size32__min0__max18446744073709551615 [32] = {
        16111552748647641413UL, 9565515815503430026UL, 5139001655140205652UL, 12095799295281398769UL,
        16788145869816738682UL, 13115893105440410066UL, 16330819859271072595UL, 17375836394331888155UL,
        12100069784879411153UL, 11094058120205107863UL, 18157669113748007305UL, 3038720421895599440UL,
        13177148200664737288UL, 15000055430892291758UL, 9330173771814577054UL, 6877940196116653959UL,
        97578727273340777UL, 5770314586537725845UL, 6254698265766475014UL, 14805656994951942853UL,
        11559579567532214708UL, 9460578370458946780UL, 4462084473937513015UL, 4550268082197673798UL,
        11830316938148179409UL, 7903504457589808555UL, 15804461544799031608UL, 5878482967214147655UL,
        10025109643247143670UL, 18370348924771590808UL, 256155115975517265UL, 3309237145050398592UL
};
Test<
  uint64_t, 
  0UL, 
  18446744073709551615UL
> doTestuint64_t__seed3281697__size32__min0__max18446744073709551615 (32, uint64_t__seed3281697__size32__min0__max18446744073709551615, 3281697);

const size_t check_uint64_t__seed5465464__size32__min18446744073709551614__max18446744073709551615 = 32;
const uint64_t uint64_t__seed5465464__size32__min18446744073709551614__max18446744073709551615 [32] = {
        18446744073709551615UL, 18446744073709551614UL, 18446744073709551614UL, 18446744073709551614UL,
        18446744073709551615UL, 18446744073709551614UL, 18446744073709551615UL, 18446744073709551614UL,
        18446744073709551615UL, 18446744073709551614UL, 18446744073709551614UL, 18446744073709551615UL,
        18446744073709551615UL, 18446744073709551614UL, 18446744073709551615UL, 18446744073709551615UL,
        18446744073709551614UL, 18446744073709551614UL, 18446744073709551614UL, 18446744073709551614UL,
        18446744073709551614UL, 18446744073709551615UL, 18446744073709551615UL, 18446744073709551614UL,
        18446744073709551615UL, 18446744073709551614UL, 18446744073709551615UL, 18446744073709551614UL,
        18446744073709551615UL, 18446744073709551615UL, 18446744073709551614UL, 18446744073709551615UL
};
Test<
  uint64_t, 
  18446744073709551614UL, 
  18446744073709551615UL
> doTestuint64_t__seed5465464__size32__min18446744073709551614__max18446744073709551615 (32, uint64_t__seed5465464__size32__min18446744073709551614__max18446744073709551615, 5465464);

const size_t check_int8_t__seed122__size32__min0__max1 = 32;
const int8_t int8_t__seed122__size32__min0__max1 [32] = {
        0, 1, 0, 1,
        1, 1, 1, 0,
        1, 0, 0, 1,
        0, 1, 0, 0,
        0, 0, 0, 1,
        1, 1, 1, 0,
        0, 0, 1, 0,
        0, 1, 0, 1
};
Test<
  int8_t, 
  0, 
  1
> doTestint8_t__seed122__size32__min0__max1 (32, int8_t__seed122__size32__min0__max1, 122);

const size_t check_int8_t__seed125__size32__min100__max101 = 32;
const int8_t int8_t__seed125__size32__min100__max101 [32] = {
        100, 101, 100, 100,
        100, 101, 100, 100,
        100, 101, 101, 100,
        100, 100, 101, 100,
        100, 100, 101, 100,
        101, 100, 100, 101,
        101, 100, 100, 100,
        101, 101, 100, 101
};
Test<
  int8_t, 
  100, 
  101
> doTestint8_t__seed125__size32__min100__max101 (32, int8_t__seed125__size32__min100__max101, 125);

const size_t check_int8_t__seed11__size32__minm106__maxm1 = 32;
const int8_t int8_t__seed11__size32__minm106__maxm1 [32] = {
        -20, -8, -89, -12,
        -47, -100, -21, -27,
        -87, -95, -55, -85,
        -42, -32, -75, -86,
        -92, -50, -100, -75,
        -81, -88, -10, -23,
        -35, -74, -23, -71,
        -78, -81, -22, -90
};
Test<
  int8_t, 
  -106, 
  -1
> doTestint8_t__seed11__size32__minm106__maxm1 (32, int8_t__seed11__size32__minm106__maxm1, 11);

const size_t check_int16_t__seed1111__size32__min0__max1 = 32;
const int16_t int16_t__seed1111__size32__min0__max1 [32] = {
        0, 1, 1, 1,
        1, 1, 1, 0,
        0, 0, 0, 0,
        0, 1, 0, 0,
        1, 1, 1, 0,
        0, 0, 0, 1,
        1, 0, 0, 1,
        1, 1, 0, 0
};
Test<
  int16_t, 
  0, 
  1
> doTestint16_t__seed1111__size32__min0__max1 (32, int16_t__seed1111__size32__min0__max1, 1111);

const size_t check_int16_t__seed2222__size32__min0__max32767 = 32;
const int16_t int16_t__seed2222__size32__min0__max32767 [32] = {
        14889, 27056, 18938, 18733,
        22910, 7336, 14880, 10882,
        13725, 10759, 8540, 27942,
        24807, 9655, 23418, 9589,
        32243, 32272, 16937, 7931,
        2518, 31195, 25303, 30375,
        10228, 15812, 19044, 15929,
        26190, 29895, 14568, 3003
};
Test<
  int16_t, 
  0, 
  32767
> doTestint16_t__seed2222__size32__min0__max32767 (32, int16_t__seed2222__size32__min0__max32767, 2222);

const size_t check_int16_t__seed12154__size32__min150__max155 = 32;
const int16_t int16_t__seed12154__size32__min150__max155 [32] = {
        152, 153, 151, 150,
        154, 152, 151, 151,
        155, 150, 151, 150,
        150, 154, 154, 153,
        152, 150, 153, 150,
        150, 152, 150, 153,
        151, 150, 152, 152,
        151, 150, 152, 153
};
Test<
  int16_t, 
  150, 
  155
> doTestint16_t__seed12154__size32__min150__max155 (32, int16_t__seed12154__size32__min150__max155, 12154);

const size_t check_int32_t__seed115611__size32__min10__max11 = 32;
const int32_t int32_t__seed115611__size32__min10__max11 [32] = {
        10, 10, 10, 10,
        10, 10, 10, 11,
        11, 10, 11, 10,
        10, 10, 10, 11,
        11, 11, 11, 11,
        10, 11, 11, 11,
        11, 11, 10, 11,
        11, 10, 10, 11
};
Test<
  int32_t, 
  10, 
  11
> doTestint32_t__seed115611__size32__min10__max11 (32, int32_t__seed115611__size32__min10__max11, 115611);

const size_t check_int32_t__seed256222__size32__min0__max2147483647 = 32;
const int32_t int32_t__seed256222__size32__min0__max2147483647 [32] = {
        496746515, 567624088, 484569067, 1688988504,
        766499218, 689631824, 1941216650, 1876789420,
        96872294, 1805318654, 912591955, 704193991,
        1855711236, 1836567265, 1139871657, 500840548,
        1833802054, 1339397869, 494936831, 1807122807,
        1306751419, 56548751, 1862659806, 1808987219,
        1881384032, 1508308942, 1687277900, 1990510312,
        2121272781, 371945498, 783820049, 1968895812
};
Test<
  int32_t, 
  0, 
  2147483647
> doTestint32_t__seed256222__size32__min0__max2147483647 (32, int32_t__seed256222__size32__min0__max2147483647, 256222);

const size_t check_int32_t__seed1215564__size32__min0__max64 = 32;
const int32_t int32_t__seed1215564__size32__min0__max64 [32] = {
        1, 52, 50, 37,
        24, 16, 11, 17,
        21, 29, 50, 42,
        39, 9, 45, 1,
        31, 38, 13, 0,
        64, 45, 50, 48,
        38, 12, 61, 32,
        53, 8, 25, 54
};
Test<
  int32_t, 
  0, 
  64
> doTestint32_t__seed1215564__size32__min0__max64 (32, int32_t__seed1215564__size32__min0__max64, 1215564);

const size_t check_int64_t__seed1111__size32__min0__max2 = 32;
const int64_t int64_t__seed1111__size32__min0__max2 [32] = {
        0, 2, 2, 2,
        0, 1, 1, 1,
        0, 2, 0, 0,
        0, 1, 2, 0,
        0, 1, 2, 2,
        2, 0, 2, 1,
        2, 1, 0, 2,
        0, 1, 2, 0
};
Test<
  int64_t, 
  0, 
  2
> doTestint64_t__seed1111__size32__min0__max2 (32, int64_t__seed1111__size32__min0__max2, 1111);

const size_t check_int64_t__seed51729492__size32__min0__max9223372036854775807 = 32;
const int64_t int64_t__seed51729492__size32__min0__max9223372036854775807 [32] = {
        5197167815737608601, 5164524729334434094, 3663646747667417275, 5060667642041351999,
        882749514974975960, 6261971503330264717, 2399901742015484673, 269988516761156170,
        4690267839488063322, 6630091603292088620, 1039314893684690491, 8328329595881629422,
        3089682114025686429, 5827846408914866943, 106883058218154311, 1198125754605348215,
        2991036216608958406, 7149462651461468107, 4873175549045494385, 3950714267424691445,
        7670964822948388382, 8426763619805119461, 620147058072304323, 8463436440432129541,
        7948085154830455075, 3800424033198688031, 5457250117209484195, 6008949995949655054,
        5491555309704885795, 8061428501423977045, 4942430258250767386, 8178856860746683183
};
Test<
  int64_t, 
  0, 
  9223372036854775807
> doTestint64_t__seed51729492__size32__min0__max9223372036854775807 (32, int64_t__seed51729492__size32__min0__max9223372036854775807, 51729492);

const size_t check_int64_t__seed1211234__size32__minm2__maxm1 = 32;
const int64_t int64_t__seed1211234__size32__minm2__maxm1 [32] = {
        -1, -2, -2, -2,
        -2, -2, -1, -1,
        -2, -2, -2, -1,
        -1, -2, -1, -2,
        -2, -2, -2, -1,
        -2, -2, -1, -1,
        -2, -1, -1, -2,
        -2, -2, -1, -1
};
Test<
  int64_t, 
  -2, 
  -1
> doTestint64_t__seed1211234__size32__minm2__maxm1 (32, int64_t__seed1211234__size32__minm2__maxm1, 1211234);

const size_t check_float__seed12345534__size32__min0__max1 = 32;
const float float__seed12345534__size32__min0__max1 [32] = {
        0.648136f, 0.814149f, 0.845193f, 0.547943f,
        0.649909f, 0.407355f, 0.650733f, 0.785973f,
        0.993774f, 0.138152f, 0.464114f, 0.581016f,
        0.379079f, 0.782647f, 0.796834f, 0.319717f,
        0.606696f, 0.917901f, 0.43706f, 0.987394f,
        0.15163f, 0.300025f, 0.414716f, 0.0258305f,
        0.838192f, 0.585333f, 0.11662f, 0.0562539f,
        0.275202f, 0.507122f, 0.635163f, 0.00510158f
};
Test<
  float, 
  0, 
  1
> doTestfloat__seed12345534__size32__min0__max1 (32, float__seed12345534__size32__min0__max1, 12345534);

const size_t check_float__seed12341111__size32__min0__max10 = 32;
const float float__seed12341111__size32__min0__max10 [32] = {
        8.31008f, 6.00561f, 7.76405f, 1.26834f,
        2.58239f, 1.20501f, 8.26018f, 6.8916f,
        0.340639f, 8.14692f, 0.97025f, 0.367475f,
        5.08378f, 0.342068f, 7.9384f, 9.08837f,
        9.41778f, 1.225f, 1.24256f, 6.00689f,
        8.39403f, 5.26683f, 5.90173f, 6.39471f,
        3.09592f, 4.28341f, 0.221577f, 2.0749f,
        1.2438f, 4.36471f, 2.1253f, 2.959f
};
Test<
  float, 
  0, 
  10
> doTestfloat__seed12341111__size32__min0__max10 (32, float__seed12341111__size32__min0__max10, 12341111);

const size_t check_float__seed0__size32__min0__max255 = 32;
const float float__seed0__size32__min0__max255 [32] = {
        216.064f, 196.035f, 223.169f, 151.743f,
        28.9236f, 139.055f, 195.555f, 243.609f,
        244.788f, 200.32f, 36.2284f, 172.072f,
        44.4359f, 10.3114f, 187.803f, 71.5789f,
        241.54f, 235.354f, 60.32f, 73.1102f,
        54.0129f, 211.478f, 241.536f, 134.923f,
        95.0295f, 232.637f, 110.441f, 68.9613f,
        9.5828f, 165.312f, 76.3342f, 113.384f
};
Test<
  float, 
  0, 
  255
> doTestfloat__seed0__size32__min0__max255 (32, float__seed0__size32__min0__max255, 0);

const size_t check_float__seed1__size32__min0__max255 = 32;
const float float__seed1__size32__min0__max255 [32] = {
        212.681f, 0.0583344f, 65.3435f, 154.19f,
        74.8455f, 120.405f, 47.0927f, 202.256f,
        94.9927f, 197.834f, 176.236f, 202.351f,
        213.789f, 159.769f, 104.271f, 226.161f,
        117.084f, 13.9677f, 212.825f, 233.174f,
        219.656f, 71.5973f, 101.032f, 159.846f,
        47.3284f, 43.3725f, 19.9179f, 139.255f,
        86.6135f, 30.214f, 50.1569f, 214.765f
};
Test<
  float, 
  0, 
  255
> doTestfloat__seed1__size32__min0__max255 (32, float__seed1__size32__min0__max255, 1);

const size_t check_float__seed2__size32__min0__max255 = 32;
const float float__seed2__size32__min0__max255 [32] = {
        222.357f, 94.3919f, 13.2224f, 222.014f,
        247.222f, 214.388f, 163.474f, 168.471f,
        78.7576f, 104.371f, 61.1748f, 152.824f,
        247.44f, 136.082f, 68.6358f, 254.266f,
        94.0643f, 131.834f, 252.061f, 207.727f,
        90.2622f, 40.6192f, 151.479f, 33.2961f,
        146.813f, 218.342f, 59.2586f, 49.2308f,
        92.6808f, 64.8516f, 252.088f, 115.266f
};
Test<
  float, 
  0, 
  255
> doTestfloat__seed2__size32__min0__max255 (32, float__seed2__size32__min0__max255, 2);

const size_t check_double__seed12345534__size32__min0__max1 = 32;
const double double__seed12345534__size32__min0__max1 [32] = {
        0.814149, 0.845193, 0.547942, 0.407355,
        0.650733, 0.785973, 0.138152, 0.464114,
        0.379079, 0.782647, 0.319717, 0.917901,
        0.987394, 0.300025, 0.0258305, 0.838192,
        0.585332, 0.0562539, 0.275202, 0.635163,
        0.321172, 0.296308, 0.88829, 0.434816,
        0.876935, 0.190707, 0.907568, 0.258934,
        0.0894648, 0.207122, 0.0447544, 0.80095
};
Test<
  double, 
  0L, 
  1L
> doTestdouble__seed12345534__size32__min0__max1 (32, double__seed12345534__size32__min0__max1, 12345534);

const size_t check_double__seed12341111__size32__min0__max10 = 32;
const double double__seed12341111__size32__min0__max10 [32] = {
        8.31008, 7.76405, 2.58239, 1.20501,
        6.8916, 0.340639, 0.97025, 5.08378,
        7.9384, 9.41778, 1.225, 1.24256,
        6.00689, 8.39403, 5.90173, 6.39471,
        3.09592, 4.28341, 0.221577, 2.0749,
        1.2438, 4.36471, 2.959, 6.23543,
        1.92301, 7.29761, 3.02919, 9.36297,
        1.42604, 8.84339, 4.58186, 5.32166
};
Test<
  double, 
  0L, 
  10L
> doTestdouble__seed12341111__size32__min0__max10 (32, double__seed12341111__size32__min0__max10, 12341111);

const size_t check_double__seed0__size32__min0__max255 = 32;
const double double__seed0__size32__min0__max255 [32] = {
        216.064, 196.035, 151.743, 28.9236,
        139.055, 243.609, 244.788, 200.32,
        36.2284, 44.4359, 10.3114, 71.5789,
        241.54, 235.354, 60.32, 73.1102,
        54.0129, 241.536, 95.0295, 232.637,
        68.9613, 165.312, 76.3342, 113.384,
        197.109, 229.474, 222.886, 50.633,
        87.1639, 182.658, 160.868, 185.492
};
Test<
  double, 
  0L, 
  255L
> doTestdouble__seed0__size32__min0__max255 (32, double__seed0__size32__min0__max255, 0);

const size_t check_double__seed1__size32__min0__max255 = 32;
const double double__seed1__size32__min0__max255 [32] = {
        0.0583344, 154.19, 74.8455, 47.0927,
        94.9927, 176.236, 202.351, 213.789,
        104.271, 117.084, 212.825, 219.656,
        101.032, 159.846, 43.3725, 19.9179,
        86.6135, 50.1569, 214.765, 210.012,
        100.751, 147.711, 72.4813, 210.395,
        17.4274, 152.233, 227.529, 113.284,
        37.4157, 239.312, 49.0479, 146.765
};
Test<
  double, 
  0L, 
  255L
> doTestdouble__seed1__size32__min0__max255 (32, double__seed1__size32__min0__max255, 1);

const size_t check_double__seed2__size32__min0__max255 = 32;
const double double__seed2__size32__min0__max255 [32] = {
        94.3919, 222.014, 214.388, 168.471,
        104.371, 61.1748, 247.44, 68.6358,
        94.0643, 131.834, 252.061, 90.2622,
        151.479, 146.813, 59.2586, 92.6808,
        252.088, 115.266, 54.5423, 112.356,
        178.411, 238.572, 102.889, 246.366,
        197.315, 60.9372, 75.768, 110.808,
        174.223, 131.954, 222.741, 238.6
};
Test<
  double, 
  0L, 
  255L
> doTestdouble__seed2__size32__min0__max255 (32, double__seed2__size32__min0__max255, 2);
