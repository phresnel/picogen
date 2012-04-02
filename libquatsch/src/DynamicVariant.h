#ifndef DYNAMIC_VARIANT_H_INCLUDED_20120402
#define DYNAMIC_VARIANT_H_INCLUDED_20120402

#include "Typename.h"
#include <vector>

namespace quatsch {

class DynamicVariant {
public:
        static DynamicVariant Floating (float value);
        static DynamicVariant Integer  (int   value);

        float floating() const;
        int   integer () const;

        Typename type() const;
private:
        DynamicVariant (Typename type_);

        Typename type_;
        float floating_;
        int   integer_;
};
typedef std::vector<DynamicVariant> DynamicArguments;

}

#endif // DYNAMIC_VARIANT_H_INCLUDED_20120402
