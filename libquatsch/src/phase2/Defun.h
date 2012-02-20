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
               TreePtr body);

        std::string                   name       () const;
        argument_list const&          arguments  () const;
        TreePtr                       body       () const;
        Typename                      return_type() const;

        size_t                        arg_count() const;
private:
        std::string name_;
        argument_list args_;
        Typename return_type_;
        TreePtr body_;
};
typedef std::shared_ptr<Defun> DefunPtr;

std::ostream& operator<< (std::ostream &os, Defun const&);
} } }

#endif // DEFUN_H_INCLUDED_20120125
