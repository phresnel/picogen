#ifndef CONSTANT_H_INCLUDED_20120210
#define CONSTANT_H_INCLUDED_20120210

#include "Tree.h"
#include <memory>
#include <string>

namespace quatsch { namespace compiler { namespace phase2 {

// A constant with a to-be-verified constexpr.
class Constant;
typedef std::shared_ptr<Constant> ConstantPtr;

class Constant {
public:
        enum class Type {
                Integer,
                Floating
        };

        static ConstantPtr Floating (std::string const &name, TreePtr expr,
                                     code_iterator begin, code_iterator end);
        static ConstantPtr Integer  (std::string const &name, TreePtr expr,
                                     code_iterator begin, code_iterator end);

        std::string name() const;
        Type type() const;

        TreePtr expression() const;

        code_iterator code_begin() const;
        code_iterator code_end  () const;

private:
        Constant (Type type, std::string const &name, TreePtr expr,
                  code_iterator begin, code_iterator end);
        void assert_type (Type t, const char *message) const;

private:
        Type type_;
        std::string name_;
        TreePtr expression_;
        code_iterator code_begin_, code_end_;
};

} } }

#endif // CONSTANT_H_INCLUDED_20120210

