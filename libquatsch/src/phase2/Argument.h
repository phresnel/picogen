#ifndef ARGUMENT_H_INCLUDED_20120210
#define ARGUMENT_H_INCLUDED_20120210

#include "Typename.h"
#include <string>

namespace quatsch { namespace compiler { namespace phase2 {

class Argument {
public:
        Argument (std::string const &name) ;
        Argument (std::string const &name, Typename type) ;

        void reset_type (Typename type) ;
        Typename type() const ;

        std::string name () const ;
private:
        std::string name_;
        Typename type_;
};

} } }

#endif // ARGUMENT_H_INCLUDED_20120210
