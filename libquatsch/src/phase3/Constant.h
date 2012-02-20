#ifndef CONSTANT_H_INCLUDED_20120215
#define CONSTANT_H_INCLUDED_20120215

#include "phase2/Typename.h"
#include <memory>
#include <string>

namespace quatsch { namespace compiler { namespace phase3 {

class Constant;
typedef std::shared_ptr<Constant> ConstantPtr;

class Constant {
public:
        static ConstantPtr Floating (std::string const &name, float value);
        static ConstantPtr Integer  (std::string const &name, int value);

        phase2::Typename type() const;

        std::string name() const;

        float floating () const;
        int   integer  () const;

private:
        Constant (phase2::Typename t, std::string const &name);
        void assert_type (phase2::Typename t, std::string const &message) const;

private:
        phase2::Typename type_;
        std::string name_;
        float floating_;
        int integer_;
};

} } }

#endif // CONSTANT_H_INCLUDED_20120215

