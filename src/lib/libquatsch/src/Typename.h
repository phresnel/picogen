#ifndef TYPENAME_H_INCLUDED_20120210
#define TYPENAME_H_INCLUDED_20120210

#include <string>
#include <stdexcept>

namespace quatsch {

enum class Typename {
        Integer,
        Float
};

inline std::string to_string (Typename t) {
        switch (t) {
        case Typename::Integer: return "Integer";
        case Typename::Float:   return "Float";
        }
        throw std::runtime_error ("switch-case not implemented in to_string(Typename)");
}

}

#endif // TYPENAME_H_INCLUDED_20120210
