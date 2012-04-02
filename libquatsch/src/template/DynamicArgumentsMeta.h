#ifndef DYNAMIC_ARGUMENTS_META_H_INCLUDED_20120402
#define DYNAMIC_ARGUMENTS_META_H_INCLUDED_20120402

#include <initializer_list>
#include <list>
#include "Typename.h"

namespace quatsch { namespace extern_template {

class DynamicArgumentsMeta {
        typedef std::list<Typename>::size_type size_type;
public:
        typedef Typename value_type;
        typedef std::list<Typename>::const_iterator const_iterator;

        DynamicArgumentsMeta (std::initializer_list<Typename> types) ;

        template <typename ...Args>
        DynamicArgumentsMeta (Args ...args) : list_(args...)
        {}

        const_iterator begin() const ;
        const_iterator end  () const ;

        size_type size() const ;

private:
        std::list<Typename> list_;
};

inline
DynamicArgumentsMeta::const_iterator begin (DynamicArgumentsMeta const &l)
{ return l.begin(); }

inline
DynamicArgumentsMeta::const_iterator end (DynamicArgumentsMeta const &l)
{ return l.end(); }


} }

#endif // DYNAMIC_ARGUMENTS_META_H_INCLUDED_20120402
