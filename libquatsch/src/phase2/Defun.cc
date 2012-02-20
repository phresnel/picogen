#include "Defun.h"

namespace quatsch { namespace compiler { namespace phase2 {

Defun::Defun (std::string const &name,
              argument_list const &args,
              Typename returnType,
              TreePtr body) :
        name_(name),
        args_(args),
        return_type_(returnType),
        body_(body)
{
}


std::string Defun::name() const
{
        return name_;
}

Defun::argument_list const& Defun::arguments() const
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


Typename Defun::return_type() const
{
        return return_type_;
}

} } }
