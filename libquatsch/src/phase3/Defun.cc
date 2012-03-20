#include "Defun.h"
#include "Tree.h"

namespace quatsch { namespace compiler { namespace phase3 {

Defun::Defun (std::string const &name,
       std::list<phase2::Argument> const &args,
       Typename return_type,
       TreePtr body)
        : name_(name),
          args_ (args),
          body_ (body),
          return_type_ (return_type)
{
}

DefunPtr Defun::ForwardDeclare (std::string name,
                                std::list<phase2::Argument> args,
                                Typename returnType)
{
        DefunPtr ret {new Defun()};
        ret->name_ = name;
        ret->args_ = args;
        ret->return_type_ = returnType;
        return ret;
}

void Defun::set_body (TreePtr body)
{
        body_ = body;
}

std::string Defun::name() const
{
        return name_;
}

std::list<phase2::Argument> Defun::arguments() const
{
        return args_;
}

TreePtr Defun::body() const
{
        return body_;
}

size_t Defun::arg_count() const
{
        return args_.size();
}

Typename Defun::return_type () const
{
        return return_type_;
}

Defun::Defun()
{
}

} } }
