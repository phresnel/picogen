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


#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif



#include <sstream>
#include <iostream>

#include "../common.hh"


// is_signed
template <typename T> struct is_signed { enum { value = true }; };
template <> struct is_signed<uint8_t> { enum { value = false }; };
template <> struct is_signed<uint16_t> { enum { value = false }; };
template <> struct is_signed<uint32_t> { enum { value = false }; };
template <> struct is_signed<uint64_t> { enum { value = false }; };

// is_long
template <typename T> struct is_long { enum { value = false }; };
template <> struct is_long<uint64_t> { enum { value = true }; };
template <> struct is_long<double> { enum { value = true }; };

// is_float
template <typename T> struct is_float { enum { value = false }; };
template <> struct is_float<float> { enum { value = true }; };
template <> struct is_float<double> { enum { value = true }; };

// Typename -> name of type as string
template <typename T> struct Typename;
template <> struct Typename<uint8_t> { 
        static const char* name () { return "uint8_t"; }
};
template <> struct Typename<uint16_t> { 
        static const char* name () { return "uint16_t"; }
};
template <> struct Typename<uint32_t> { 
        static const char* name () { return "uint32_t"; }
};
template <> struct Typename<uint64_t> { 
        static const char* name () { return "uint64_t"; }
};

template <> struct Typename<int8_t> { 
        static const char* name () { return "int8_t"; }
};
template <> struct Typename<int16_t> { 
        static const char* name () { return "int16_t"; }
};
template <> struct Typename<int32_t> { 
        static const char* name () { return "int32_t"; }
};
template <> struct Typename<int64_t> { 
        static const char* name () { return "int64_t"; }
};

template <> struct Typename<float> { 
        static const char* name () { return "float"; }
};
template <> struct Typename<double> { 
        static const char* name () { return "double"; }
};

// todo: needed anymore?
#define MAKE_STRING(T) Typename<T>::name()


// when T is char, it would get printed as ascii through cout, not as value
template <typename T> T printable (T value) {
        return value;
}
int printable (int8_t value) { return value; }
int printable (uint8_t value) { return value; }



// dump a compilable rng test
template <
        typename T, 
        typename kallisto::random::make_int_if_float<T>::type min, 
        typename kallisto::random::make_int_if_float<T>::type max
>
void dump (uint32_t seed, size_t num) {
        using kallisto::random::MersenneTwister;
        typedef typename 
                kallisto::random::make_int_if_float<T>::type IFF;
        MersenneTwister<T, min, max> mt (seed);
        
        std::string id;
        {
                std::stringstream tmp;
                tmp
                        << MAKE_STRING(T) 
                        << "__seed" << printable(seed)        
                        << "__size" << printable(num);
                if (kallisto::int_cmp_le<IFF,0,min>::value) {
                        tmp << "__min" << printable(min);
                } else {
                        tmp << "__minm" << printable(-min);
                }
                if (kallisto::int_cmp_le<IFF,0,max>::value) {
                        tmp << "__max" << printable(max);
                } else {
                        tmp << "__maxm" << printable(-max);
                }
                tmp >> id;
        }
        std::cout
                << "\nconst size_t check_" 
                << id << " = " << printable(num)
                << ";\n";
        std::cout
                << "const " << MAKE_STRING(T) 
                << " " << id
                << " [" << num << "] = {";
        for (size_t i=0; i<num; ++i) {
                std::cout
                        << ((i%4==0)?"\n        ":" ")
                        << printable(mt.rand());
                if (is_float<T>::value) {
                        std::cout
                                << (is_long<T>::value ? "" : "f" );
                } else {
                        std::cout
                                << (is_signed<T>::value ? "" : "U" )
                                << (is_long<T>::value ? "L" : "" );
                };
                std::cout << ((i+1)!=num?",":"");                        
        }
        std::cout << "\n};\n";
        std::cout
                << "Test<" << '\n'
                << "  " << MAKE_STRING(T) << ", \n"
                ;
        std::cout
                << "  " << printable(min)
                << (is_signed<T>::value ? "" : "U" )
                << (is_long<T>::value ? "L" : "" )
                << ", \n"
                ;
        std::cout
                << "  " << printable(max)
                << (is_signed<T>::value ? "" : "U" )
                << (is_long<T>::value ? "L" : "" )
                << "\n"
                ;
        std::cout
                << "> "
                << "doTest" << id
                << " (" << num << ", " 
                << id << ", "
                << printable(seed)
                << ")"
                << ";\n";
}



// print compilable prerequisite for tests
void printTestSuite () {
        std::cout <<
        "///////////////////////////////////////////////////////////////////////////////\n"
        "// This file was generated automatically, manual edits might be overwritten\"\n"
        "// without any warning!\n"
        "///////////////////////////////////////////////////////////////////////////////\n"
        "// (C)2009 Sebastian Mach (*1983) / phresnel\n"
        "//  this file is licensed under GPL, version 3, or newer\n"
        "//  mail: com.gmail.phresnel , for nice mail swap \"com\" and \"phresnel\" \n"
        "//   web: http://picogen.org , http://phresnel.org \n"
        "///////////////////////////////////////////////////////////////////////////////\n"
        "// file generated at " 
        ;
        
        {
                time_t rawtime;
                struct tm * timeinfo;
                time (&rawtime);
                timeinfo = localtime (&rawtime);
                std::stringstream ss;
                ss << asctime (timeinfo);
                std::string tmp = ss.str();
                tmp.erase (tmp.find('\n'));
                std::cout << tmp;
        }

        std::cout << 
        "\n"
        "// generator compiled at " __DATE__ " " __TIME__ "\\n\n"
        ;
        
        std::cout <<
        "///////////////////////////////////////////////////////////////////////////////\n"
        "#define __STDC_LIMIT_MACROS\n"
        "#include <stdint.h>\n"
        "#include <iostream>\n"
        "#include <cmath>\n"
        "#include \"../common.hh\"\n"
        "\n"
        "template <typename T> struct cmp_ {\n"
        "        bool operator () (T lhs, T rhs) {\n"
        "                return lhs == rhs;\n"
        "        }\n"
        "};\n"
        "template <> struct cmp_<float> {\n"
        "        bool operator () (float lhs, float rhs) {\n"
        "                return fabsf (lhs-rhs) < 0.001f;\n"
        "        }\n"
        "};\n"
        "template <> struct cmp_<double> {\n"
        "        bool operator () (double lhs, double rhs) {\n"
        "                return fabs (lhs-rhs) < 0.001;\n"
        "        }\n"
        "};\n"
        "template <typename T> bool cmp (T lhs, T rhs) {\n"
        "        return cmp_<T>()(lhs,rhs);\n"
        "}\n"
        "\n"       
        "// helpers to print types nicely\n"
        "template <typename T> struct Typename;\n"
        "template <> struct Typename<uint8_t> { static const char* name () { return \"uint8_t\"; } };\n"
        "template <> struct Typename<uint16_t> { static const char* name () { return \"uint16_t\"; } };\n"
        "template <> struct Typename<uint32_t> { static const char* name () { return \"uint32_t\"; } };\n"
        "template <> struct Typename<uint64_t> { static const char* name () { return \"uint64_t\"; } };\n"
        "template <> struct Typename<int8_t> { static const char* name () { return \"int8_t\"; } };\n"
        "template <> struct Typename<int16_t> { static const char* name () { return \"int16_t\"; } };\n"
        "template <> struct Typename<int32_t> { static const char* name () { return \"int32_t\"; } };\n"
        "template <> struct Typename<int64_t> { static const char* name () { return \"int64_t\"; } };\n"
        "template <> struct Typename<float> { static const char* name () { return \"float\"; } };\n"
        "template <> struct Typename<double> { static const char* name () { return \"double\"; } };\n"
        "#define MAKE_STRING(T) Typename<T>::name()\n"
        "\n"
        "// so that char-types will be printed as numbers\n"
        "template <typename T> T printable (T value) {\n"
        "        return value;\n"
        "}\n"
        "int printable (int8_t value) { return value; }\n"
        "int printable (uint8_t value) { return value; }\n"        
        "\n"
        "// validator\n"
        "template <\n"
        "        typename T, \n"
        "        typename kallisto::random::make_int_if_float<T>::type min,\n"
        "        typename kallisto::random::make_int_if_float<T>::type max\n"
        "> struct Test {\n"
        "        Test (size_t count, const T *testData, int seed) {\n"
        "                using kallisto::random::MersenneTwister;\n"
        "                MersenneTwister<T,min,max> mt (seed);\n"
        "                std::vector<T> mtVals;\n"
        "                bool fail = false;\n"
        "                for (size_t i=0; i<count; ++i) {\n"
        "                        T const val = mt.rand();\n"
        "                        mtVals.push_back (val);\n"
        "                        if (!cmp (val, testData[i]))\n"
        "                                fail = true;\n"
        "                }\n"
        "                if (fail) {\n"
        "                        std::cout << \"---------------------------------------------------------\\n\";\n"
        "                        std::cout << \"Regression in kallisto::random::MersenneTwister detected!\\n\";\n"
        "                        std::cout\n"
        "                                << \"T=\" << MAKE_STRING(T) << \", \"\n"
        "                                << \"seed=\" << seed << \", \" \n"
        "                                << \"test size=\" << count << \"\\n\";\n"
        "                        std::cout << \" original values: \";\n"
        "                        for (size_t i=0; i<count; ++i)\n"
        "                                std::cout << printable(testData[i]) << \" \";\n"
        "                        std::cout << '\\n';\n"
        "                        std::cout << \" regressed values: \";\n"
        "                        for (size_t i=0; i<mtVals.size(); ++i)\n"
        "                                std::cout << printable(mtVals[i]) << \" \";\n"
        "                        std::cout << '\\n';\n"
        "                        std::cout << \"---------------------------------------------------------\\n\";\n"
        "                } else {\n"
        "                        std::cout << \"Nice test for \";\n"
        "                        std::cout\n"
        "                                << \"T=\" << MAKE_STRING(T) << \", \"\n"
        "                                << \"seed=\" << printable(seed) << \", \" \n"
        "                                << \"min=\" << printable(min) << \", \" \n"
        "                                << \"max=\" << printable(max) << \", \" \n"
        "                                << \"test size=\" << count << \"\\n\";\n"
        "                }\n"
        "        }\n"
        "};\n"
        "\n"
        "int main () {\n"
        "        std::cout << \"random number generation regression tests\\n\";\n"
        "        std::cout << \"file generated at " 
        ;
        
        {
                time_t rawtime;
                struct tm * timeinfo;
                time (&rawtime);
                timeinfo = localtime (&rawtime);
                std::stringstream ss;
                ss << asctime (timeinfo);
                std::string tmp = ss.str();
                tmp.erase (tmp.find('\n'));
                std::cout << tmp;
        }

        std::cout << "\\n\";\n";
        std::cout << 
        "        std::cout << \"generator compiled at " __DATE__ " " __TIME__ "\\n\";\n"
        "}\n\n";
}



int main () {        
        
        printTestSuite ();        

        // charge!
        
        dump<uint32_t, 0, 255> (0, 32);        
        dump<uint32_t, 0, 255> (1, 32);
        dump<uint32_t, 0, 255> (2, 32);
        
        dump<int32_t, 0, 255> (0, 32);
        dump<int32_t, 0, 255> (1, 32);
        dump<int32_t, 0, 255> (2, 32);

        dump<uint8_t, 0, 1> (122, 32);        
        dump<uint8_t, 100, 101> (125, 32);
        dump<uint8_t, 150, 255> (11, 32);
        
        dump<uint16_t, 0, 1> (1111, 32);        
        dump<uint16_t, 0, 0xFFFF> (2222, 32);
        dump<uint16_t, 150, 155> (12154, 32);
        
        dump<uint32_t, 10, 11> (115611, 32);        
        dump<uint32_t, 0, 0xFFFFFFFF> (256222, 32);
        dump<uint32_t, 0, 64> (1215564, 32);
        
        dump<uint64_t, 0, 2> (1111, 32);        
        dump<uint64_t, 0, 0xFFFFFFFFFFFFFFFFUL> (0x321321, 32);
        dump<uint64_t, 0xFFFFFFFFFFFFFFFEUL, 0xFFFFFFFFFFFFFFFFUL> (5465464, 32);
        
        dump<int8_t, 0, 1> (122, 32);        
        dump<int8_t, 100, 101> (125, 32);
        dump<int8_t, 150, 255> (11, 32);
        
        dump<int16_t, 0, 1> (1111, 32);
        dump<int16_t, 0, 32767> (2222, 32);
        dump<int16_t, 150, 155> (12154, 32);
        
        dump<int32_t, 10, 11> (115611, 32);        
        dump<int32_t, 0, kallisto::traits::integer_limits<int32_t>::max> (256222, 32);
        dump<int32_t, 0, 64> (1215564, 32);
        
        dump<int64_t, 0, 2> (1111, 32);        
        dump<int64_t, 0, kallisto::traits::integer_limits<int64_t>::max> (0x3155454L, 32);
        dump<int64_t, 0xFFFFFFFFFFFFFFFEL, 0xFFFFFFFFFFFFFFFFL> (1211234L, 32);
        
        dump<float,0,1> (12345534, 32);
        dump<float,0,10> (12341111, 32);
        dump<float,0,255> (0, 32);
        dump<float,0,255> (1, 32);
        dump<float,0,255> (2, 32);
        
        dump<double,0,1> (12345534, 32);
        dump<double,0,10> (12341111, 32);
        dump<double,0,255> (0, 32);
        dump<double,0,255> (1, 32);
        dump<double,0,255> (2, 32);
}
