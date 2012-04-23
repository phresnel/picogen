#ifndef ARGUMENT_H_INCLUDED_20120210
#define ARGUMENT_H_INCLUDED_20120210

#include "Typename.h"
#include "code_iterator.h"
#include <string>

namespace quatsch { namespace compiler { namespace phase2 {

class Argument {
public:
        Argument (std::string const &name, code_iterator from, code_iterator to) ;
        Argument (std::string const &name, Typename type, code_iterator from, code_iterator to) ;

        void reset_type (Typename type) ;
        Typename type() const ;

        std::string name () const ;

        code_iterator code_begin() const;
        code_iterator code_end() const;
private:
        std::string name_;
        Typename type_;
        code_iterator code_begin_, code_end_;
};

} } }

#endif // ARGUMENT_H_INCLUDED_20120210
