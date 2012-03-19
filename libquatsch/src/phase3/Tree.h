#ifndef TREE_H_INCLUDED_20120202
#define TREE_H_INCLUDED_20120202

#include "phase2/Tree.h"
#include "phase2/Argument.h"
#include <boost/optional.hpp>
#include <memory>
#include <list>
#include <iosfwd>

namespace quatsch { namespace compiler { namespace phase3 {

        class Defun;
        typedef std::shared_ptr<Defun> DefunPtr;

        class Tree;
        typedef std::shared_ptr<Tree> TreePtr;
        typedef std::list<TreePtr>    OperandList;

        class Tree {
        public:
                enum class Type {
                        StackRef,
                        Integer, Floating,
                        Call,
                        TemplateCall,
                        ExternCall,
                        Builtin     // e.g. '*'
                };

                static TreePtr StackRef (size_t index, phase2::Argument const&);
                static TreePtr Integer  (int value);
                static TreePtr Floating (float value);
                static TreePtr Builtin  (phase2::BuiltinPtr, OperandList);
                static TreePtr Call     (DefunPtr, OperandList);

                Type type() const;
                Typename expression_type() const;

                int integer() const;
                float floating() const;

                size_t stackref_index() const;
                phase2::Argument stackref_argument() const;

                DefunPtr call_callee() const;
                OperandList call_args() const;

                phase2::BuiltinPtr builtin() const;
                OperandList builtin_args() const;
        private:
                Tree (Type type, Typename etype) :
                        type_(type), expression_type_ (etype) {}

                void assert_type (Type type, const char *message) const;

        private:
                Type type_;
                Typename expression_type_;

                size_t stackvar_index_;
                boost::optional<phase2::Argument> stackvar_arg_;
                int    integer_value_;
                float  floating_value_;

                phase2::BuiltinPtr builtin_;
                phase3::DefunPtr callee_;
                OperandList operands_;
        };
} } }

#endif // TREE_H_INCLUDED_20120202
