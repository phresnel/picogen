#ifndef INSTANTIATION_H_INCLUDED_20120302
#define INSTANTIATION_H_INCLUDED_20120302

#include "Typename.h"
#include <list>

namespace quatsch { namespace extern_template {

class DynamicArgument {
public:
        static DynamicArgument Floating (float value);
        static DynamicArgument Integer  (int   value);

        float floating() const;
        int   integer () const;
private:
        // No need for a public type() function. The call should be verified already.
        DynamicArgument (Typename type_);

        Typename type_;
        float floating_;
        int   integer_;
};


class DynamicArgumentsMeta {
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

private:
        std::list<Typename> list_;
};
inline DynamicArgumentsMeta::const_iterator begin (DynamicArgumentsMeta const &l) { return l.begin(); }
inline DynamicArgumentsMeta::const_iterator end   (DynamicArgumentsMeta const &l) { return l.end(); }



class Instantiation // should also describe runtime-argument list!
{
public:
        Typename return_type() const;
        DynamicArgumentsMeta arguments_meta() const;

private:
        Typename return_type_;
        DynamicArgumentsMeta arg_desc_;
};

} }

#endif // INSTANTIATION_H_INCLUDED_20120302
