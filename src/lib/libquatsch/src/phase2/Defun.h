#ifndef DEFUN_H_INCLUDED_20120125
#define DEFUN_H_INCLUDED_20120125

#include "Tree.h"
#include "Argument.h"
#include "Typename.h"
#include <list>
#include <string>

namespace quatsch { namespace compiler { namespace phase2 {


class Defun {
public:
        typedef std::list<Argument> argument_list;

        Defun (std::string const &name,
               argument_list const &args,
               Typename returnType,
               TreePtr body,
               code_iterator begin, code_iterator end);

        std::string           name       () const;
        argument_list const&  arguments  () const;
        TreePtr               body       () const;
        Typename              return_type() const;

        size_t                arg_count  () const;

        code_iterator         code_begin () const;
        code_iterator         code_end   () const;
private:
        std::string name_;
        argument_list args_;
        Typename return_type_;
        TreePtr body_;

        code_iterator code_begin_, code_end_;
};

typedef std::shared_ptr<Defun> DefunPtr;


} } }

#endif // DEFUN_H_INCLUDED_20120125
