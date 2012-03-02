#ifndef CONSTANT_H_INCLUDED_20120215
#define CONSTANT_H_INCLUDED_20120215

#include "Typename.h"
#include <memory>
#include <string>

namespace quatsch { namespace compiler { namespace phase3 {

class Constant;
typedef std::shared_ptr<Constant> ConstantPtr;

class Constant {
public:
        static ConstantPtr Floating (std::string const &name, float value);
        static ConstantPtr Integer  (std::string const &name, int value);

        Typename type() const;

        std::string name() const;

        float floating () const;
        int   integer  () const;

private:
        Constant (Typename t, std::string const &name);
        void assert_type (Typename t, std::string const &message) const;

private:
        Typename type_;
        std::string name_;
        float floating_;
        int integer_;
};

} } }

#endif // CONSTANT_H_INCLUDED_20120215

