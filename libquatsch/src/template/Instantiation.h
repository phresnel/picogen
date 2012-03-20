#ifndef INSTANTIATION_H_INCLUDED_20120302
#define INSTANTIATION_H_INCLUDED_20120302

#include "Typename.h"
#include <list>
#include <functional>
#include <vector>

namespace quatsch { namespace extern_template {

class DynamicVariant {
public:
        static DynamicVariant Floating (float value);
        static DynamicVariant Integer  (int   value);

        float floating() const;
        int   integer () const;
private:
        // No need for a public type() function. The call should be verified already.
        DynamicVariant (Typename type_);

        Typename type_;
        float floating_;
        int   integer_;
};
typedef std::vector<DynamicVariant> DynamicArguments;


class DynamicArgumentsMeta {
        typedef std::list<Typename>::size_type size_type;
public:
        typedef Typename value_type;
        typedef std::list<Typename>::const_iterator const_iterator;

        DynamicArgumentsMeta (std::initializer_list<Typename> types) : list_(types)
        {}

        template <typename ...Args>
        DynamicArgumentsMeta (Args ...args) : list_(args...)
        {}

        const_iterator begin() const { return list_.begin(); }
        const_iterator end  () const { return list_.end  (); }

        size_type size() const { return list_.size(); }

private:
        std::list<Typename> list_;
};
inline DynamicArgumentsMeta::const_iterator begin (DynamicArgumentsMeta const &l) { return l.begin(); }
inline DynamicArgumentsMeta::const_iterator end   (DynamicArgumentsMeta const &l) { return l.end(); }



/*
class Instantiation // should also describe runtime-argument list!
{
public:
        Instantiation(Typename return_type, DynamicArgumentsMeta const&,
                      std::function<DynamicVariant(DynamicArguments)> const &);
        Typename return_type() const;
        DynamicArgumentsMeta arguments_meta() const;

        // TODO: I think the above meta functions should directly given back
        //       to the caller of incarnate(), as the caller will only have
        //       to validate once.
        // TODO: should instantiate_() just return std::function?

        DynamicVariant operator() (DynamicArguments const& in) const;

private:
        Typename return_type_;
        DynamicArgumentsMeta arg_desc_;
        std::function<DynamicVariant(DynamicArguments)> fun_;
};*/
typedef std::function<DynamicVariant(DynamicArguments const&)> extern_function;

struct Instantiation
{
        extern_function      function;
        Typename             return_type;
        DynamicArgumentsMeta arguments_meta;
};


} }

#endif // INSTANTIATION_H_INCLUDED_20120302
